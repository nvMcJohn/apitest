#include "pch.h"

#include "problems/dynamicstreaming.h"
#include "mapunsynchronized.h"
#include "framework/gfx_gl.h"

const size_t kTripleBuffer = 3;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
DynamicStreamingGLMapUnsynchronized::DynamicStreamingGLMapUnsynchronized()
: mUniformBuffer()
, mVertexBuffer()
, mProgram()
, mStartDestOffset()
, mParticleBufferSize()
, mBufferLockManager(true)

{ }

// --------------------------------------------------------------------------------------------------------------------
DynamicStreamingGLMapUnsynchronized::~DynamicStreamingGLMapUnsynchronized()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool DynamicStreamingGLMapUnsynchronized::Init(size_t _maxVertexCount)
{
    // Uniform Buffer
    glGenBuffers(1, &mUniformBuffer);

    // Program
    const char* kUniformNames[] = { "CB0", nullptr };

    mProgram = CreateProgramT("streaming_vb_gl_vs.glsl",
                              "streaming_vb_gl_fs.glsl",
                              kUniformNames, &mUniformLocation);

    if (mProgram == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    // Dynamic vertex buffer
    mParticleBufferSize = kTripleBuffer * sizeof(Vec2) * _maxVertexCount;
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mParticleBufferSize, nullptr, GL_DYNAMIC_DRAW);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void DynamicStreamingGLMapUnsynchronized::Render(const std::vector<Vec2>& _vertices)
{
    // Program
    glUseProgram(mProgram);

    // Uniforms
    Constants cb;
    cb.width = 2.0f / mWidth;
    cb.height = -2.0f / mHeight;

    glBindBuffer(GL_UNIFORM_BUFFER, mUniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(Constants), &cb, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, mUniformBuffer);

    // Input Layout
    glEnableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)offsetof(Vec2, x));

    // Rasterizer State
    glDisable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, mWidth, mHeight);

    // Blend State
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Depth Stencil State
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);

    const int kParticleCount = int(_vertices.size()) / kVertsPerParticle;
    const int kParticleSizeBytes = int(kVertsPerParticle * sizeof(Vec2));
    const int kStartIndex = mStartDestOffset / sizeof(Vec2);
    const GLbitfield kAccess = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;

    mBufferLockManager.WaitForLockedRange(mStartDestOffset, _vertices.size() * sizeof(Vec2));

    for (int i = 0; i < kParticleCount; ++i)
    {
        const int vertexOffset = i * kVertsPerParticle;
        const int srcOffset = vertexOffset;
        const int dstOffset = mStartDestOffset + (i * kParticleSizeBytes);

        void* dst = glMapBufferRange(GL_ARRAY_BUFFER, dstOffset, kParticleSizeBytes, kAccess);
        if (dst) {
            memcpy(dst, &_vertices[vertexOffset], kParticleSizeBytes);
            glUnmapBuffer(GL_ARRAY_BUFFER);

            glDrawArrays(GL_TRIANGLES, kStartIndex + vertexOffset, kVertsPerParticle);
        }
    }

    mBufferLockManager.LockRange(mStartDestOffset, _vertices.size() * sizeof(Vec2));

    mStartDestOffset = (mStartDestOffset + (kParticleCount * kParticleSizeBytes)) % mParticleBufferSize;
}

// --------------------------------------------------------------------------------------------------------------------
void DynamicStreamingGLMapUnsynchronized::Shutdown()
{
    glDeleteBuffers(1, &mVertexBuffer);

    glDeleteBuffers(1, &mUniformBuffer);
    glDeleteProgram(mProgram);
}

