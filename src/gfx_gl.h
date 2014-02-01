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
    virtual void SwapBuffers() override;

    virtual EGfxApi GetApiType() const { return EGfxApi::OpenGLGeneric; }

protected:
    SDL_Window* mWnd;
    SDL_GLContext mGLrc;
};

// GL Utilities
bool CreateShader(GLenum _target, const char* _path, GLuint* _outShader);
bool CompileProgram(GLuint* _outProgram, ...);
