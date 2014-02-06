#pragma once

#include "gfx_gl.h"

class Cubes_GL_Bindless : public Cubes
{
public:
    Cubes_GL_Bindless();
    virtual ~Cubes_GL_Bindless();

    virtual bool Init() override;

    virtual bool Begin(GfxBaseApi* _activeAPI) override;

    virtual void Draw(Matrix* _transforms, int _count) override;

private:
    struct Vertex
    {
        Vec3 pos;
        Vec3 color;
    };

    GLuint m_ibs[CUBES_COUNT];
    GLuint64 m_ib_addrs[CUBES_COUNT];
    GLsizeiptr m_ib_sizes[CUBES_COUNT];
    GLuint m_vbs[CUBES_COUNT];
    GLuint64 m_vbo_addrs[CUBES_COUNT];
    GLsizeiptr m_vbo_sizes[CUBES_COUNT];
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;
};
