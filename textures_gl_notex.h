#pragma once

#include "gfx_gl.h"

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class Textures_GL_NoTex : public Textures
{
public:
    Textures_GL_NoTex();
    virtual ~Textures_GL_NoTex();

    virtual bool init() override;

    virtual bool begin(GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer) override;
    virtual void end(GfxSwapChain* swap_chain) override;

    virtual void draw(Matrix* transforms, int count) override;

private:
    struct Vertex
    {
        Vec3 pos;
        Vec2 tex;
    };

    GLuint m_ib;
    GLuint m_vb_pos;
    GLuint m_vb_tex;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    GLuint m_transform_buffer;

    DrawElementsIndirectCommand m_commands[TEXTURES_COUNT];
};
