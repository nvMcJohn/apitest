#include "pch.h"

#include "bindless.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLBindless::UntexturedObjectsGLBindless()
: m_ibs()
, m_vbs()
, m_prog()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsGLBindless::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                       const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                       size_t _objectCount)
{
    if (!UntexturedObjectsSolution::Init(_vertices, _indices, _objectCount)) {
        return false;
    }

    // Shaders
    m_prog = CreateProgram("cubes_gl_bindless_vs.glsl", 
                           "cubes_gl_bindless_fs.glsl");

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

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBindless::Render(const std::vector<Matrix>& _transforms)
{
    // Program
    Vec3 dir = { -0.5f, -1, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 0, 1 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
    Matrix view = matrix_look_at(eye, at, up);
    Matrix viewProj = mProj * view;

    glUseProgram(m_prog);
    glUniformMatrix4fv(0, 1, GL_TRUE, &viewProj.x.x);

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


    for (size_t u = 0; u < _transforms.size(); ++u)
    {
        glBufferAddressRangeNV(GL_ELEMENT_ARRAY_ADDRESS_NV, 0, m_ib_addrs[u], m_ib_sizes[u]);
        glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 0, m_vbo_addrs[u] + offsetof(UntexturedObjectsProblem::Vertex, pos), m_vbo_sizes[u] - offsetof(UntexturedObjectsProblem::Vertex, pos));
        glBufferAddressRangeNV(GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV, 1, m_vbo_addrs[u] + offsetof(UntexturedObjectsProblem::Vertex, color), m_vbo_sizes[u] - offsetof(UntexturedObjectsProblem::Vertex, color));

        glUniformMatrix4fv(1, 1, GL_FALSE, &_transforms[u].x.x);
        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, nullptr);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBindless::Shutdown()
{
    if (m_ibs.size()) {
        glDeleteBuffers(m_ibs.size(), &*m_ibs.begin());
        m_ibs.clear();
    }

    if (m_vbs.size()) {
        glDeleteBuffers(m_vbs.size(), &*m_vbs.begin());
        m_vbs.clear();
    }

    glDeleteProgram(m_prog);
    m_prog = 0;

    glDisableClientState(GL_ELEMENT_ARRAY_UNIFIED_NV);
    glDisableClientState(GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV);
}
