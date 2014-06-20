#pragma once

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLDynamicBuffer : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLDynamicBuffer();
    virtual ~UntexturedObjectsGLDynamicBuffer() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    // The name of this solution.
    virtual std::string GetName() const { return "GLDynamicBuffer"; }
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    struct Constants
    {
        Matrix world;
    };

    GLuint m_ib;
    GLuint m_vb;
    GLuint m_ub;
    GLuint m_va;
    GLuint m_prog;
    GLuint m_vao;

    struct UniformLocations {
        GLuint ViewProjection;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
