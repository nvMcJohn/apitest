#pragma once

#include "gfx_gl.h"

#define QUERY_COUNT 4

class Cubes_GL_BindlessIndirect : public Cubes
{
public:
    Cubes_GL_BindlessIndirect();
    virtual ~Cubes_GL_BindlessIndirect();

    virtual bool Init() override;

    virtual bool Begin(GfxBaseApi* _activeAPI) override;

    virtual void Draw(Matrix* transforms, int count) override;

private:
    struct Vertex
    {
        Vec3 pos;
        Vec3 color;
    };

    struct Command
    {
        DrawElementsIndirectCommand Draw;
        GLuint                      reserved; 
        BindlessPtrNV               indexBuffer;
        BindlessPtrNV               vertexBuffers[2];
    };

    void resolveQueries();

    GLuint m_ibs[CUBES_COUNT];
    GLuint64 m_ib_addrs[CUBES_COUNT];
    GLsizeiptr m_ib_sizes[CUBES_COUNT];
    GLuint m_vbs[CUBES_COUNT];
    GLuint64 m_vbo_addrs[CUBES_COUNT];
    GLsizeiptr m_vbo_sizes[CUBES_COUNT];
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    GLuint m_queries[QUERY_COUNT];
    GLint m_currentQueryIssue;
    GLint m_currentQueryGet;

    GLuint m_transform_buffer;
    void *m_transform_ptr;

    Command m_commands[CUBES_COUNT];
    GLuint m_cmd_buffer;
    void *m_cmd_ptr;
};
