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
    
    console::debug("%s", message);
}

// --------------------------------------------------------------------------------------------------------------------
GfxApiOpenGLGeneric::GfxApiOpenGLGeneric()
: mGLrc()
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
        console::warn("Unable to create SDL Window, which is required for GL.");
        return false;
    }

    mGLrc = SDL_GL_CreateContext(mWnd);
    if (mGLrc == 0) {
        console::warn("Unable to create a GL context, which is required for GL.");
        return false;
    }

    if (SDL_GL_MakeCurrent(mWnd, mGLrc) != 0) {
        console::warn("Unable to MakeCurrent on GL context.");
        return false;
    }
    
    // TODO: Redo how this is done, needs fallbacks, should use macro file.
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
void GfxApiOpenGLGeneric::Clear(Vec4 _clearColor, GLfloat _clearDepth)
{
    // TODO: This should go elsewhere.
    glViewport(0, 0, GetWidth(), GetHeight());

    static_assert(sizeof(Vec4) == 4 * sizeof(GLfloat), "Unexpected size difference. Waaaat");
    glClearBufferfv(GL_COLOR, 0, &_clearColor.x);
    glClearBufferfv(GL_DEPTH, 0, &_clearDepth);
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLGeneric::SwapBuffers()
{
    assert(mWnd);

    SDL_GL_SwapWindow(mWnd);
}

// ------------------------------------------------------------------------------------------------
std::string FileContentsToString(std::string _filename)
{
    std::string retBuffer;

    FILE* file = 0;
    fopen_s(&file, _filename.c_str(), "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        size_t bytes = ftell(file);

        retBuffer.resize(bytes + 1);

        fseek(file, 0, SEEK_SET);
        fread(&(*retBuffer.begin()), 1, bytes, file);
        fclose(file);
        file = 0;
        return retBuffer;
    }

    console::error("Unable to locate file '%s'", _filename);

    return retBuffer;
}

// ------------------------------------------------------------------------------------------------
GLuint CompileShaderFromFile(GLenum _shaderType, std::string _shaderFilename)
{
    std::string shaderFullPath = "Shaders/glsl/" + _shaderFilename;

    GLuint retVal = glCreateShader(_shaderType);
    std::string fileContents = FileContentsToString(shaderFullPath);
    const char* fileContentsCstr = fileContents.c_str();
    const char* includePath = ".";

    glShaderSource(retVal, 1, &fileContentsCstr, NULL);
    glCompileShader(retVal);

    GLint compileStatus = 0;
    glGetShaderiv(retVal, GL_COMPILE_STATUS, &compileStatus);

    GLint glinfoLogLength = 0;
    glGetShaderiv(retVal, GL_INFO_LOG_LENGTH, &glinfoLogLength);
    if (glinfoLogLength > 1) {
        GLchar* buffer = new GLchar[glinfoLogLength];
        glGetShaderInfoLog(retVal, glinfoLogLength, &glinfoLogLength, buffer);
        if (compileStatus != GL_TRUE) {
            console::warn("Shader Compilation failed for shader '%s', with the following errors:", _shaderFilename.c_str());
        } else {
            console::log("Shader Compilation succeeded for shader '%s', with the following log:", _shaderFilename.c_str());
        }

        console::log("%s", buffer);
        delete[] buffer;
    }

    if (compileStatus != GL_TRUE) {
        assert(!"Shader failed compilation, here's an assert to break you in the debugger.");
        glDeleteShader(retVal);
        retVal = 0;
    }

    return retVal;
}

// ------------------------------------------------------------------------------------------------
GLuint LinkShaders(GLuint _vs, GLuint _fs)
{
    GLuint retVal = glCreateProgram();
    glAttachShader(retVal, _vs);
    glAttachShader(retVal, _fs);
    glLinkProgram(retVal);

    GLint linkStatus = 0;
    glGetProgramiv(retVal, GL_LINK_STATUS, &linkStatus);

    GLint glinfoLogLength = 0;
    glGetProgramiv(retVal, GL_INFO_LOG_LENGTH, &glinfoLogLength);

    if (glinfoLogLength > 1) {
        GLchar* buffer = new GLchar[glinfoLogLength];
        glGetProgramInfoLog(retVal, glinfoLogLength, &glinfoLogLength, buffer);
        if (linkStatus != GL_TRUE) {
            console::warn("Shader Linking failed with the following errors:");
        }
        else {
            console::log("Shader Linking succeeded, with following warnings/messages:");
        }
        delete[] buffer;
    }

    if (linkStatus != GL_TRUE) {
        assert(!"Shader failed linking, here's an assert to break you in the debugger.");
        glDeleteProgram(retVal);
        retVal = 0;
    }

    return retVal;
}

// ------------------------------------------------------------------------------------------------
GLuint LinkShaders(GLuint _vs, GLuint _tcs, GLuint _tes, GLuint _fs)
{
    GLuint retVal = glCreateProgram();
    glAttachShader(retVal, _vs);
    glAttachShader(retVal, _tcs);
    glAttachShader(retVal, _tes);
    glAttachShader(retVal, _fs);
    glLinkProgram(retVal);

    GLint linkStatus = 0;
    glGetProgramiv(retVal, GL_LINK_STATUS, &linkStatus);

    GLint glinfoLogLength = 0;
    glGetProgramiv(retVal, GL_INFO_LOG_LENGTH, &glinfoLogLength);
    if (glinfoLogLength > 1) {
        GLchar* buffer = new GLchar[glinfoLogLength];
        glGetProgramInfoLog(retVal, glinfoLogLength, &glinfoLogLength, buffer);
        if (linkStatus != GL_TRUE) {
            console::warn("Shader Linking failed with the following errors:");
        }
        else {
            console::log("Shader Linking succeeded, with following warnings/messages:");
        }
        delete[] buffer;
    }

    if (linkStatus != GL_TRUE) {
        assert(!"Shader failed linking, here's an assert to break you in the debugger.");

        glDeleteProgram(retVal);
        retVal = 0;
    }

    return retVal;
}

// ------------------------------------------------------------------------------------------------
GLuint CreateProgram(std::string _vsFilename, std::string _psFilename)
{
    GLuint vs = CompileShaderFromFile(GL_VERTEX_SHADER, _vsFilename),
           fs = CompileShaderFromFile(GL_FRAGMENT_SHADER, _psFilename);

    // If any are 0, dump out early.
    if ((vs * fs) == 0) {
        return 0;
    }

    GLuint retProgram = LinkShaders(vs, fs);

    // Flag these now, they're either attached (linked in) and will be cleaned up with the link, or the
    // link failed and we're about to lose track of them anyways.
    glDeleteShader(fs);
    glDeleteShader(vs);

    return retProgram;
}

// ------------------------------------------------------------------------------------------------
GLuint CreateProgram(std::string _vsFilename, std::string _tcsFilename, std::string _tesFilename, std::string _fsFilename)
{
    GLuint vs = CompileShaderFromFile(GL_VERTEX_SHADER, _vsFilename),
           tcs = CompileShaderFromFile(GL_TESS_CONTROL_SHADER, _tcsFilename),
           tes = CompileShaderFromFile(GL_TESS_EVALUATION_SHADER, _tesFilename),
           fs = CompileShaderFromFile(GL_FRAGMENT_SHADER, _fsFilename);

    // If any are 0, dump out early.
    if ((vs * tcs * tes * fs) == 0) {
        return 0;
    }

    GLuint retProgram = LinkShaders(vs, tcs, tes, fs);

    // Flag these now, they're either attached (linked in) and will be cleaned up with the link, or the
    // link failed and we're about to lose track of them anyways.
    glDeleteShader(fs);
    glDeleteShader(tes);
    glDeleteShader(tcs);
    glDeleteShader(vs);

    return retProgram;
}
