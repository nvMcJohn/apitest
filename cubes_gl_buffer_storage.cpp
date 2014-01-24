#include "cubes_gl_buffer_storage.h"
#include "mathlib.h"
#include <assert.h>
#include <stdint.h>

Cubes_GL_BufferStorage::Cubes_GL_BufferStorage()
    : m_ib()
    , m_vb()
    , m_vs()
    , m_fs()
    , m_prog()
    , m_transform_buffer()
    , m_transform_ptr()
    , m_cmd_buffer()
    , m_cmd_ptr()
{}

Cubes_GL_BufferStorage::~Cubes_GL_BufferStorage()
{
    if (m_transform_ptr)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_transform_buffer);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }

    if (m_cmd_ptr)
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_cmd_buffer);
        glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
    }

    glDeleteBuffers(1, &m_ib);
    glDeleteBuffers(1, &m_vb);
    glDeleteBuffers(1, &m_transform_buffer);
    glDeleteBuffers(1, &m_cmd_buffer);
    glDeleteShader(m_vs);
    glDeleteShader(m_fs);
    glDeleteProgram(m_prog);
}

bool Cubes_GL_BufferStorage::init()
{
    // Shaders
    if (!create_shader(GL_VERTEX_SHADER, "cubes_gl_buffer_storage_vs.glsl", &m_vs))
        return false;

    if (!create_shader(GL_FRAGMENT_SHADER, "cubes_gl_buffer_storage_fs.glsl", &m_fs))
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

    glGenBuffers(1, &m_transform_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_transform_buffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, CUBES_COUNT * 64, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT);
    m_transform_ptr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, CUBES_COUNT * 64, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

    glGenBuffers(1, &m_cmd_buffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_cmd_buffer);
    glBufferStorage(GL_DRAW_INDIRECT_BUFFER, CUBES_COUNT * sizeof(DrawElementsIndirectCommand), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT);
    m_cmd_ptr = glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, CUBES_COUNT * sizeof(DrawElementsIndirectCommand), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

    return glGetError() == GL_NO_ERROR;
}

bool Cubes_GL_BufferStorage::begin(GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer)
{
    int width = 0;
    int height = 0;
    SDL_GetWindowSize(swap_chain->wnd, &width, &height);

    // Bind and clear frame buffer
    int fbo = PTR_AS_INT(frame_buffer);
    float c[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float d = 1.0f;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
    glClearBufferfv(GL_COLOR, 0, c);
    glClearBufferfv(GL_DEPTH, 0, &d);

    // Program
    Vec3 dir = { -0.5f, -1, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 0, 1 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
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

void Cubes_GL_BufferStorage::end(GfxSwapChain* swap_chain)
{
    SDL_GL_SwapWindow(swap_chain->wnd);
#if defined(_DEBUG)
    GLenum error = glGetError();
    assert(error == GL_NO_ERROR);
#endif
}

void Cubes_GL_BufferStorage::draw(Matrix* transforms, int count)
{
    assert(count <= CUBES_COUNT);

    for (int i = 0; i < count; ++i)
    {
        DrawElementsIndirectCommand *cmd = &m_commands[i];
        cmd->count = 36;
        cmd->instanceCount = 1;
        cmd->firstIndex = 0;
        cmd->baseVertex = 0;
        cmd->baseInstance = 0;
    }

    memcpy(m_transform_ptr, transforms, sizeof(Matrix) * count);
    memcpy(m_cmd_ptr, m_commands, sizeof(DrawElementsIndirectCommand) * count);
    glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, nullptr, count, 0);
}
