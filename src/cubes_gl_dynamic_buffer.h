#pragma once

#include "gfx_gl.h"
#include "mathlib.h"

class Cubes_GL_DynamicBuffer : public Cubes
{
public:
    Cubes_GL_DynamicBuffer();
    virtual ~Cubes_GL_DynamicBuffer();

    virtual bool init() override;

    virtual bool begin(GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer) override;
    virtual void end(GfxSwapChain* swap_chain) override;

    virtual void draw(Matrix* transforms, int count) override;

private:
    struct Vertex
    {
        Vec3 pos;
        Vec3 color;
    };

    struct Constants
    {
        Matrix world;
    };

    GLuint m_ib;
    GLuint m_vb;
    GLuint m_ub;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;
};
