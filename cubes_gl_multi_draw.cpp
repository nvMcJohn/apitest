#include "cubes_gl_multi_draw.h"
#include "mathlib.h"
#include <assert.h>
#include <stdint.h>

#define USE_BUFFER_STORAGE 1

Cubes_GL_MultiDraw::Cubes_GL_MultiDraw()
    : m_ib()
    , m_vb()
    , m_tb()
    , m_vs()
    , m_fs()
    , m_prog()
    , m_tb_ptr()
{}

Cubes_GL_MultiDraw::~Cubes_GL_MultiDraw()
{
    if (m_tb_ptr)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_tb);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }

    glDeleteBuffers(1, &m_ib);
    glDeleteBuffers(1, &m_vb);
    glDeleteBuffers(1, &m_tb);
    glDeleteShader(m_vs);
    glDeleteShader(m_fs);
    glDeleteProgram(m_prog);
}

bool Cubes_GL_MultiDraw::init()
{
    // Shaders
    if (!create_shader(GL_VERTEX_SHADER, "cubes_gl_multi_draw_vs.glsl", &m_vs))
        return false;

    if (!create_shader(GL_FRAGMENT_SHADER, "cubes_gl_multi_draw_fs.glsl", &m_fs))
        return false;

    if (!compile_program(&m_prog, m_vs, m_fs, 0))
        return false;

    // Buffers
    Vertex vertices[] =
    {
        { -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f },
        {  0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f },
        {  0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f },
        { -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 1.0f },
        { -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f },
        {  0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f },
        {  0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f },
        { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f },
    };

    uint16_t indices[] =
    {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        3, 2, 5, 3, 5, 4,
        2, 1, 6, 2, 6, 5,
        1, 7, 6, 1, 0, 7,
        0, 3, 4, 0, 4, 7
    };

    glGenBuffers(1, &m_vb);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_tb);
#if USE_BUFFER_STORAGE
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_tb);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, 64 * 64 * 64 * 64, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT);
    m_tb_ptr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, 64 * 64 * 64 * 64, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
#endif

    return glGetError() == GL_NO_ERROR;
}

bool Cubes_GL_MultiDraw::begin(void* window, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer)
{
    HWND hWnd = reinterpret_cast<HWND>(window);

    RECT rc;
    GetClientRect(hWnd, &rc);
    int width = rc.right - rc.left;
    int height = rc.bottom - rc.top;

    // Bind and clear frame buffer
    int fbo = PTR_AS_INT(frame_buffer);
    float c[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float d = 1.0f;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glClearBufferfv(GL_COLOR, 0, c);
    glClearBufferfv(GL_DEPTH, 0, &d);

    // Program
    Vec3 eye = { -50, -120, 100 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 0, 1 };
    Matrix view = matrix_look_at(eye, at, up);
    Matrix proj = matrix_perspective_rh_gl(radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);
    Matrix view_proj = proj * view;

    glUseProgram(m_prog);
    glUniformMatrix4fv(0, 1, GL_TRUE, &view_proj.x.x);

    // Input Layout
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Rasterizer State
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDisable(GL_SCISSOR_TEST);
    glViewport(0, 0, width, height);

    // Blend State
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Depth Stencil State
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    return true;
}

void Cubes_GL_MultiDraw::end(GfxSwapChain* swap_chain)
{
    SwapBuffers(swap_chain->hdc);
#if defined(_DEBUG)
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
#endif
}

void Cubes_GL_MultiDraw::draw(Matrix* transforms, int count)
{
#if USE_BUFFER_STORAGE
    memcpy(m_tb_ptr, transforms, sizeof(Matrix) * count);
    glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);
#else
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_tb);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(Matrix) * count, transforms, GL_DYNAMIC_DRAW);
#endif

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_tb);

    m_commands.resize(count);

    for (int i = 0; i < count; ++i)
    {
        DrawElementsIndirectCommand *cmd = &m_commands[i];
        cmd->count = 36;
        cmd->instanceCount = 1;
        cmd->firstIndex = 0;
        cmd->baseVertex = 0;
        cmd->baseInstance = 0;
    }

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, m_commands.data(), (GLsizei)m_commands.size(), 0);
}
