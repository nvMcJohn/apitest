#pragma once

#include "gfx_gl.h"

class Cubes_GL_MultiDraw : public Cubes
{
public:
    Cubes_GL_MultiDraw();
    virtual ~Cubes_GL_MultiDraw();

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

    GLuint m_ib;
    GLuint m_vb;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    GLuint m_transform_buffer;

    DrawElementsIndirectCommand m_commands[CUBES_COUNT];
};
