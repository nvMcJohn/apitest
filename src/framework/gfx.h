// --------------------------------------------------------------------------------------------------------------------
// Graphics interfaces

#pragma once

#include "mathlib.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// API Abstraction

enum class EGfxApi
{
    Direct3D11,
    OpenGLGeneric
};

bool IsOpenGL(EGfxApi _api);

// --------------------------------------------------------------------------------------------------------------------
class GfxBaseApi
{
public:
    GfxBaseApi() : mWidth(1), mHeight(1) { }
    virtual ~GfxBaseApi() { }

    virtual bool Init(const std::string& _title, int _x, int _y, int _width, int _height)
    {
        mTitleRoot = _title;
        mWidth = _width;
        mHeight = _height;
        return true;
    }

    virtual void Shutdown() = 0;

    virtual void Activate() = 0;
    virtual void Deactivate() = 0;
    virtual void Clear(Vec4 _clearColor, GLfloat _clearDepth) = 0;
    virtual void SwapBuffers() = 0;

    virtual EGfxApi GetApiType() const = 0;

    virtual const char* GetShortName() const = 0;
    virtual const char* GetLongName() const = 0;

    inline size_t GetWidth() const     { return mWidth; }
    inline size_t GetHeight() const    { return mHeight; }

    inline void OnResize(size_t _newWidth, size_t _newHeight)
    {
        mWidth = _newWidth;
        mHeight = _newHeight;
    }

    inline void OnProblemOrSolutionSet(const std::string& _problemName, const std::string& _solutionName)
    {
        if (mWnd == nullptr) {
            return;
        }

        std::string newTitle = mTitleRoot + " - " + _problemName;

        if (_solutionName.length()) {
            newTitle += " - " + _solutionName;
        }

        SDL_SetWindowTitle(mWnd, newTitle.c_str());
    }

    inline void MoveWindow(int _x, int _y)
    {
        SDL_SetWindowPosition(mWnd, _x, _y);
    }

protected:
    SDL_Window* mWnd;

private:

    std::string mTitleRoot;
    size_t mWidth;
    size_t mHeight;
};

GfxBaseApi *CreateGfxOpenGLGeneric();
GfxBaseApi *CreateGfxDirect3D11();
