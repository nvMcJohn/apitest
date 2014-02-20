#pragma once

#include "solutions/dynamicstreamingsoln.h"

// TODO: Split these out into separate solutions...
#define GL_USE_PERSISTENT_MAP 1
#define GL_USE_MAP 1

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class DynamicStreamingPersistent : public DynamicStreamingSolution
{
public:
    DynamicStreamingPersistent();
    virtual ~DynamicStreamingPersistent();

    virtual bool Init() override;
    virtual void Render(const std::vector<Vec2>& _vertices) override;
    virtual void Shutdown() override;

    // The name of this solution.
    virtual std::string GetName() const override { return "DynamicStreamingPersistent"; }

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

    struct UniformLocations {
        GLuint CB0;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;

#if GL_USE_PERSISTENT_MAP
    void* mVertexDataPtr;
#endif
};
