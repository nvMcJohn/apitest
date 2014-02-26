#include "pch.h"

#include "texturearraymultidraw.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsGLTextureArrayMultiDraw::TexturedQuadsGLTextureArrayMultiDraw(bool _useShaderDrawParameters)
: mIndexBuffer()
, mVertexBuffer()
, mDrawIDBuffer()
, mVertexArray()
, mProgram()
, mTransformBuffer()
, mTexAddressBuffer()
, mUseShaderDrawParameters(_useShaderDrawParameters)
{ }

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsGLTextureArrayMultiDraw::Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
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

    if (mUseShaderDrawParameters && !HasExtension("GL_ARB_shader_draw_parameters")) {
        console::warn("Unable to initialize solution, ARB_shader_draw_parameters is required but not available.");
        return false;
    }

    // Program
    const char* kUniformNames[] = { "ViewProjection", "TexContainer", nullptr };

    mProgram = CreateProgramT("textures_gl_texture_array_multidraw_vs.glsl",
                              "textures_gl_texture_array_multidraw_fs.glsl",
                              mUseShaderDrawParameters ? std::string("#define USE_SHADER_DRAW_PARAMETERS 1\n") : std::string(""),
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
    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    mVertexBuffer = NewBufferFromVector(GL_ARRAY_BUFFER, _vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadsProblem::Vertex), (void*)offsetof(TexturedQuadsProblem::Vertex, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadsProblem::Vertex), (void*)offsetof(TexturedQuadsProblem::Vertex, tex));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // If we aren't using shader draw parameters, use the workaround instead.
    if (!mUseShaderDrawParameters) {
        std::vector<uint32_t> drawids(_objectCount);
        for (uint32_t i = 0; i < _objectCount; ++i) {
            drawids[i] = i;
        }

        mDrawIDBuffer = NewBufferFromVector(GL_ARRAY_BUFFER, drawids, GL_STATIC_DRAW);
        glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(uint32_t), 0);
        glVertexAttribDivisor(2, 1);
        glEnableVertexAttribArray(2);
    }

    mIndexBuffer = NewBufferFromVector(GL_ELEMENT_ARRAY_BUFFER, _indices, GL_STATIC_DRAW);

    glGenBuffers(1, &mTransformBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mTransformBuffer);

    // Set the command buffer size.
    m_commands.resize(_objectCount);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLTextureArrayMultiDraw::Render(const std::vector<Matrix>& _transforms)
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
        DrawElementsIndirectCommand *cmd = &m_commands[u];
        cmd->count = mIndexCount;
        cmd->instanceCount = 1;
        cmd->firstIndex = 0;
        cmd->baseVertex = 0;
        cmd->baseInstance = u;
    }

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, &*m_commands.begin(), xformCount, 0);
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLTextureArrayMultiDraw::Shutdown()
{
    for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
        SafeDelete(*it);
    }

    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mDrawIDBuffer);
    glDeleteVertexArrays(1, &mVertexArray);
    glDeleteBuffers(1, &mTransformBuffer);
    glDeleteBuffers(1, &mTexAddressBuffer);
    glDeleteProgram(mProgram);

    mTextures.clear();
    mTexUnits.clear();

    mTexManager.Shutdown();
}

// --------------------------------------------------------------------------------------------------------------------
std::string TexturedQuadsGLTextureArrayMultiDraw::GetName() const
{
    if (mUseShaderDrawParameters) {
        return "TexturedQuadsGLTextureArrayMultiDraw-SDP";
    }

    return "TexturedQuadsGLTextureArrayMultiDraw-NoSDP";
}
