#pragma once

#include "solutions/dynamicstreamingsoln.h"
#include "framework/bufferlock.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class DynamicStreamingGLMapUnsynchronized : public DynamicStreamingSolution
{
public:
    DynamicStreamingGLMapUnsynchronized();
    virtual ~DynamicStreamingGLMapUnsynchronized();

    virtual bool Init(size_t _maxVertexCount) override;
    virtual void Render(const std::vector<Vec2>& _vertices) override;
    virtual void Shutdown() override;

    virtual std::string GetName() const override { return "GLMapUnsynchronized"; }
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    struct Constants
    {
        float width;
        float height;
        float pad[2];
    };

    GLuint mUniformBuffer;
    GLuint mVertexBuffer;
    GLuint mProgram;
    GLuint mVAO;

    size_t mStartDestOffset;
    size_t mParticleBufferSize;

    BufferLockManager mBufferLockManager;

    struct UniformLocations {
        GLuint CB0;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
