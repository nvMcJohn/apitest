#include "pch.h"

#include "multidraw.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLMultiDraw::UntexturedObjectsGLMultiDraw()
: m_ib()
, m_vb()
, m_prog()
, m_transform_buffer()
{ }

// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLMultiDraw::~UntexturedObjectsGLMultiDraw()
{
    
}

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsGLMultiDraw::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                        const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                        size_t _objectCount)
{
    // Shaders
    m_prog = CreateProgram("cubes_gl_multi_draw_vs.glsl", "cubes_gl_multi_draw_fs.glsl");
    if (!m_prog) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
    }

    // Buffers
    glGenBuffers(1, &m_vb);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(UntexturedObjectsProblem::Vertex), &*_vertices.begin(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(UntexturedObjectsProblem::Index), &*_indices.begin(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_transform_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_transform_buffer);

    // Set the command buffer size.
    m_commands.resize(_objectCount);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLMultiDraw::Render(const std::vector<Matrix>& _transforms)
{
    size_t count = _transforms.size();
    assert(count <= m_commands.size());

    // Program
    Vec3 dir = { -0.5f, -1, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 0, 1 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
    Matrix view = matrix_look_at(eye, at, up);
    Matrix view_proj = mProj * view;

    glUseProgram(m_prog);
    glUniformMatrix4fv(0, 1, GL_TRUE, &view_proj.x.x);

    // Input Layout
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(UntexturedObjectsProblem::Vertex), (void*)offsetof(UntexturedObjectsProblem::Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(UntexturedObjectsProblem::Vertex), (void*)offsetof(UntexturedObjectsProblem::Vertex, color));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Rasterizer State
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glDisable(GL_SCISSOR_TEST);

    // Blend State
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Depth Stencil State
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    for (size_t u = 0; u < count; ++u)
    {
        DrawElementsIndirectCommand *cmd = &m_commands[u];
        cmd->count = 36;
        cmd->instanceCount = 1;
        cmd->firstIndex = 0;
        cmd->baseVertex = 0;
        cmd->baseInstance = 0;
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_transform_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _transforms.size() * sizeof(Matrix), &*_transforms.begin(), GL_DYNAMIC_DRAW);

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, &*m_commands.begin(), count, 0);
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLMultiDraw::Shutdown()
{
    glDeleteBuffers(1, &m_ib);
    glDeleteBuffers(1, &m_vb);
    glDeleteBuffers(1, &m_transform_buffer);
    glDeleteProgram(m_prog);
}