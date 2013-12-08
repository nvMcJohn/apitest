// GL implementation

#pragma once

#include "gfx.h"

#include "wgl.h"

#define PTR_AS_INT(x)           static_cast<int>(reinterpret_cast<intptr_t>(x))

struct GfxSwapChain
{
    HDC hdc;
    HWND hwnd;
    HGLRC hglrc;
};

class GfxApi_GL : public GfxApi
{
public:
    GfxApi_GL();
    virtual ~GfxApi_GL();

    virtual bool init() override;
    virtual bool create_swap_chain(void* hwnd,
        GfxSwapChain** out_swap_chain,
        GfxFrameBuffer** out_frame_buffer) override;

    virtual void destroy_swap_chain(GfxSwapChain* swap_chain) override;
    virtual void destroy_frame_buffer(GfxFrameBuffer* frame_buffer) override;

    virtual StreamingVB* create_streaming_vb() override;
};

// GL Utilities
bool create_shader(GLenum target, const GLchar* source_code, GLuint* out_shader);
bool compile_program(GLuint* out_program, ...);
