#include "pch.h"

#include "cubes_gl_bindless.h"
#include "mathlib.h"
#include <assert.h>
#include <stdint.h>

Cubes_GL_Bindless::Cubes_GL_Bindless()
    : m_ibs()
    , m_vbs()
    , m_vs()
    , m_fs()
    , m_prog()
{}

Cubes_GL_Bindless::~Cubes_GL_Bindless()
{
    glDeleteBuffers(CUBES_COUNT, m_ibs);
    glDeleteBuffers(CUBES_COUNT, m_vbs);
    glDeleteShader(m_vs);
    glDeleteShader(m_fs);
    glDeleteProgram(m_prog);

    glDisableClientState(GL_ELEMENT_ARRAY_UNIFIED_NV);
    glDisableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
}

bool Cubes_GL_Bindless::Init()
{
    // Shaders
    if (!CreateShader(GL_VERTEX_SHADER, "cubes_gl_bindless_vs.glsl", &m_vs))
        return false;

    if (!CreateShader(GL_FRAGMENT_SHADER, "cubes_gl_bindless_fs.glsl", &m_fs))
        return false;

    if (!CompileProgram(&m_prog, m_vs, m_fs, 0))
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

    glGenBuffers(CUBES_COUNT, m_ibs);
    glGenBuffers(CUBES_COUNT, m_vbs);
    for (int i = 0; i < CUBES_COUNT; ++i)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibs[i]);
        glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, 0);
        glGetBufferParameterui64vNV(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &m_ib_addrs[i]);
        glMakeBufferResidentNV(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
        m_ib_sizes[i] = sizeof(indices);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbs[i]);
        glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);
        glGetBufferParameterui64vNV(GL_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &m_vbo_addrs[i]);
        glMakeBufferResidentNV(GL_ARRAY_BUFFER, GL_READ_ONLY);
        m_vbo_sizes[i] = sizeof(vertices);
    }


    return glGetError() == GL_NO_ERROR;
}

bool Cubes_GL_Bindless::Begin(GfxBaseApi* _activeAPI)
{
    int width = _activeAPI->GetWidth();
    int height = _activeAPI->GetHeight();

    float c[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float d = 1.0f;
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
    glEnableClientState(GL_ELEMENT_ARRAY_UNIFIED_NV);
    glEnableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribFormatNV(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex));
    glVertexAttribFormatNV(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex));

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

void Cubes_GL_Bindless::Draw(Matrix* transforms, int count)
{
    for (int i = 0; i < count; ++i)
    {
        glBufferAddressRangeNV(GL_ELEMENT_ARRAY_ADDRESS_NV, 0, m_ib_addrs[i], m_ib_sizes[i]);
        glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 0, m_vbo_addrs[i] + offsetof(Vertex, pos), m_vbo_sizes[i] - offsetof(Vertex, pos));
        glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 1, m_vbo_addrs[i] + offsetof(Vertex, color), m_vbo_sizes[i] - offsetof(Vertex, color));

        glUniformMatrix4fv(1, 1, GL_FALSE, &transforms[i].x.x);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
    }
}
