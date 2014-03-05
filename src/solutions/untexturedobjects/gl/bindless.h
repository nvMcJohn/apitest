#pragma once

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLBindless : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLBindless();
    virtual ~UntexturedObjectsGLBindless() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const { return "UntexturedObjectsGLBindless"; }
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    std::vector<GLuint> m_ibs;
    std::vector<GLuint64> m_ib_addrs;
    std::vector<GLsizeiptr> m_ib_sizes;
    std::vector<GLuint> m_vbs;
    std::vector<GLuint64> m_vbo_addrs;
    std::vector<GLsizeiptr> m_vbo_sizes;

    GLuint m_prog;

    struct UniformLocations {
        GLuint ViewProjection;
        GLuint World;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
