#pragma once

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLMultiDraw : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLMultiDraw(bool _useShaderDraw);
    virtual ~UntexturedObjectsGLMultiDraw() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const; 
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api, true); }

private:
    GLuint m_ib;
    GLuint m_vb;
    GLuint m_varray;
    GLuint m_drawid;
    GLuint m_prog;

    GLuint m_transform_buffer;

    bool mUseShaderDrawParameters;

    std::vector<DrawElementsIndirectCommand> m_commands;

    struct UniformLocations {
        GLuint ViewProjection;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
