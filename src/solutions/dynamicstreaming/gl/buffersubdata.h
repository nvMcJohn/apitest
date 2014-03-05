#pragma once

#include "solutions/dynamicstreamingsoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class DynamicStreamingGLBufferSubData : public DynamicStreamingSolution
{
public:
    DynamicStreamingGLBufferSubData();
    virtual ~DynamicStreamingGLBufferSubData();

    virtual bool Init(size_t _maxVertexCount) override;
    virtual void Render(const std::vector<Vec2>& _vertices) override;
    virtual void Shutdown() override;

    virtual std::string GetName() const override { return "DynamicStreamingGLBufferSubData"; }
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

    // TODO: Sync objects

    struct UniformLocations {
        GLuint CB0;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
