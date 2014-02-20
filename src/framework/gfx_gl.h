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

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// GL Utilities
GLuint CreateProgram(std::string _vsFilename, std::string _psFilename);
GLuint CreateProgram(std::string _vsFilename, std::string _psFilename, const char** _uniformNames, GLuint* _outUniformLocations);

template <typename T> 
GLuint CreateProgramT(std::string _vsFilename, std::string _psFilename, const char** _uniforms, T* _outUniformLocationStruct)
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

    return CreateProgram(_vsFilename, _psFilename, _uniforms, reinterpret_cast<GLuint*>(_outUniformLocationStruct));
}

// --------------------------------------------------------------------------------------------------------------------
template <typename T>
inline void BufferData(GLenum _target, const std::vector<T>& _data, GLenum _usage)
{
    if (_data.size() > 0) {
        glBufferData(_target, sizeof(T)* _data.size(), &*_data.cbegin(), _usage);
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

    assert(glGetError() == GL_NO_ERROR);

    return retVal;
}

GLuint NewTex2DFromDetails(const TextureDetails& _texDetails);
