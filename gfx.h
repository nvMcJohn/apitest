// Graphics API

#pragma once

// Choose API
//#define USE_DX11
#define USE_GL4

// API Backend options
#define GL_USE_MAP 1

// Size of dynamic buffer used (should be large enough for 3 frames of submission)
#define DYN_VB_SIZE 24 * 1024 * 1024

struct GfxSwapChain;
struct GfxFrameBuffer;

struct Vert
{
    float x, y;
};

namespace gfx
{
    bool init_device();
    void shutdown_device();

    bool init_objects();
    void destroy_objects();

    bool create_swap_chain(void* hwnd,
        GfxSwapChain** out_swap_chain,
        GfxFrameBuffer** out_frame_buffer);

    void destroy_swap_chain(GfxSwapChain* swap_chain);
    void destroy_frame_buffer(GfxFrameBuffer* frame_buffer);

    void set_frame_buffer(GfxFrameBuffer* frame_buffer);
    void clear_color(const float c[4]);
    void clear_depth(float d);

    bool begin(void* hwnd, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer);
    void end(GfxSwapChain* swap_chain);

    void draw(Vert* vertices, int count);
}
