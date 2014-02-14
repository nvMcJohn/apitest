#include "pch.h"

#include "bufferstorage.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLBufferStorage::UntexturedObjectsGLBufferStorage()
    : m_ib()
    , m_vb()
    , m_prog()
    , m_transform_buffer()
    , m_transform_ptr()
    , m_cmd_buffer()
    , m_cmd_ptr()
{}

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsGLBufferStorage::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                            const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                            size_t _objectCount)
{
    if (!UntexturedObjectsSolution::Init(_vertices, _indices, _objectCount)) {
        return false;
    }

    // Shaders
    m_prog = CreateProgram("cubes_gl_buffer_storage_vs.glsl",
                           "cubes_gl_buffer_storage_fs.glsl");

    if (m_prog == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    glGenBuffers(1, &m_vb);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UntexturedObjectsProblem::Vertex) * _vertices.size(), &*_vertices.begin(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(UntexturedObjectsProblem::Index) * _indices.size(), &*_indices.begin(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_transform_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_transform_buffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, _objectCount * 64, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT);
    m_transform_ptr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, _objectCount * 64, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

    glGenBuffers(1, &m_cmd_buffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_cmd_buffer);
    glBufferStorage(GL_DRAW_INDIRECT_BUFFER, _objectCount * sizeof(DrawElementsIndirectCommand), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT);
    m_cmd_ptr = glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, _objectCount * sizeof(DrawElementsIndirectCommand), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

    m_commands.resize(_objectCount);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBufferStorage::Render(const std::vector<Matrix>& _transforms)
{
    const auto objCount = m_commands.size();
    const auto xformCount = _transforms.size();
    assert(objCount == xformCount);

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

    for (auto it = m_commands.begin(); it != m_commands.end(); ++it) {
        DrawElementsIndirectCommand *cmd = &*it;
        cmd->count = mIndexCount;
        cmd->instanceCount = 1;
        cmd->firstIndex = 0;
        cmd->baseVertex = 0;
        cmd->baseInstance = 0;
    }

    memcpy(m_transform_ptr, &*_transforms.begin(), sizeof(Matrix) * xformCount);
    memcpy(m_cmd_ptr, &*m_commands.begin(), sizeof(DrawElementsIndirectCommand) * objCount);
    glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);

    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_SHORT, nullptr, objCount, 0);
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBufferStorage::Shutdown()
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
    glDeleteProgram(m_prog);
}
