#pragma once

#include "solutions/dynamicstreamingsoln.h"
#include "framework/bufferlock.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class DynamicStreamingGLMapPersistent : public DynamicStreamingSolution
{
public:
    DynamicStreamingGLMapPersistent();
    virtual ~DynamicStreamingGLMapPersistent();

    virtual bool Init(size_t _maxVertexCount) override;
    virtual void Render(const std::vector<Vec2>& _vertices) override;
    virtual void Shutdown() override;

    virtual std::string GetName() const override { return "GLMapPersistent"; }
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

    size_t mStartDestOffset;
    size_t mParticleBufferSize;

    BufferLockManager mBufferLockManager;

    struct UniformLocations {
        GLuint CB0;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;

    void* mVertexDataPtr;
    BufferLock* mBufferLock;
};
