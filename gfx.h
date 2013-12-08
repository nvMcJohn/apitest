// --------------------------------------------------------------------------------------------------------------------
// Graphics interfaces

#pragma once

struct GfxSwapChain;
struct GfxFrameBuffer;

// --------------------------------------------------------------------------------------------------------------------
// Streaming VB test

// Size of dynamic buffer used (should be large enough for 3 frames of submission)
#define DYN_VB_SIZE 24 * 1024 * 1024

struct Vert
{
    float x, y;
};

class StreamingVB
{
public:
    virtual ~StreamingVB() {}

    virtual bool init() = 0;

    virtual bool begin(void* hwnd, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer) = 0;
    virtual void end(GfxSwapChain* swap_chain) = 0;

    virtual void draw(Vert* vertices, int count) = 0;
};

// --------------------------------------------------------------------------------------------------------------------
// API Abstraction

class GfxApi
{
public:
    virtual ~GfxApi() {}

    virtual bool init() = 0;
    virtual bool create_swap_chain(void* hwnd,
        GfxSwapChain** out_swap_chain,
        GfxFrameBuffer** out_frame_buffer) = 0;

    virtual void destroy_swap_chain(GfxSwapChain* swap_chain) = 0;
    virtual void destroy_frame_buffer(GfxFrameBuffer* frame_buffer) = 0;

    // tests
    virtual StreamingVB* create_streaming_vb() = 0;
};

GfxApi *create_gfx_gl();
GfxApi *create_gfx_dx11();
