#include "pch.h"

#include "gfx_gl.h"
#include "console.h"

// --------------------------------------------------------------------------------------------------------------------
namespace GLRenderer
{
    uint32_t GetApiError()
    {
        #if defined(_DEBUG)
            return glGetError();
        #else
            return GL_NO_ERROR;
        #endif
    }
}

// --------------------------------------------------------------------------------------------------------------------
GfxBaseApi *CreateGfxOpenGLGeneric() { return new GfxApiOpenGLGeneric; }
GfxBaseApi *CreateGfxOpenGLCore() { return new GfxApiOpenGLCore; }

std::tuple<std::string, std::string> versionSplit(const std::string& _srcString);

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
GfxApiOpenGLBase::GfxApiOpenGLBase()
: mGLrc()
{ }

// --------------------------------------------------------------------------------------------------------------------
GfxApiOpenGLBase::~GfxApiOpenGLBase()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool GfxApiOpenGLBase::Init(const std::string& _title, int _x, int _y, int _width, int _height)
{
#if defined(_DEBUG)
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

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

    console::log("GL created successfully! Info follows.");
    console::log("Vendor: %s", glGetString(GL_VENDOR));
    console::log("Renderer: %s", glGetString(GL_RENDERER));
    console::log("Version: %s", glGetString(GL_VERSION));
    console::log("Shading Language Version: %s", glGetString(GL_SHADING_LANGUAGE_VERSION));

    std::list<std::string> stripExts;
    CheckExtensions(stripExts);
    ResolveExtensions();
    
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
    if (glDebugMessageControl != NULL && glDebugMessageCallback != NULL) {
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);
        // glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
        glDebugMessageCallback(ErrorCallback, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
    }
#endif

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLBase::Shutdown()
{
    SDL_GL_MakeCurrent(nullptr, nullptr);

    ResetExtensions();

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
void GfxApiOpenGLBase::Activate()
{
    assert(mWnd);
    SDL_ShowWindow(mWnd);
    SDL_GL_MakeCurrent(mWnd, mGLrc);
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLBase::Deactivate()
{
    assert(mWnd);
    SDL_GL_MakeCurrent(mWnd, nullptr);
    SDL_HideWindow(mWnd);
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLBase::Clear(Vec4 _clearColor, GLfloat _clearDepth)
{
    // TODO: This should go elsewhere.
    glViewport(0, 0, GetWidth(), GetHeight());

    glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
    glClearDepthf(_clearDepth);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GLenum err = GLRenderer::GetApiError();
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLBase::SwapBuffers()
{
    assert(mWnd);

    SDL_GL_SwapWindow(mWnd);
}

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
bool GfxApiOpenGLCore::Init(const std::string& _title, int _x, int _y, int _width, int _height)
{
    // These must be set before we call the base class.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    if (!GfxApiOpenGLBase::Init(_title, _x, _y, _width, _height)) {
        return false;
    }

    // Now that we have something valid, create our VAO and bind it. Ugh! So lame that this is required.
    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiOpenGLCore::Shutdown()
{
    // Must cleanup before we call base class.
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &mVertexArrayObject);

    GfxApiOpenGLBase::Shutdown();
}

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
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

    console::error("Unable to locate file '%s'", _filename.c_str());

    return retBuffer;
}

// --------------------------------------------------------------------------------------------------------------------
GLuint CompileShaderFromFile(GLenum _shaderType, std::string _shaderFilename, std::string _shaderPrefix)
{
    std::string shaderFullPath = "Shaders/glsl/" + _shaderFilename;

    const char* shaderPrefix = _shaderPrefix.c_str();

    GLuint retVal = glCreateShader(_shaderType);
    std::string fileContents = FileContentsToString(shaderFullPath);
    const char* includePath = ".";

    // GLSL has this annoying feature that the #version directive must appear first. But we 
    // want to inject some #define shenanigans into the shader. 
    // So to do that, we need to split for the part of the shader up to the end of the #version line,
    // and everything after that. We can then inject our defines right there.
    auto strTuple = versionSplit(fileContents);
    std::string versionStr = std::get<0>(strTuple);
    std::string shaderContents = std::get<1>(strTuple);

    const char* shaderStrings[] = {
        versionStr.c_str(),
        "\n",
        _shaderPrefix.c_str(),
        "\n",
        shaderContents.c_str()
    };

    glShaderSource(retVal, ArraySize(shaderStrings), shaderStrings, nullptr);
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
        glDeleteShader(retVal);
        retVal = 0;
    }

    return retVal;
}

// --------------------------------------------------------------------------------------------------------------------
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
            console::log("Shader Linking succeeded, with following warnings/messages:\n");
        }

        console::log("%s", buffer);
        delete[] buffer;
    }

    if (linkStatus != GL_TRUE) {
        #ifndef POSIX
            assert(!"Shader failed linking, here's an assert to break you in the debugger.");
        #endif
        glDeleteProgram(retVal);
        retVal = 0;
    }

    return retVal;
}

