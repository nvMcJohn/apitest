#include "gfx_gl.h"
#include "console.h"
#include <assert.h>
#include <stdio.h>

#include "cubes_gl_uniform.h"
#include "cubes_gl_dynamic_buffer.h"
#include "streaming_vb_gl.h"

GfxApi *create_gfx_gl() { return new GfxApi_GL; }

static void APIENTRY ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    console::debug("%s\n", message);
}

GfxApi_GL::GfxApi_GL()
{}

GfxApi_GL::~GfxApi_GL()
{}

bool GfxApi_GL::init()
{
    return true;
}

bool GfxApi_GL::create_swap_chain(void* window,
    GfxSwapChain** out_swap_chain,
    GfxFrameBuffer** out_frame_buffer)
{
    HWND hwnd = (HWND)window;

    GfxSwapChain* swap_chain = new GfxSwapChain;
    swap_chain->hdc = nullptr;
    swap_chain->hwnd = nullptr;
    swap_chain->hglrc = nullptr;

    *out_swap_chain = swap_chain;
    *out_frame_buffer = nullptr;

    HDC hdc = GetDC(hwnd);

    BYTE colorBits = 24;
    BYTE alphaBits = 8;
    BYTE depthBits = 24;
    BYTE stencilBits = 8;
    UINT multisample = 1;

    // Choose pixel format
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = colorBits;
    pfd.cAlphaBits = alphaBits;
    pfd.cAccumBits = 0;
    pfd.cDepthBits = depthBits;
    pfd.cStencilBits = stencilBits;
    pfd.cAuxBuffers = 0;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    BOOL result = SetPixelFormat(hdc, pixelFormat, &pfd);
    if (!result)
    {
        ReleaseDC(hwnd, hdc);
        return false;
    }

    swap_chain->hwnd = hwnd;
    swap_chain->hdc = hdc;

    // Create context
    swap_chain->hglrc = wglCreateContext(hdc);

    // Make the context and surface current
    if (!wglMakeCurrent(hdc, swap_chain->hglrc))
        return false;

    wgl::bind_wgl();
    wgl::bind_gl();

    if (wglSwapIntervalEXT)
        wglSwapIntervalEXT(0);

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
    glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE);
    glDebugMessageCallback(ErrorCallback, nullptr);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

    return true;
}

void GfxApi_GL::destroy_swap_chain(GfxSwapChain* swap_chain)
{
    if (swap_chain)
    {
        wglMakeCurrent(nullptr, nullptr);

        if (swap_chain->hglrc)
            wglDeleteContext(swap_chain->hglrc);

        if (swap_chain->hdc)
            ReleaseDC(swap_chain->hwnd, swap_chain->hdc);

        delete swap_chain;
    }
}

void GfxApi_GL::destroy_frame_buffer(GfxFrameBuffer* frame_buffer)
{}

TestCase* GfxApi_GL::create_test(TestId id)
{
    switch (id)
    {
    case TestId::StreamingVB:           return new StreamingVB_GL;
    case TestId::CubesUniform:          return new Cubes_GL_Uniform;
    case TestId::CubesDynamicBuffer:    return new Cubes_GL_DynamicBuffer;
    }

    return nullptr;
}

bool create_shader(GLenum target, const char* path, GLuint* out_shader)
{
    FILE* fp = fopen(path, "rb");
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

bool compile_program(GLuint* out_program, ...)
{
    GLuint p = glCreateProgram();

    va_list args;
    va_start(args, out_program);

    while (GLuint sh = va_arg(args, GLuint))
        glAttachShader(p, sh);

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
