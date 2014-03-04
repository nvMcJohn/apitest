#pragma once

#include "solutions/untexturedobjectssoln.h"
#include "framework/bufferlock.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLMapPersistent : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLMapPersistent();
    virtual ~UntexturedObjectsGLMapPersistent() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    // The name of this solution.
    virtual std::string GetName() const { return "UntexturedObjectsGLMapPersistent"; }

private:
    GLuint m_ib;
    GLuint m_vb;
    GLuint m_varray;
    GLuint m_drawid;
    GLuint m_prog;

    GLuint m_transform_buffer;

    size_t mStartDestOffset;
    size_t mTransformBufferSize;

    BufferLockManager mBufferLockManager;

    struct UniformLocations {
        GLuint ViewProjection;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;

    void* mTransformDataPtr;
};