// --------------------------------------------------------------------------------------------------------------------
GLuint CreateProgram(const std::string& _vsFilename, const std::string& _psFilename)
{
    return CreateProgram(_vsFilename, _psFilename, std::string(""));
}

// --------------------------------------------------------------------------------------------------------------------
GLuint CreateProgram(const std::string& _vsFilename, const std::string& _psFilename, const std::string& _shaderPrefix)
{
    GLuint vs = CompileShaderFromFile(GL_VERTEX_SHADER, _vsFilename, _shaderPrefix),
           fs = CompileShaderFromFile(GL_FRAGMENT_SHADER, _psFilename, _shaderPrefix);

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

// --------------------------------------------------------------------------------------------------------------------
GLuint CreateProgram(const std::string& _vsFilename, const std::string& _psFilename, const char** _uniformNames, GLuint* _outUniformLocations)
{
    return CreateProgram(_vsFilename, _psFilename, std::string(""), _uniformNames, _outUniformLocations);
}

// --------------------------------------------------------------------------------------------------------------------
GLuint CreateProgram(const std::string& _vsFilename, const std::string& _psFilename, const std::string& _shaderPrefix, const char** _uniformNames, GLuint* _outUniformLocations)
{
    GLuint retProg = CreateProgram(_vsFilename, _psFilename, _shaderPrefix);
    if (retProg != 0) {
        for (int i = 0; _uniformNames[i] != nullptr; ++i) {
            _outUniformLocations[i] = glGetUniformLocation(retProg, _uniformNames[i]);
        }
    }

    return retProg;
}

// --------------------------------------------------------------------------------------------------------------------
GLuint NewTex2DFromDetails(const TextureDetails& _texDetails)
{
    GLuint retVal = 0;
    GLuint texs[2] = { 0 };

    glGenTextures(1, &retVal);

    if (retVal == 0) {
        return retVal;
    }

    glBindTexture(GL_TEXTURE_2D, retVal);
    glTexStorage2D(GL_TEXTURE_2D, _texDetails.szMipMapCount, _texDetails.glFormat,
                                  _texDetails.dwWidth, _texDetails.dwHeight);

    size_t offset = 0;
    for (int mip = 0; mip < _texDetails.szMipMapCount; ++mip) {
        glCompressedTexSubImage2D(GL_TEXTURE_2D, mip, 0, 0, _texDetails.MipMapWidth(mip), _texDetails.MipMapHeight(mip), _texDetails.glFormat, _texDetails.pSizes[mip], (char*)_texDetails.pPixels + offset);
        offset += _texDetails.pSizes[mip];
    }

    assert(GLRenderer::GetApiError() == GL_NO_ERROR);

    return retVal;
}

// --------------------------------------------------------------------------------------------------------------------
bool matchVersionLine(const std::string& _srcString, size_t _startPos, size_t _endPos)
{
    size_t checkPos = _startPos;
    assert(_endPos <= _srcString.size());

    // GCC doesn't support regexps yet, so we're doing a hand-coded look for 
    // ^\s*#\s*version\s+\d+\s*$
    // Annoying!

    // ^ was handled by the caller.

    // \s*
    while (checkPos < _endPos && (_srcString[checkPos] == ' ' || _srcString[checkPos] == '\t')) {
        ++checkPos;
    }

    if (checkPos == _endPos) {
        return false;
    }

    // #
    if (_srcString[checkPos] == '#') {
        ++checkPos;        
    } else {
        return false;
    }

    if (checkPos == _endPos) {
        return false;
    }

    // \s*
    while (checkPos < _endPos && (_srcString[checkPos] == ' ' || _srcString[checkPos] == '\t')) {
        ++checkPos;
    }

    if (checkPos == _endPos) {
        return false;
    }

    // version
    const char* kSearchString = "version";
    const size_t kSearchStringLen = strlen(kSearchString);

    if (checkPos + kSearchStringLen >= _endPos) {
        return false;
    }

    if (strncmp(kSearchString, &_srcString[checkPos], kSearchStringLen) == 0) {
        checkPos += kSearchStringLen;
    } else {
        return false;
    }

    // \s+ (as \s\s*)
    if (_srcString[checkPos] == ' ' || _srcString[checkPos] == '\t') {
        ++checkPos;
    } else {
        return false;
    }

    while (checkPos < _endPos && (_srcString[checkPos] == ' ' || _srcString[checkPos] == '\t')) {
        ++checkPos;
    }

    if (checkPos == _endPos) {
        return false;
    }

    // \d+ (as \d\d*)
    if (_srcString[checkPos] >= '0' && _srcString[checkPos] <= '9') {
        ++checkPos;
    } else {
        return false;
    }

    // Check the version number
    while (checkPos < _endPos && (_srcString[checkPos] >= '0' && _srcString[checkPos] <= '9')) {
        ++checkPos;
    }

    while (checkPos < _endPos && (_srcString[checkPos] == ' ' || _srcString[checkPos] == '\t')) {
        ++checkPos;
    }

    while (checkPos < _endPos && (_srcString[checkPos] == '\r' || _srcString[checkPos] == '\n')) {
        ++checkPos;
    }

    // NOTE that if the string terminates here we're successful (unlike above)
    if (checkPos == _endPos) {
        return true;
    }
    
    return false;
}

// --------------------------------------------------------------------------------------------------------------------
std::tuple<std::string, std::string> strsplit(const std::string& _srcString, size_t _splitEndPos)
{
    return std::make_tuple(
        _srcString.substr(0, _splitEndPos),
        _srcString.substr(_splitEndPos, std::string::npos)
    );
}

// --------------------------------------------------------------------------------------------------------------------
std::tuple<std::string, std::string> versionSplit(const std::string& _srcString)
{
    const size_t StringLen = _srcString.size();
    size_t matchEndPos = 0;
    size_t substrStartPos = 0;
    size_t eolPos = 0;
    for (eolPos = substrStartPos; eolPos < StringLen; ++eolPos) {
        if (_srcString[eolPos] != '\n') {
            continue;
        }

        if (matchVersionLine(_srcString, substrStartPos, eolPos + 1)) {
            return strsplit(_srcString, eolPos + 1);
        }

        substrStartPos = eolPos + 1;
    }

    // Could be on the last line (not really--the shader will be invalid--but we'll do it anyways)
    if (matchVersionLine(_srcString, substrStartPos, StringLen)) {
        return strsplit(_srcString, eolPos + 1);
    }

    return std::make_tuple(std::string(""), _srcString);
}

// --------------------------------------------------------------------------------------------------------------------
bool IsOpenGL(EGfxApi _api)
{
    switch (_api)
    {
        case EGfxApi::Direct3D11:
            return false;
        case EGfxApi::OpenGLGeneric:
            return true;
        case EGfxApi::OpenGLCore:
            return true;
        default:
            assert(!"Need to update IsOpenGL with new API type.");
            return false;
    };
}
