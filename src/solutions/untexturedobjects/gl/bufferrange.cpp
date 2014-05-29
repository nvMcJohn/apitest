#include "pch.h"

#include "bufferrange.h"
#include "framework/gfx_gl.h"
#include <climits>

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLBufferRange::UntexturedObjectsGLBufferRange()
: mIndexBuffer()
, mVertexBuffer()
, mUniformBuffer()
, mVertexArrayObject()
, mProgram()
, mMatrixStride()
, mMaxUniformBlockSize()
, mMaxBatchSize()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsGLBufferRange::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                          const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                          size_t _objectCount)
{
    if (!UntexturedObjectsSolution::Init(_vertices, _indices, _objectCount)) {
        return false;
    }

    // Program
    const char* kUniformNames[] = { "ViewProjection", nullptr };

    mProgram = CreateProgramT("cubes_gl_buffer_range_vs.glsl",
                              "cubes_gl_buffer_range_fs.glsl",
                              kUniformNames, &mUniformLocation);

    if (mProgram == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    GLuint UB0 = glGetUniformBlockIndex(mProgram, "UB0");
    glUniformBlockBinding(mProgram, UB0, 0);

    glGenBuffers(1, &mVertexBuffer);
    glGenBuffers(1, &mIndexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    BufferData(GL_ARRAY_BUFFER, _vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    BufferData(GL_ELEMENT_ARRAY_BUFFER, _indices, GL_STATIC_DRAW);

    GLint uniformBufferOffsetAlignment = 0;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &uniformBufferOffsetAlignment);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &mMaxUniformBlockSize);
    mMatrixStride = iceil(sizeof(Matrix), uniformBufferOffsetAlignment);
    mMaxBatchSize = mMaxUniformBlockSize / mMatrixStride;

    glGenBuffers(1, &mUniformBuffer);
    mStorage.resize(mMatrixStride * mMaxBatchSize);

    return GLRenderer::GetApiError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBufferRange::Render(const std::vector<Matrix>& _transforms)
{
    assert(_transforms.size() <= INT_MAX);
    int xformCount = (int)_transforms.size();

    // Program
    Vec3 dir = { -0.5f, -1, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 0, 1 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
    Matrix view = matrix_look_at(eye, at, up);
    Matrix view_proj = mProj * view;

    glUseProgram(mProgram);
    glUniformMatrix4fv(mUniformLocation.ViewProjection, 1, GL_TRUE, &view_proj.x.x);

    // Input Layout
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(UntexturedObjectsProblem::Vertex), (void*)offsetof(UntexturedObjectsProblem::Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(UntexturedObjectsProblem::Vertex), (void*)offsetof(UntexturedObjectsProblem::Vertex, color));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Rasterizer State
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDisable(GL_SCISSOR_TEST);

    // Blend State
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Depth Stencil State
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);

    for (int batchStart = 0; batchStart < xformCount; batchStart += mMaxBatchSize)
    {
        int batchCount = std::min(xformCount - batchStart, mMaxBatchSize);

        for (int i = 0; i < batchCount; ++i)
        {
            char *dst = &mStorage[mMatrixStride * i];
            *(Matrix *)dst = _transforms[batchStart + i];
        }

        BufferData(GL_UNIFORM_BUFFER, mStorage, GL_DYNAMIC_DRAW);

        for (int i = 0; i < batchCount; ++i)
        {
            glBindBufferRange(GL_UNIFORM_BUFFER, 0, mUniformBuffer, mMatrixStride * i, sizeof(Matrix));

            glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, nullptr);
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBufferRange::Shutdown()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mUniformBuffer);
    glDeleteVertexArrays(1, &mVertexArrayObject);
    glDeleteProgram(mProgram);
}
