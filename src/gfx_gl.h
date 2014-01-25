#pragma once

#include "gfx.h"

#include "wgl.h"

#define PTR_AS_INT(x)           static_cast<int>(reinterpret_cast<intptr_t>(x))

#define GL_USE_DIRECT_STATE_ACCESS 1

struct GfxSwapChain
{
    SDL_Window* wnd;
    SDL_GLContext glrc;
};

class GfxApi_GL : public GfxApi
{
public:
    GfxApi_GL();
    virtual ~GfxApi_GL();

    virtual bool init() override;
    virtual bool create_swap_chain(void* wnd,
        GfxSwapChain** out_swap_chain,
        GfxFrameBuffer** out_frame_buffer) override;

    virtual void destroy_swap_chain(GfxSwapChain* swap_chain) override;
    virtual void destroy_frame_buffer(GfxFrameBuffer* frame_buffer) override;

    virtual TestCase* create_test(TestId id) override;
};

// GL Utilities
bool create_shader(GLenum target, const char* path, GLuint* out_shader);
bool compile_program(GLuint* out_program, ...);
