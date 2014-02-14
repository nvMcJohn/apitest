#include "pch.h"

#include "bindlessindirect.h"
#include "framework/gfx_gl.h"

const size_t kQueryCount = 4;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLBindlessIndirect::UntexturedObjectsGLBindlessIndirect()
: m_ibs()
, m_vbs()
, m_prog()
, m_currentQueryIssue(0)
, m_currentQueryGet(-1)
, m_transform_buffer()
, m_transform_ptr()
, m_cmd_buffer()
, m_cmd_ptr()
{
}

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsGLBindlessIndirect::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                               const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                               size_t _objectCount)
{
    if (!UntexturedObjectsSolution::Init(_vertices, _indices, _objectCount)) {
        return false;
    }

    m_prog = CreateProgram("cubes_gl_bindless_indirect_vs.glsl",
                           "cubes_gl_bindless_indirect_fs.glsl");

    if (m_prog == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    m_ibs.resize(_objectCount);
    m_ib_addrs.resize(_objectCount);
    m_ib_sizes.resize(_objectCount);

    m_vbs.resize(_objectCount);
    m_vbo_addrs.resize(_objectCount);
    m_vbo_sizes.resize(_objectCount);

    m_commands.resize(_objectCount);

    glGenBuffers(_objectCount, &*m_ibs.begin());
    glGenBuffers(_objectCount, &*m_vbs.begin());

    for (size_t u = 0; u < _objectCount; ++u)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibs[u]);
        glBufferStorage(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(UntexturedObjectsProblem::Index), &*_indices.begin(), 0);
        glGetBufferParameterui64vNV(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &m_ib_addrs[u]);
        glMakeBufferResidentNV(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
        m_ib_sizes[u] = _indices.size() * sizeof(UntexturedObjectsProblem::Index);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbs[u]);
        glBufferStorage(GL_ARRAY_BUFFER, _vertices.size() * sizeof(UntexturedObjectsProblem::Vertex), &*_vertices.begin(), 0);
        glGetBufferParameterui64vNV(GL_ARRAY_BUFFER, GL_BUFFER_GPU_ADDRESS_NV, &m_vbo_addrs[u]);
        glMakeBufferResidentNV(GL_ARRAY_BUFFER, GL_READ_ONLY);
        m_vbo_sizes[u] = _vertices.size() * sizeof(UntexturedObjectsProblem::Vertex);
    }

    glGenBuffers(1, &m_transform_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_transform_buffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, _objectCount * 64, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT);
    m_transform_ptr = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, _objectCount * 64, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

    glGenBuffers(1, &m_cmd_buffer);
    glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_cmd_buffer);
    glBufferStorage(GL_DRAW_INDIRECT_BUFFER, _objectCount * sizeof(Command), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_DYNAMIC_STORAGE_BIT);
    m_cmd_ptr = glMapBufferRange(GL_DRAW_INDIRECT_BUFFER, 0, _objectCount * sizeof(Command), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);

    m_queries.resize(4);
    glGenQueries(kQueryCount, &*m_queries.begin());

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBindlessIndirect::Render(const std::vector<Matrix>& _transforms)
{
    const auto xformCount = _transforms.size();
    const auto objCount = m_commands.size();
    assert(xformCount == objCount);

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
    glEnableClientState(GL_ELEMENT_ARRAY_UNIFIED_NV);
    glEnableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribFormatNV(0, 3, GL_FLOAT, GL_FALSE, sizeof(UntexturedObjectsProblem::Vertex));
    glVertexAttribFormatNV(1, 3, GL_FLOAT, GL_FALSE, sizeof(UntexturedObjectsProblem::Vertex));

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

    size_t i = 0;
    for (auto it = m_commands.begin(); it != m_commands.end(); ++it) {
        Command *cmd = &*it;

        cmd->Draw.count = mIndexCount;
        cmd->Draw.instanceCount = 1;
        cmd->Draw.firstIndex = 0;
        cmd->Draw.baseVertex = 0;
        cmd->Draw.baseInstance = 0;
        cmd->reserved = 0;
        cmd->indexBuffer.index = 0;
        cmd->indexBuffer.reserved = 0;
        cmd->indexBuffer.address = m_ib_addrs[i];
        cmd->indexBuffer.length = m_ib_sizes[i];
        cmd->vertexBuffers[0].index = 0;
        cmd->vertexBuffers[0].reserved = 0;
        cmd->vertexBuffers[0].address = m_vbo_addrs[i] + offsetof(UntexturedObjectsProblem::Vertex, pos);
        cmd->vertexBuffers[0].length = m_vbo_sizes[i] - offsetof(UntexturedObjectsProblem::Vertex, pos);
        cmd->vertexBuffers[1].index = 1;
        cmd->vertexBuffers[1].reserved = 0;
        cmd->vertexBuffers[1].address = m_vbo_addrs[i] + offsetof(UntexturedObjectsProblem::Vertex, color);
        cmd->vertexBuffers[1].length = m_vbo_sizes[i] - offsetof(UntexturedObjectsProblem::Vertex, color);

        ++i;
    }

    memcpy(m_transform_ptr, &*_transforms.begin(), sizeof(Matrix) * xformCount);

    memcpy(m_cmd_ptr, &*m_commands.begin(), sizeof(Command) * objCount);

    glMemoryBarrier(GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT);

    // resolveQueries();

    // glBeginQuery(GL_TIME_ELAPSED, m_queries[m_currentQueryIssue]);
    glMultiDrawElementsIndirectBindlessNV(GL_TRIANGLES, GL_UNSIGNED_SHORT, nullptr, objCount, 0, 2);
    // glEndQuery(GL_TIME_ELAPSED);

    // m_currentQueryIssue = (m_currentQueryIssue + 1) % kQueryCount;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBindlessIndirect::Shutdown()
{
    if (m_transform_ptr)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_transform_buffer);
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
    glDeleteBuffers(1, &m_transform_buffer);

    if (m_cmd_ptr)
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_cmd_buffer);
        glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER);
    }

    glDeleteQueries(m_queries.size(), &*m_queries.begin());
    glDeleteBuffers(1, &m_cmd_buffer);

    glDeleteBuffers(m_ibs.size(), &*m_ibs.begin());
    glDeleteBuffers(m_vbs.size(), &*m_vbs.begin());
    glDeleteProgram(m_prog);

    // TODO: These could also go in ::End. 
    glDisableClientState(GL_ELEMENT_ARRAY_UNIFIED_NV);
    glDisableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBindlessIndirect::resolveQueries()
{
    // Only happens the first time, and we don't need to resolve to move forward.
    if (m_currentQueryGet == -1) {
        m_currentQueryGet = 0;
        return;
    }

    while (1) {
        GLuint available = GL_FALSE;
        glGetQueryObjectuiv(m_queries[m_currentQueryGet], GL_QUERY_RESULT_AVAILABLE, &available);
        if (available == GL_FALSE && ((m_currentQueryIssue + 1) % kQueryCount) != m_currentQueryGet) {
            // If we're not already overlapping, can just exit if the result is unavailable.
            break;
        }

        GLint64 timeElapsed = 0;
        glGetQueryObjecti64v(m_queries[m_currentQueryGet], GL_QUERY_RESULT, &timeElapsed);

        console::debug("Elapsed GPU: %.2f ms\n", float(timeElapsed) / 1000000.0f);

        m_currentQueryGet = (m_currentQueryGet + 1) % kQueryCount;
        if (m_currentQueryGet == m_currentQueryIssue) {
            break;
        }
    }
}
