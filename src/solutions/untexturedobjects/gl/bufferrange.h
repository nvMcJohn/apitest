#pragma once

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLBufferRange : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLBufferRange();
    virtual ~UntexturedObjectsGLBufferRange() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    // The name of this solution.
    virtual std::string GetName() const { return "UntexturedObjectsGLBufferRange"; }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mUniformBuffer;
    GLuint mProgram;

    GLint mMatrixStride;
    GLint mMaxUniformBlockSize;
    GLuint mMaxBatchSize;

    std::vector<char> mStorage;

    struct UniformLocations {
        GLuint ViewProjection;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
