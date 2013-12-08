// Streaming VB test - GL Implementation

#pragma once

#include "gfx_gl.h"

class StreamingVB_GL : public StreamingVB
{
public:
    StreamingVB_GL();
    virtual ~StreamingVB_GL();

    virtual bool init() override;

    virtual bool begin(void* hwnd, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer) override;
    virtual void end(GfxSwapChain* swap_chain) override;

    virtual void draw(Vert* vertices, int count) override;

private:
    GLuint m_ub;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    GLuint m_dyn_vb;
    int m_dyn_offset;
};
