#pragma once

#include "gfx_gl.h"
#include <vector>

class Cubes_GL_BufferStorage : public Cubes
{
public:
    Cubes_GL_BufferStorage();
    virtual ~Cubes_GL_BufferStorage();

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

    GLuint m_transform_buffer;
    void *m_transform_ptr;

    std::vector<DrawElementsIndirectCommand> m_commands;
    GLuint m_cmd_buffer;
    void *m_cmd_ptr;
};
