#pragma once

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLBufferStorage : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLBufferStorage();
    virtual ~UntexturedObjectsGLBufferStorage() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    // The name of this solution.
    virtual std::string GetName() const { return "UntexturedObjectsGLBufferStorage"; }

private:
    GLuint m_ib;
    GLuint m_vb;
    GLuint m_prog;

    GLuint m_transform_buffer;
    void *m_transform_ptr;

    std::vector<DrawElementsIndirectCommand> m_commands;
    GLuint m_cmd_buffer;
    void *m_cmd_ptr;

    struct UniformLocations {
        GLuint ViewProjection;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;

};
