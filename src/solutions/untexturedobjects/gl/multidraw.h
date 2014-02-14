#pragma once

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLMultiDraw : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLMultiDraw();
    virtual ~UntexturedObjectsGLMultiDraw() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    // The name of this solution.
    virtual std::string GetName() const { return "UntexturedObjectsGLMultiDraw"; }

private:
    GLuint m_ib;
    GLuint m_vb;
    GLuint m_prog;

    GLuint m_transform_buffer;

    std::vector<DrawElementsIndirectCommand> m_commands;
};
