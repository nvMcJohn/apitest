#pragma once

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLUniform : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLUniform();
    virtual ~UntexturedObjectsGLUniform() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const { return "GLUniform"; }
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    GLuint m_ib;
    GLuint m_vb;
    GLuint m_prog;

    struct UniformLocations {
        GLuint ViewProjection;
        GLuint World;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
