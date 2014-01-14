// --------------------------------------------------------------------------------------------------------------------
// Graphics interfaces

#pragma once

#include "mathlib.h"

struct GfxSwapChain;
struct GfxFrameBuffer;

// --------------------------------------------------------------------------------------------------------------------
// Test Case

enum class TestId
{
    StreamingVB,
    CubesUniform,
    CubesDynamicBuffer,
    CubesBufferRange,
    CubesTexCoord,
    CubesMultiDraw,
    CubesBufferStorage,
    CubesBindless,
    CubesBindlessIndirect,
    TexturesForward,
    TexturesBindless,
    TexturesSparseBindlessTextureArray
};

class TestCase
{
public:
    virtual ~TestCase() {}

    virtual bool init() = 0;

    virtual bool begin(void* hwnd, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer) = 0;
    virtual void render() = 0;
    virtual void end(GfxSwapChain* swap_chain) = 0;
};

// --------------------------------------------------------------------------------------------------------------------
// Streaming VB test

// Size of dynamic buffer used (should be large enough for 3 frames of submission)
#define DYN_VB_SIZE 24 * 1024 * 1024

struct VertexPos2
{
    float x, y;
};

class StreamingVB : public TestCase
{
public:
    virtual void draw(VertexPos2* vertices, int count) = 0;
    virtual void render();
};

// --------------------------------------------------------------------------------------------------------------------
// Cubes test

#define CUBES_X 64
#define CUBES_Y 64
#define CUBES_Z 64
#define CUBES_COUNT (CUBES_X * CUBES_Y * CUBES_Z)

class Cubes : public TestCase
{
public:
    virtual void draw(Matrix* transforms, int count) = 0;
    virtual void render();
};

// --------------------------------------------------------------------------------------------------------------------
// Textures test
#define TEXTURES_X 100
#define TEXTURES_Y 100
#define TEXTURES_COUNT (TEXTURES_X * TEXTURES_Y)

class Textures : public TestCase
{
public:
    virtual void draw(Matrix* transforms, int count) = 0;
    virtual void render();
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

    virtual TestCase* create_test(TestId id) = 0;
};

GfxApi *create_gfx_gl();
GfxApi *create_gfx_dx11();
