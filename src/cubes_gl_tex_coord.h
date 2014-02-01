#pragma once

#include "gfx_gl.h"
#include "mathlib.h"

class Cubes_GL_TexCoord : public Cubes
{
public:
    Cubes_GL_TexCoord();
    virtual ~Cubes_GL_TexCoord();

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
};
