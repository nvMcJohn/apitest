#pragma once

#include "solutions/untexturedobjectssoln.h"
#include "framework/buffer.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLBufferStorage : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLBufferStorage(bool _useShaderDrawParameters);
    virtual ~UntexturedObjectsGLBufferStorage() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const;
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mVertexArrayObject;
    GLuint mDrawIdBuffer;
    GLuint mProgram;

    CircularBuffer<Matrix> mTransformBuffer;
    CircularBuffer<DrawElementsIndirectCommand> mCommands;
    bool mUseShaderDrawParameters;

    struct UniformLocations {
        GLuint ViewProjection;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
