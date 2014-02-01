// --------------------------------------------------------------------------------------------------------------------
// Graphics interfaces

#pragma once

#include "mathlib.h"

class GfxBaseApi;

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
    TexturesNoTex,
    TexturesForward,
    TexturesBindless,
    TexturesBindlessMultiDraw,
    TexturesSparseBindlessTextureArray,
    TexturesSparseBindlessTextureArrayMultiDraw
};

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TestCase
{
public:
    virtual ~TestCase() {}

    virtual bool Init() = 0;

    virtual bool Begin(GfxBaseApi* _activeAPI) { return true; }
    virtual void Render() = 0;
    virtual void End() { }
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
    virtual void Draw(VertexPos2* vertices, int count) = 0;
    virtual void Render();
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
    virtual void Draw(Matrix* transforms, int count) = 0;
    virtual void Render();
};

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// Textures test
#define TEXTURES_X 100
#define TEXTURES_Y 100
#define TEXTURES_COUNT (TEXTURES_X * TEXTURES_Y)

// --------------------------------------------------------------------------------------------------------------------
class Textures : public TestCase
{
public:
    virtual void Draw(Matrix* transforms, int count) = 0;
    virtual void Render();
};

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// API Abstraction

enum class EGfxApi
{
    Direct3D11,
    OpenGLGeneric
};

// --------------------------------------------------------------------------------------------------------------------
class GfxBaseApi
{
public:
    GfxBaseApi() : mWidth(1), mHeight(1) { }
    virtual ~GfxBaseApi() { }

    virtual bool Init(const std::string& _title, int _x, int _y, int _width, int _height)
    {
        mWidth = _width;
        mHeight = _height;
        return true;
    }

    virtual void Shutdown() = 0;

    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
    virtual void SwapBuffers() = 0;

    virtual EGfxApi GetApiType() const = 0;

    inline size_t GetWidth() const     { return mWidth; }
    inline size_t GetHeight() const    { return mHeight; }

    inline void OnResize(size_t _newWidth, size_t _newHeight)
    {
        mWidth = _newWidth;
        mHeight = _newHeight;
    }

private:
    size_t mWidth;
    size_t mHeight;
};

GfxBaseApi *CreateGfxOpenGLGeneric();
GfxBaseApi *CreateGfxDirect3D11();
