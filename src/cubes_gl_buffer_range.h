#pragma once

#include "gfx_gl.h"
#include <vector>

class Cubes_GL_BufferRange : public Cubes
{
public:
    Cubes_GL_BufferRange();
    virtual ~Cubes_GL_BufferRange();

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
    GLuint m_ub;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    std::vector<char> storage;
};
