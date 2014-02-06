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
