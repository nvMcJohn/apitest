#pragma once

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsGLBindlessIndirect : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsGLBindlessIndirect();
    virtual ~UntexturedObjectsGLBindlessIndirect() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const { return "GLBindlessIndirect"; }
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    struct Command
    {
        DrawElementsIndirectCommand Draw;
        GLuint                      reserved; 
        BindlessPtrNV               indexBuffer;
        BindlessPtrNV               vertexBuffers[2];
    };

    void resolveQueries();

    std::vector<GLuint> m_ibs;
    std::vector<GLuint64> m_ib_addrs;
    std::vector<GLsizeiptr> m_ib_sizes;
    std::vector<GLuint> m_vbs;
    std::vector<GLuint64> m_vbo_addrs;
    std::vector<GLsizeiptr> m_vbo_sizes;
    GLuint m_prog;
    GLuint m_vao;

    std::vector<GLuint> m_queries;
    GLint m_currentQueryIssue;
    GLint m_currentQueryGet;

    GLuint m_transform_buffer;
    void *m_transform_ptr;

    std::vector<Command> m_commands;
    GLuint m_cmd_buffer;
    void *m_cmd_ptr;

    struct UniformLocations {
        GLuint ViewProjection;
        GLuint World;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
