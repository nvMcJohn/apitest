#include "pch.h"

#include "bindlessmultidraw.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsGLBindlessMultiDraw::TexturedQuadsGLBindlessMultiDraw()
: mIndexBuffer()
, mVertexBuffer()
, mProgram()
, mTransformBuffer()
, mTexAddressBuffer()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsGLBindlessMultiDraw::Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                                            const std::vector<TexturedQuadsProblem::Index>& _indices,
                                            const std::vector<TextureDetails*>& _textures,
                                            size_t _objectCount)
{
    if (!TexturedQuadsSolution::Init(_vertices, _indices, _textures, _objectCount)) {
        return false;
    }

    // Prerequisites
    if (glGetTextureHandleARB == nullptr) {
        console::warn("Unable to initialize solution '%s', requires support for bindless textures (not present).", GetName().c_str());
        return false;
    }

    // Programs
    mProgram = CreateProgram("textures_gl_bindless_multidraw_vs.glsl",
                             "textures_gl_bindless_multidraw_fs.glsl");

    if (mProgram == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    // Textures
    for (auto it = _textures.begin(); it != _textures.end(); ++it) {
        GLuint tex = NewTex2DFromDetails(*(*it));
        if (!tex) {
            console::warn("Unable to initialize solution '%s', texture creation failed.", GetName().c_str());
            return false;
        }

        // Needs to be freed later.
        mTextures.push_back(tex);

        GLuint64 texHandle = glGetTextureHandleARB(tex);
        if (texHandle == 0) {
            console::warn("Unable to initialize solution '%s', couldn't get texture handle.", GetName().c_str());
        }
        mTexHandles.push_back(texHandle);
    }

    // Buffers
    mVertexBuffer = NewBufferFromVector(GL_ARRAY_BUFFER, _vertices, GL_STATIC_DRAW);
    mIndexBuffer = NewBufferFromVector(GL_ELEMENT_ARRAY_BUFFER, _indices, GL_STATIC_DRAW);

    glGenBuffers(1, &mTransformBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mTransformBuffer);

    auto srcIt = mTexHandles.cbegin();
    std::vector<GLuint64> texAddressContents(_objectCount);
    for (auto dstIt = texAddressContents.begin(); dstIt != texAddressContents.end(); ++dstIt) {
        if (srcIt == mTexHandles.cend()) {
            srcIt = mTexHandles.cbegin();
        }

        (*dstIt) = *srcIt;
        ++srcIt;
    }

    mTexAddressBuffer = NewBufferFromVector(GL_SHADER_STORAGE_BUFFER, texAddressContents, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mTexAddressBuffer);

    mCommands.resize(_objectCount);

    return GLRenderer::GetApiError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLBindlessMultiDraw::Render(const std::vector<Matrix>& _transforms)
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
    glUniformMatrix4fv(0, 1, GL_TRUE, &view_proj.x.x);

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

    for (auto it = mTexHandles.begin(); it != mTexHandles.end(); ++it) {
        glMakeTextureHandleResidentARB(*it);
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mTransformBuffer);
    BufferData(GL_SHADER_STORAGE_BUFFER, _transforms, GL_DYNAMIC_DRAW);
    size_t xformCount = _transforms.size();
    assert(xformCount <= mObjectCount);

    for (size_t u = 0; u < xformCount; ++u) {
        DrawElementsIndirectCommand *cmd = &mCommands[u];
        cmd->count = mIndexCount;
        cmd->instanceCount = 1;
        cmd->firstIndex = 0;
        cmd->baseVertex = 0;
        cmd->baseInstance = 0;
    }

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, &*mCommands.begin(), xformCount, 0);

    for (auto it = mTexHandles.begin(); it != mTexHandles.end(); ++it) {
        glMakeTextureHandleNonResidentARB(*it);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLBindlessMultiDraw::Shutdown()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
        glDeleteTextures(1, &*it);
    }

    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mTransformBuffer);
    glDeleteProgram(mProgram);

    mCommands.clear();
    mTextures.clear();
    mTexHandles.clear();
}
