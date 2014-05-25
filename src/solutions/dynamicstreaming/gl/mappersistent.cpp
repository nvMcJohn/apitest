#include "pch.h"

#include "problems/dynamicstreaming.h"
#include "mappersistent.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
DynamicStreamingGLMapPersistent::DynamicStreamingGLMapPersistent()
: mUniformBuffer()
, mVertexBuffer()
, mProgram()
, mStartDestOffset()
, mParticleBufferSize()
, mBufferLockManager(true)
{ }

// --------------------------------------------------------------------------------------------------------------------
DynamicStreamingGLMapPersistent::~DynamicStreamingGLMapPersistent()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool DynamicStreamingGLMapPersistent::Init(size_t _maxVertexCount)
{
    if (glBufferStorage == nullptr) {
        console::warn("Unable to initialize solution '%s', glBufferStorage() unavailable.", GetName().c_str());
        return false;
    }

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
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

    mParticleBufferSize = kTripleBuffer * sizeof(Vec2) * _maxVertexCount;
    const GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
    glBufferStorage(GL_ARRAY_BUFFER, mParticleBufferSize, NULL, flags);
    mVertexDataPtr = glMapBufferRange(GL_ARRAY_BUFFER, 0, mParticleBufferSize, flags);

    return GLRenderer::GetApiError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void DynamicStreamingGLMapPersistent::Render(const std::vector<Vec2>& _vertices)
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
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)offsetof(Vec2, x));
    glEnableVertexAttribArray(0);

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

    // Need to wait for this area to become available. If we've sized things properly, it will always be 
    // available right away.
    mBufferLockManager.WaitForLockedRange(mStartDestOffset, _vertices.size() * sizeof(Vec2));

    for (int i = 0; i < kParticleCount; ++i)
    {
        const int vertexOffset = i * kVertsPerParticle;
        const int srcOffset = vertexOffset;
        const int dstOffset = mStartDestOffset + (i * kParticleSizeBytes);

        void* dst = (unsigned char*) mVertexDataPtr + dstOffset;
        memcpy(dst, &_vertices[vertexOffset], kParticleSizeBytes);

        glDrawArrays(GL_TRIANGLES, kStartIndex + vertexOffset, kVertsPerParticle);
    }

    // Lock this area for the future.
    mBufferLockManager.LockRange(mStartDestOffset, _vertices.size() * sizeof(Vec2));

    mStartDestOffset = (mStartDestOffset + (kParticleCount * kParticleSizeBytes)) % mParticleBufferSize;
}

// --------------------------------------------------------------------------------------------------------------------
void DynamicStreamingGLMapPersistent::Shutdown()
{
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glDeleteBuffers(1, &mVertexBuffer);

    glDeleteBuffers(1, &mUniformBuffer);
    glDeleteProgram(mProgram);
}

