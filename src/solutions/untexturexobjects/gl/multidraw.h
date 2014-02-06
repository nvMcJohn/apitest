#pragma once

#include "gfx_gl.h"

class Cubes_GL_MultiDraw : public Cubes
{
public:
    Cubes_GL_MultiDraw();
    virtual ~Cubes_GL_MultiDraw();

    virtual bool Init() override;

    virtual bool Begin(GfxBaseApi* _activeAPI) override;

    virtual void Draw(Matrix* transforms, int count) override;

private:
    struct Vertex
    {
        Vec3 pos;
        Vec3 color;
    };

    GLuint m_ib;
    GLuint m_vb;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    GLuint m_transform_buffer;

    DrawElementsIndirectCommand m_commands[CUBES_COUNT];
};
