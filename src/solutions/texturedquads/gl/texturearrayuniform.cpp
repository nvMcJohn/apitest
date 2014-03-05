#include "pch.h"

#include "texturearrayuniform.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsGLTextureArrayUniform::TexturedQuadsGLTextureArrayUniform()
: mIndexBuffer()
, mVertexBuffer()
, mProgram()
, mTransformBuffer()
, mTexAddressBuffer()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsGLTextureArrayUniform::Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                                              const std::vector<TexturedQuadsProblem::Index>& _indices,
                                              const std::vector<TextureDetails*>& _textures,
                                              size_t _objectCount)
{
    if (!TexturedQuadsSolution::Init(_vertices, _indices, _textures, _objectCount)) {
        return false;
    }

    // Prerequisites
    auto numTextures = _textures.size();
    if (!mTexManager.Init(false, numTextures)) {
        return false;
    }

    // Program
    const char* kUniformNames[] = { "ViewProjection", "DrawID", "TexContainer", nullptr };

    mProgram = CreateProgramT("textures_gl_texture_array_uniform_vs.glsl",
                              "textures_gl_texture_array_uniform_fs.glsl",
                              kUniformNames, &mUniformLocation);

    if (mProgram == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    // Textures
    for (auto it = _textures.begin(); it != _textures.end(); ++it) {
        mTextures.push_back(mTexManager.newTexture2DFromDetails(*it));
    }

    GLint lastTexId = -1;
    GLint lastTexUnit = -1;
    std::vector<DenseTexAddress> texAddress(numTextures);
    for (size_t i = 0; i < numTextures; ++i) {
        auto texture = mTextures[i];
        auto texId = texture->GetTexId();
        if (lastTexId != texId) {
            lastTexId = texId;
            lastTexUnit = (GLint) mTexUnits.size();

            glActiveTexture(GL_TEXTURE0 + lastTexUnit);
            glBindTexture(GL_TEXTURE_2D_ARRAY, texId);
            mTexUnits.push_back(lastTexUnit);
        }

        texAddress[i].m_container_index = lastTexUnit;
        texAddress[i].m_layer = ((float) texture->getSliceNum() + 0.5f) / numTextures;
    }

    std::vector<DenseTexAddress> texAddressContents(_objectCount);
    for (uint32_t i = 0; i < _objectCount; ++i) {
        texAddressContents[i] = texAddress[i % numTextures];
    }

    mTexAddressBuffer = NewBufferFromVector(GL_SHADER_STORAGE_BUFFER, texAddressContents, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mTexAddressBuffer);

    // Buffers
    mVertexBuffer = NewBufferFromVector(GL_ARRAY_BUFFER, _vertices, GL_STATIC_DRAW);
    mIndexBuffer = NewBufferFromVector(GL_ELEMENT_ARRAY_BUFFER, _indices, GL_STATIC_DRAW);

    glGenBuffers(1, &mTransformBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mTransformBuffer);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLTextureArrayUniform::Render(const std::vector<Matrix>& _transforms)
{
    // Program
    Vec3 dir = { 0, 0, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 1, 0 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
    Matrix view = matrix_look_at(eye, at, up);
    Matrix view_proj = mProj * view;

    glUseProgram(mProgram);
    glUniformMatrix4fv(mUniformLocation.ViewProjection, 1, GL_TRUE, &view_proj.x.x);
    glUniform1iv(mUniformLocation.TexContainer, mTexUnits.size(), mTexUnits.data());

    // Input Layout. First the IB
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

    // Then the VBs.
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadsProblem::Vertex), (void*)offsetof(TexturedQuadsProblem::Vertex, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadsProblem::Vertex), (void*)offsetof(TexturedQuadsProblem::Vertex, tex));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Rasterizer State
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    glDisable(GL_SCISSOR_TEST);

    // Blend State
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Depth Stencil State
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mTransformBuffer);
    BufferData(GL_SHADER_STORAGE_BUFFER, _transforms, GL_DYNAMIC_DRAW);
    size_t xformCount = _transforms.size();
    assert(xformCount <= mObjectCount);

    for (size_t u = 0; u < xformCount; ++u) {
        glUniform1i(mUniformLocation.DrawID, u);

        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, 0);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLTextureArrayUniform::Shutdown()
{
    for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
        SafeDelete(*it);
    }

    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mTransformBuffer);
    glDeleteBuffers(1, &mTexAddressBuffer);
    glDeleteProgram(mProgram);

    mTextures.clear();
    mTexUnits.clear();

    mTexManager.Shutdown();
}
