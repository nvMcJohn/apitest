#include "gfx_gl.h"
#include "console.h"
#include <assert.h>
#include <stdio.h>

#include "SDL.h"

#include "cubes_gl_uniform.h"
#include "cubes_gl_dynamic_buffer.h"
#include "cubes_gl_buffer_range.h"
#include "cubes_gl_tex_coord.h"
#include "cubes_gl_multi_draw.h"
#include "cubes_gl_buffer_storage.h"
#include "cubes_gl_bindless.h"
#include "cubes_gl_bindless_indirect.h"
#include "textures_gl_notex.h"
#include "textures_gl_forward.h"
#include "textures_gl_bindless.h"
#include "textures_gl_bindless_multidraw.h"
#include "textures_gl_sparse_bindless_texture_array.h"
#include "textures_gl_sparse_bindless_texture_array_multidraw.h"

#include "streaming_vb_gl.h"

GfxApi *create_gfx_gl() { return new GfxApi_GL; }

static void APIENTRY ErrorCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
    if (source == GL_DEBUG_SOURCE_API && type == GL_DEBUG_TYPE_OTHER && severity == GL_DEBUG_SEVERITY_LOW && strstr(message, "DRAW_INDIRECT_BUFFER") == nullptr) {
        return;
    }
    
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

bool GfxApi_GL::create_swap_chain(void* _wnd,
    GfxSwapChain** out_swap_chain,
    GfxFrameBuffer** out_frame_buffer)
{
    SDL_Window* wnd = (SDL_Window*)_wnd;

    GfxSwapChain* swap_chain = new GfxSwapChain;
    swap_chain->wnd = nullptr;
    swap_chain->glrc = nullptr;

    *out_swap_chain = swap_chain;
    *out_frame_buffer = nullptr;

    swap_chain->wnd = wnd;
    swap_chain->glrc = SDL_GL_CreateContext(wnd);

    SDL_GL_MakeCurrent(swap_chain->wnd, swap_chain->glrc);

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
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    GLenum err = glGetError();
#endif

    return true;
}

void GfxApi_GL::destroy_swap_chain(GfxSwapChain* swap_chain)
{
    if (swap_chain)
    {
        SDL_GL_MakeCurrent(nullptr, nullptr);

        if (swap_chain->glrc) {
            SDL_GL_DeleteContext(swap_chain->glrc);
        }

        delete swap_chain;
    }
}

void GfxApi_GL::destroy_frame_buffer(GfxFrameBuffer* frame_buffer)
{}

TestCase* GfxApi_GL::create_test(TestId id)
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
