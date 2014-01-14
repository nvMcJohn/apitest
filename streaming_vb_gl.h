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

    virtual void draw(VertexPos2* vertices, int count) override;

private:
    struct Constants
    {
        float width;
        float height;
        float pad[2];
    };

    GLuint m_ub;
    GLuint m_vs;
    GLuint m_fs;
    GLuint m_prog;

    GLuint m_dyn_vb;
    int m_dyn_offset;
    void* m_dyn_vb_ptr;
};
