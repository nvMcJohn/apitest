#include "pch.h"

#include "notex.h"
#include "framework/gfx_gl.h"
#include <stdio.h>

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsGLNoTex::TexturedQuadsGLNoTex()
: mIndexBuffer()
, mVertexBuffer()
, mDrawIDBuffer()
, mVertexArray()
, mProgram()
, mTransformStorageBuffer()
, mMaxDrawsPerKickoff()
{}

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsGLNoTex::Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                                const std::vector<TexturedQuadsProblem::Index>& _indices,
                                const std::vector<TextureDetails*>& _textures,
                                size_t _objectCount)
{
    if (!TexturedQuadsSolution::Init(_vertices, _indices, _textures, _objectCount)) {
        return false;
    }

    if (!HasExtension(ARB_base_instance)) {
        // Currently required.
        return false;
    }

    if (!HasExtension(ARB_shader_storage_buffer_object)) {
        GLint maxUboSize = 0;
        glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxUboSize);
        mMaxDrawsPerKickoff = maxUboSize / sizeof(Matrix);
    }

    // Program
    const char* kUniformNames[] = { "ViewProjection", nullptr };

    mProgram = CreateProgramT("textures_gl_notex_vs.glsl",
                                "textures_gl_notex_fs.glsl",
                                kUniformNames, &mUniformLocation);

    if (mProgram == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    // Buffers
    glGenVertexArrays(1, &mVertexArray);
    glBindVertexArray(mVertexArray);

    mVertexBuffer = NewBufferFromVector(GL_ARRAY_BUFFER, _vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadsProblem::Vertex), (void*)offsetof(TexturedQuadsProblem::Vertex, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadsProblem::Vertex), (void*)offsetof(TexturedQuadsProblem::Vertex, tex));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    std::vector<uint32_t> drawids(_objectCount);
    for (uint32_t i = 0; i < _objectCount; ++i) {
        drawids[i] = i;
    }

    mDrawIDBuffer = NewBufferFromVector(GL_ARRAY_BUFFER, drawids, GL_STATIC_DRAW);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(uint32_t), 0);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(2);

    mIndexBuffer = NewBufferFromVector(GL_ELEMENT_ARRAY_BUFFER, _indices, GL_STATIC_DRAW);

    if (HasExtension(ARB_shader_storage_buffer_object)) {
        glGenBuffers(1, &mTransformStorageBuffer);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mTransformStorageBuffer);
    } else {
        mTransformUniformBuffers.resize(mMaxDrawsPerKickoff);
        glGenBuffers(mMaxDrawsPerKickoff, mTransformUniformBuffers.data());

        for (int i = 0; i < mMaxDrawsPerKickoff; ++i) {
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, mTransformUniformBuffers[i]);
        }
    }


    return GLRenderer::GetApiError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLNoTex::Render(const std::vector<Matrix>& _transforms)
{
    size_t xformCount = _transforms.size();
    assert(xformCount <= mObjectCount);

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

    if (HasExtension(ARB_shader_storage_buffer_object)) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, mTransformStorageBuffer);
        BufferData(GL_SHADER_STORAGE_BUFFER, _transforms, GL_DYNAMIC_DRAW);
    } else {
        assert(_transforms.size() <= mTransformUniformBuffers.size() * mMaxDrawsPerKickoff);

        GLint startXform = 0;
        GLint remainingXforms = signed(xformCount);
        for (auto it = mTransformUniformBuffers.begin(); it != mTransformUniformBuffers.end(); ++it) {
            GLint copyXformCount = std::min(mMaxDrawsPerKickoff, remainingXforms);

            glBindBuffer(GL_UNIFORM_BUFFER, *it);
            BufferDataArray(GL_UNIFORM_BUFFER, &_transforms[startXform], copyXformCount, GL_DYNAMIC_DRAW);

            remainingXforms -= copyXformCount;
            startXform += copyXformCount;
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, mTransformUniformBuffers[0]);
    }

    for (size_t u = 0; u < xformCount; ++u) {
        // This isn't supported on OSX. :(
        glDrawElementsInstancedBaseInstance(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, 0, 1, u);

        if (!HasExtension(ARB_shader_storage_buffer_object)) {
            if (((u + 1) % mMaxDrawsPerKickoff == 0) && (u + 1 < xformCount)) {
                glBindBufferBase(GL_UNIFORM_BUFFER, 0, mTransformUniformBuffers[(u + 1) / mMaxDrawsPerKickoff]);
            }
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLNoTex::Shutdown()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    // No textures to clean up here.

    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mDrawIDBuffer);
    glDeleteVertexArrays(1, &mVertexArray);
    if (HasExtension(ARB_shader_storage_buffer_object)) {
        glDeleteBuffers(1, &mTransformStorageBuffer);
    } else {
        glDeleteBuffers(mTransformUniformBuffers.size(), mTransformUniformBuffers.data());
        mTransformUniformBuffers.clear();
    }

    glDeleteProgram(mProgram);
}
