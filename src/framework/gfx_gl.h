#pragma once

#include "gfx.h"
#include "GL/glextensions.h"

namespace GLRenderer
{
    uint32_t GetApiError();
}

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class GfxApiOpenGLBase : public GfxBaseApi
{
public:
    GfxApiOpenGLBase();
    virtual ~GfxApiOpenGLBase(); 

    virtual bool Init(const std::string& _title, int _x, int _y, int _width, int _height) override;
    virtual void Shutdown() override;

    virtual void Activate() override;
    virtual void Deactivate() override;
    virtual void Clear(Vec4 _clearColor, GLfloat _clearDepth) override;
    virtual void SwapBuffers() override;

    virtual EGfxApi GetApiType() const = 0;

    inline virtual const char* GetShortName() const = 0;
    inline virtual const char* GetLongName() const = 0;

protected:
    SDL_GLContext mGLrc;
};

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// This implementation is just whatever we get from the platform by default, without asking for anything specific
// ie, we're not asking for Core or Compatibility. See the log output to determine what type was actually created.
class GfxApiOpenGLGeneric : public GfxApiOpenGLBase
{
public:
    GfxApiOpenGLGeneric() { }
    virtual ~GfxApiOpenGLGeneric() override { }

    virtual EGfxApi GetApiType() const override { return EGfxApi::OpenGLGeneric; }

    inline virtual const char* GetShortName() const override { return SGetShortName(); }
    inline virtual const char* GetLongName() const override { return SGetLongName(); }

    static const char* SGetShortName() { return "oglgeneric"; }
    static const char* SGetLongName() { return "OpenGL (Generic)"; }
};

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class GfxApiOpenGLCore : public GfxApiOpenGLBase
// For this implementation, we ask explicitly for a Core Context. 
{
public:
    GfxApiOpenGLCore() : mVertexArrayObject(0) { }
    virtual ~GfxApiOpenGLCore() override { }

    virtual bool Init(const std::string& _title, int _x, int _y, int _width, int _height) override;
    virtual void Shutdown() override;

    virtual EGfxApi GetApiType() const override { return EGfxApi::OpenGLCore; }

    inline virtual const char* GetShortName() const override { return SGetShortName(); }
    inline virtual const char* GetLongName() const override { return SGetLongName(); }

    static const char* SGetShortName() { return "oglcore"; }
    static const char* SGetLongName() { return "OpenGL (Core)"; }

protected:
    GLuint mVertexArrayObject;
};


// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// GL Utilities
GLuint CreateProgram(const std::string& _vsFilename, const std::string& _psFilename);
GLuint CreateProgram(const std::string& _vsFilename, const std::string& _psFilename, const std::string& _shaderPrefix);
GLuint CreateProgram(const std::string& _vsFilename, const std::string& _psFilename, const char** _uniformNames, GLuint* _outUniformLocations);
GLuint CreateProgram(const std::string& _vsFilename, const std::string& _psFilename, const std::string& _shaderPrefix, const char** _uniformNames, GLuint* _outUniformLocations);

template <typename T> 
GLuint CreateProgramT(const std::string& _vsFilename, const std::string& _psFilename, const char** _uniforms, T* _outUniformLocationStruct)
{
    return CreateProgramT(_vsFilename, _psFilename, std::string(""), _uniforms, _outUniformLocationStruct);
}

template <typename T>
GLuint CreateProgramT(const std::string& _vsFilename, const std::string& _psFilename, const std::string& _shaderPrefix, const char** _uniforms, T* _outUniformLocationStruct)
{
    assert(_uniforms != nullptr);
    assert(_outUniformLocationStruct != nullptr);

    int uniformCount = 0;
    while (_uniforms[uniformCount] != nullptr) {
        ++uniformCount;
    }

    // Ensure that the sizes match, otherwise there is a parameter mismatch.
    assert(uniformCount == (sizeof(T) / sizeof(GLuint))
           && (sizeof(T) % sizeof(GLuint) == 0));

    return CreateProgram(_vsFilename, _psFilename, _shaderPrefix, _uniforms, reinterpret_cast<GLuint*>(_outUniformLocationStruct));
}

// --------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void BufferData(GLenum _target, const std::vector<T>& _data, GLenum _usage)
{
    if (_data.size() > 0) {
        glBufferData(_target, sizeof(T) * _data.size(), &*_data.cbegin(), _usage);
    }
    else {
        glBufferData(_target, 0, nullptr, _usage);
    }
}

// --------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void BufferDataArray(GLenum _target, const T* _data, GLint _count, GLenum _usage)
{
    if (_count > 0) {
        glBufferData(_target, sizeof(T) * _count, _data, _usage);
    }
    else {
        glBufferData(_target, 0, nullptr, _usage);
    }
}

// --------------------------------------------------------------------------------------------------------------------
template <typename T>
inline GLuint NewBufferFromVector(GLenum _target, const std::vector<T>& _data, GLenum _usage)
{
    GLuint retVal = 0;
    glGenBuffers(1, &retVal);
    if (retVal == 0) {
        return 0;
    }

    glBindBuffer(_target, retVal);
    BufferData(_target, _data, _usage);

    assert(GLRenderer::GetApiError() == GL_NO_ERROR);

    return retVal;
}

GLuint NewTex2DFromDetails(const TextureDetails& _texDetails);