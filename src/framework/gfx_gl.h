#pragma once

#include "gfx.h"
#include "wgl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class GfxApiOpenGLGeneric : public GfxBaseApi
{
public:
    GfxApiOpenGLGeneric();
    virtual ~GfxApiOpenGLGeneric(); 

    virtual bool Init(const std::string& _title, int _x, int _y, int _width, int _height) override;
    virtual void Shutdown() override;

    virtual void Activate() override;
    virtual void Deactivate() override;
    virtual void Clear(Vec4 _clearColor, GLfloat _clearDepth) override;
    virtual void SwapBuffers() override;

    virtual EGfxApi GetApiType() const { return EGfxApi::OpenGLGeneric; }

protected:
    SDL_GLContext mGLrc;
};

// GL Utilities
GLuint CreateProgram(std::string _vsFilename, std::string _psFilename);
GLuint CreateProgram(std::string _vsFilename, std::string _tcsFilename, std::string _tesFilename, std::string _fsFilename);
