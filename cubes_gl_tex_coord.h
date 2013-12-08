#pragma once

#include "gfx_gl.h"
#include "mathlib.h"

class Cubes_GL_TexCoord : public Cubes
{
public:
    Cubes_GL_TexCoord();
    virtual ~Cubes_GL_TexCoord();

    virtual bool init() override;

    virtual bool begin(void* hwnd, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer) override;
    virtual void end(GfxSwapChain* swap_chain) override;

    virtual void draw(Matrix* transforms, int count) override;

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
