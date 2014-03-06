#include "pch.h"

#include "sparsebindlesstexturearraymultidraw.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsGLSparseBindlessTextureArrayMultiDraw::TexturedQuadsGLSparseBindlessTextureArrayMultiDraw(bool _useShaderDrawParameters)
: mIndexBuffer()
, mVertexBuffer()
, mDrawIDBuffer()
, mProgram()
, mTransformBuffer()
, mTexAddressBuffer()
, mUseShaderDrawParameters(_useShaderDrawParameters)
{ }

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsGLSparseBindlessTextureArrayMultiDraw::Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                                                              const std::vector<TexturedQuadsProblem::Index>& _indices,
                                                              const std::vector<TextureDetails*>& _textures,
                                                              size_t _objectCount)
{
    if (!TexturedQuadsSolution::Init(_vertices, _indices, _textures, _objectCount)) {
        return false;
    }

    // Prerequisites
    if (!mTexManager.Init()) {
        return false;
    }

    if (glGetTextureHandleARB == nullptr) {
        console::warn("Unable to initialize solution '%s', requires support for bindless textures (not present).", GetName().c_str());
        return false;
    }

    if (mUseShaderDrawParameters && !HasExtension("GL_ARB_shader_draw_parameters")) {
        console::warn("Unable to initialize solution, ARB_shader_draw_parameters is required but not available.");
        return false;
    }

    // Program
    mProgram = CreateProgram("textures_gl_sparse_bindless_texture_array_multidraw_vs.glsl",
                             "textures_gl_sparse_bindless_texture_array_multidraw_fs.glsl",
                             mUseShaderDrawParameters ? std::string("#define USE_SHADER_DRAW_PARAMETERS 1\n") : std::string("")
                             );

    if (mProgram == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    // Textures
    for (auto it = _textures.begin(); it != _textures.end(); ++it) {
        mTextures.push_back(mTexManager.newTexture2DFromDetails(*it));
    }

    // Buffers
    mVertexBuffer = NewBufferFromVector(GL_ARRAY_BUFFER, _vertices, GL_STATIC_DRAW);
    mIndexBuffer = NewBufferFromVector(GL_ELEMENT_ARRAY_BUFFER, _indices, GL_STATIC_DRAW);

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

    glGenBuffers(1, &mTransformBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mTransformBuffer);

    auto srcIt = mTextures.cbegin();
    std::vector<TexAddress> texAddressContents(_objectCount);
    for (auto dstIt = texAddressContents.begin(); dstIt != texAddressContents.end(); ++dstIt) {
        if (srcIt == mTextures.cend()) {
            srcIt = mTextures.cbegin();
        }

        (*dstIt) = (*srcIt)->GetAddress();
        ++srcIt;
    }

    mTexAddressBuffer = NewBufferFromVector(GL_SHADER_STORAGE_BUFFER, texAddressContents, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mTexAddressBuffer);

    mCommands.resize(_objectCount);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLSparseBindlessTextureArrayMultiDraw::Render(const std::vector<Matrix>& _transforms)
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

    for (size_t u = 0; u < xformCount; ++u)
    {
        DrawElementsIndirectCommand *cmd = &mCommands[u];
        cmd->count = mIndexCount;
        cmd->instanceCount = 1;
        cmd->firstIndex = 0;
        cmd->baseVertex = 0;
        cmd->baseInstance = mUseShaderDrawParameters ? 0 : u;
    }

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, &*mCommands.begin(), xformCount, 0);
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLSparseBindlessTextureArrayMultiDraw::Shutdown()
{
    for (auto it = mTextures.begin(); it != mTextures.end(); ++it) {
        SafeDelete(*it);
    }

    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteBuffers(1, &mDrawIDBuffer);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mTransformBuffer);
    glDeleteBuffers(1, &mTexAddressBuffer);
    glDeleteProgram(mProgram);

    mTextures.clear();
    mCommands.clear();

    mTexManager.Shutdown();
}

// --------------------------------------------------------------------------------------------------------------------
std::string TexturedQuadsGLSparseBindlessTextureArrayMultiDraw::GetName() const
{
    if (mUseShaderDrawParameters) {
        return "TexturedQuadsGLSBTAMultiDraw-SDP";
    }

    return "TexturedQuadsGLSBTAMultiDraw-NoSDP";
}
