#include "gfx.h"
#include "console.h"
#include <assert.h>

#if defined(USE_GL4)

#define PTR_AS_INT(x)           static_cast<int>(reinterpret_cast<intptr_t>(x))

#include "wgl.h"

#include "gl4_vs_code.h"
#include "gl4_fs_code.h"

struct GfxSwapChain
{
    HDC hdc;
    HWND hwnd;
    HGLRC hglrc;
};

struct UIConstants
{
    float width;
    float height;
    float pad[2];
};

namespace gfx
{
    static bool create_shader(GLenum target, const GLchar* source_code, GLuint* out_shader);
    static bool compile_program(GLuint* out_program, ...);

    static int s_window_width;
    static int s_window_height;
    static int s_attrib_enables;

    static GLuint s_ub;
    static GLuint s_vs;
    static GLuint s_fs;
    static GLuint s_prog;

    static GLuint s_dyn_vb;
    static int s_dyn_offset;
}

static bool gfx::create_shader(GLenum target, const GLchar* source_code, GLuint* out_shader)
{
    GLuint shader = glCreateShader(target);
    glShaderSource(shader, 1, &source_code, nullptr);
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
        return false;
    }

    *out_shader = shader;
    return true;
}

static bool gfx::compile_program(GLuint* out_program, ...)
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

bool gfx::init_device()
{
    return true;
}

bool gfx::init_objects()
{
    // Uniform Buffer
    glGenBuffers(1, &s_ub);

    // Shaders
    if (!create_shader(GL_VERTEX_SHADER, s_vs_code, &s_vs))
        return false;

    if (!create_shader(GL_FRAGMENT_SHADER, s_fs_code, &s_fs))
        return false;

    GLuint p;
    if (!compile_program(&p, s_vs, s_fs, 0))
        return false;
    s_prog = p;

    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);

    // Dynamic vertex buffer
    glGenBuffers(1, &s_dyn_vb);
    glBindBuffer(GL_ARRAY_BUFFER, s_dyn_vb);
    glBufferData(GL_ARRAY_BUFFER, DYN_VB_SIZE, nullptr, GL_DYNAMIC_DRAW);

    return true;
}

void gfx::destroy_objects()
{
    glDeleteBuffers(1, &s_dyn_vb);

    glDeleteBuffers(1, &s_ub);
    glDeleteShader(s_vs);
    glDeleteShader(s_fs);
    glDeleteProgram(s_prog);
}

void gfx::shutdown_device()
{
}

bool gfx::create_swap_chain(void* window, 
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

    return true;
}

void gfx::destroy_swap_chain(GfxSwapChain* swap_chain)
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

void gfx::destroy_frame_buffer(GfxFrameBuffer* frame_buffer)
{
}

void gfx::set_frame_buffer(GfxFrameBuffer* frame_buffer)
{
    int fbo = PTR_AS_INT(frame_buffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
}

void gfx::clear_color(const float c[4])
{
    glClearBufferfv(GL_COLOR, 0, c);
}

void gfx::clear_depth(float d)
{
    glClearBufferfv(GL_DEPTH, 0, &d);
}

bool gfx::begin(void* window, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer)
{
    HWND hWnd = reinterpret_cast<HWND>(window);

    RECT rc;
    GetClientRect(hWnd, &rc);
    s_window_width = rc.right - rc.left;
    s_window_height = rc.bottom - rc.top;

    // Program
    glUseProgram(s_prog);

    // Uniforms
    UIConstants cb;
    cb.width = 2.0f / s_window_width;
    cb.height = -2.0f / s_window_height;

    glBindBuffer(GL_UNIFORM_BUFFER, s_ub);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(UIConstants), &cb, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, s_ub);

    // Input Layout
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, s_dyn_vb);
    glVertexAttribPointer(0, 2, GL_FLOAT, FALSE, sizeof(Vert), (void*)offsetof(Vert, x));

    // Rasterizer State
    glDisable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, s_window_width, s_window_height);

    // Blend State
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Depth Stencil State
    glDisable(GL_DEPTH_TEST);
    glDepthMask(0);

    return true;
}

void gfx::end(GfxSwapChain* swap_chain)
{
    SwapBuffers(swap_chain->hdc);
#if defined(_DEBUG)
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
#endif
}

void gfx::draw(Vert* vertices, int count)
{
#if GL_USE_MAP
    int stride = sizeof(Vert);
    int vertex_offset = (s_dyn_offset + stride - 1) / stride;
    int byte_offset = vertex_offset * stride;
    int size = count * stride;

    GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT | GL_MAP_UNSYNCHRONIZED_BIT;
    if (byte_offset + size > DYN_VB_SIZE)
    {
        access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT;
        vertex_offset = byte_offset = 0;
    }

    s_dyn_offset = byte_offset + size;
    void* dst = glMapBufferRange(GL_ARRAY_BUFFER, byte_offset, size, access);
    if (!dst)
        return;

    memcpy(dst, vertices, count * sizeof(Vert));

    glUnmapBuffer(GL_ARRAY_BUFFER);
#else
    int vertex_offset = (s_dyn_offset + sizeof(Vert) - 1) / sizeof(Vert);
    int byte_offset = vertex_offset * sizeof(Vert);
    int size = count * sizeof(Vert);

    if (byte_offset + size > DYN_VB_SIZE)
    {
        byte_offset = 0;
        vertex_offset = 0;
    }

    glBufferSubData(GL_ARRAY_BUFFER, byte_offset, size, vertices);
    s_dyn_offset = byte_offset + size;
#endif

    glDrawArrays(GL_TRIANGLES, vertex_offset, count);
}

#endif
