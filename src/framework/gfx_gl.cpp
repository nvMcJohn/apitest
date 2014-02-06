#include "pch.h"

#include "gfx_gl.h"
#include "console.h"

GfxBaseApi *CreateGfxOpenGLGeneric() { return new GfxApiOpenGLGeneric; }

// --------------------------------------------------------------------------------------------------------------------
static SDL_Window* CreateGLWindow(const char* _title, int _x, int _y, int _width, int _height)
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* retWnd = SDL_CreateWindow(_title, _x, _y, _width, _height, SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN);

    return retWnd;
}

// --------------------------------------------------------------------------------------------------------------------
static void APIENTRY ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    if (source == GL_DEBUG_SOURCE_API && type == GL_DEBUG_TYPE_OTHER && severity == GL_DEBUG_SEVERITY_LOW && strstr(message, "DRAW_INDIRECT_BUFFER") == nullptr) {
        return;
    }
    
    console::debug("%s\n", message);
}

// --------------------------------------------------------------------------------------------------------------------
GfxApiOpenGLGeneric::GfxApiOpenGLGeneric()
: mWnd()
, mGLrc()
{ }

// --------------------------------------------------------------------------------------------------------------------
GfxApiOpenGLGeneric::~GfxApiOpenGLGeneric()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool GfxApiOpenGLGeneric::Init(const std::string& _title, int _x, int _y, int _width, int _height)
{
    if (!GfxBaseApi::Init(_title, _x, _y, _width, _height)) {
        return false;
    }

    mWnd = CreateGLWindow(_title.c_str(), _x, _y, _width, _height);
    if (!mWnd) {
        console::debug("Unable to create SDL Window, which is required for GL.");
        return false;
    }

    mGLrc = SDL_GL_CreateContext(mWnd);
    if (mGLrc == 0) {
        console::debug("Unable to create a GL context, which is required for GL.");
        return false;
    }

    if (SDL_GL_MakeCurrent(mWnd, mGLrc) != 0) {
        console::debug("Unable to MakeCurrent on GL context.");
        return false;
    }
    
    // TODO: Redo how this is done, needs fallbacks, should use 
    wgl::bind_gl();

    SDL_GL_SetSwapInterval(0);

    // Default GL State
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glDisable(GL_SCISSOR_TEST);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(1);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

#if defined(_DEBUG)
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
    // glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(ErrorCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
#endif

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLGeneric::Shutdown()
{
    SDL_GL_MakeCurrent(nullptr, nullptr);

    if (mGLrc) {
        SDL_GL_DeleteContext(mGLrc);
        mGLrc = 0;
    }

    if (mWnd) {
        SDL_DestroyWindow(mWnd);
        mWnd = nullptr;
    }
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLGeneric::Activate()
{
    assert(mWnd);
    SDL_ShowWindow(mWnd);
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLGeneric::Deactivate()
{
    assert(mWnd);
    SDL_HideWindow(mWnd);
}

#if 0
// --------------------------------------------------------------------------------------------------------------------
TestCase* GfxApiOpenGLGeneric::create_test(TestId id)
{
    switch (id)
    {
    case TestId::StreamingVB:                                   return new StreamingVB_GL;
    case TestId::CubesUniform:                                  return new Cubes_GL_Uniform;
    case TestId::CubesDynamicBuffer:                            return new Cubes_GL_DynamicBuffer;
    case TestId::CubesBufferRange:                              return new Cubes_GL_BufferRange;
    case TestId::CubesTexCoord:                                 return new Cubes_GL_TexCoord;
    case TestId::CubesMultiDraw:                                return new Cubes_GL_MultiDraw;
    case TestId::CubesBufferStorage:                            return new Cubes_GL_BufferStorage;
    case TestId::CubesBindless:                                 return new Cubes_GL_Bindless;
    case TestId::CubesBindlessIndirect:                         return new Cubes_GL_BindlessIndirect;
    case TestId::TexturesNoTex:                                 return new Textures_GL_NoTex;
    case TestId::TexturesForward:                               return new Textures_GL_Forward;
    case TestId::TexturesBindless:                              return new Textures_GL_Bindless;
    case TestId::TexturesBindlessMultiDraw:                     return new Textures_GL_Bindless_MultiDraw;
    case TestId::TexturesSparseBindlessTextureArray:            return new Textures_GL_Sparse_Bindless_Texture_Array;
    case TestId::TexturesSparseBindlessTextureArrayMultiDraw:   return new Textures_GL_Sparse_Bindless_Texture_Array_MultiDraw;
    }

    return nullptr;
}
#endif

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLGeneric::SwapBuffers()
{
    assert(mWnd);

    SDL_GL_SwapWindow(mWnd);
}

// --------------------------------------------------------------------------------------------------------------------
bool CreateShader(GLenum target, const char* path, GLuint* out_shader)
{
    std::string pathname = "Shaders/glsl/";
    pathname += path;

    FILE* fp = fopen(pathname.c_str(), "rb");
    if (!fp)
        return false;

    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* source_code = new char[file_size];
    size_t bytes_read = fread(source_code, 1, file_size, fp);
    assert(bytes_read == file_size);

    GLuint shader = glCreateShader(target);
    glShaderSource(shader, 1, &source_code, &file_size);
    glCompileShader(shader);

    GLint compile_status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if (!compile_status)
    {
        GLint len;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
        if (len > 1)
        {
            char* log = new char[len];
            GLsizei chars_written;
            glGetShaderInfoLog(shader, len, &chars_written, log);
            console::debug("%s", log);
            delete[] log;
        }

        glDeleteShader(shader);
        delete[] source_code;
        return false;
    }

    *out_shader = shader;
    delete[] source_code;
    return true;
}

// --------------------------------------------------------------------------------------------------------------------
bool CompileProgram(GLuint* out_program, ...)
{
    GLuint p = glCreateProgram();

    va_list args;
    va_start(args, out_program);

    while (GLuint sh = va_arg(args, GLuint)) {
        glAttachShader(p, sh);
    }

    va_end(args);

    glLinkProgram(p);

    GLint link_status;
    glGetProgramiv(p, GL_LINK_STATUS, &link_status);
    if (!link_status)
    {
        GLint len;
        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);
        if (len > 1)
        {
            char* log = new char[len];
            GLsizei chars_written;
            glGetProgramInfoLog(p, len, &chars_written, log);
            console::debug("%s", log);
            delete[] log;
        }

        glDeleteProgram(p);
        return false;
    }

    *out_program = p;
    return true;
}
