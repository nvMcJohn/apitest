#pragma once

#include "gfx_gl.h"
#include <vector>

class Cubes_GL_BufferRange : public Cubes
{
public:
    Cubes_GL_BufferRange();
    virtual ~Cubes_GL_BufferRange();

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
    GLuint m_ub;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    std::vector<char> storage;
};
