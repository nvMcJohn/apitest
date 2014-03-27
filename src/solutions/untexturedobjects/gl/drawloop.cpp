#include "pch.h"

#include "drawloop.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLDrawLoop::UntexturedObjectsGLDrawLoop()
: m_ib()
, m_vb()
, m_varray()
, m_drawid()
, m_prog()
, m_transform_buffer()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsGLDrawLoop::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                       const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                       size_t _objectCount)
{
    if (!UntexturedObjectsSolution::Init(_vertices, _indices, _objectCount)) {
        return false;
    }

    // Program
    const char* kUniformNames[] = { "ViewProjection", nullptr };

    m_prog = CreateProgramT("cubes_gl_multi_draw_vs.glsl",
                            "cubes_gl_multi_draw_fs.glsl",
                            kUniformNames, &mUniformLocation);

    if (m_prog == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    glGenVertexArrays(1, &m_varray);
    glBindVertexArray(m_varray);

    // Buffers
    glGenBuffers(1, &m_vb);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(UntexturedObjectsProblem::Vertex), &*_vertices.begin(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(UntexturedObjectsProblem::Vertex), (void*) offsetof(UntexturedObjectsProblem::Vertex, pos));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(UntexturedObjectsProblem::Vertex), (void*) offsetof(UntexturedObjectsProblem::Vertex, color));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    std::vector<uint32_t> drawids(_objectCount);
    for (uint32_t i = 0; i < _objectCount; ++i) {
        drawids[i] = i;
    }

    glGenBuffers(1, &m_drawid);
    glBindBuffer(GL_ARRAY_BUFFER, m_drawid);
    glBufferData(GL_ARRAY_BUFFER, drawids.size() * sizeof(uint32_t), drawids.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_INT, sizeof(uint32_t), 0);
    glVertexAttribDivisor(2, 1);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &m_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(UntexturedObjectsProblem::Index), &*_indices.begin(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_transform_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_transform_buffer);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLDrawLoop::Render(const std::vector<Matrix>& _transforms)
{
    size_t count = _transforms.size();
    assert(count <= mObjectCount);

    // Program
    Vec3 dir = { -0.5f, -1, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 0, 1 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
    Matrix view = matrix_look_at(eye, at, up);
    Matrix view_proj = mProj * view;

    glUseProgram(m_prog);
    glUniformMatrix4fv(mUniformLocation.ViewProjection, 1, GL_TRUE, &view_proj.x.x);

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

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_transform_buffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, _transforms.size() * sizeof(Matrix), &*_transforms.begin(), GL_DYNAMIC_DRAW);

    for (size_t u = 0; u < count; ++u) {
        glDrawElementsInstancedBaseInstance(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, nullptr, 1, u);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLDrawLoop::Shutdown()
{
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &m_ib);
    glDeleteBuffers(1, &m_vb);
    glDeleteVertexArrays(1, &m_varray);
    glDeleteBuffers(1, &m_drawid);
    glDeleteBuffers(1, &m_transform_buffer);
    glDeleteProgram(m_prog);
}
