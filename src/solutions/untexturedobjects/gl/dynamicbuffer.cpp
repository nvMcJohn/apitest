#include "pch.h"

#include "dynamicbuffer.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLDynamicBuffer::UntexturedObjectsGLDynamicBuffer()
: m_ib()
, m_vb()
, m_ub()
, m_prog()
{}

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsGLDynamicBuffer::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                            const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                            size_t _objectCount)
{
    if (!UntexturedObjectsSolution::Init(_vertices, _indices, _objectCount)) {
        return false;
    }

    // Program
    const char* kUniformNames[] = { "ViewProjection", nullptr };

    m_prog = CreateProgramT("cubes_gl_dynamic_buffer_vs.glsl",
                            "cubes_gl_dynamic_buffer_fs.glsl",
                            kUniformNames, &mUniformLocation);

    if (m_prog == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    GLuint UB0 = glGetUniformBlockIndex(m_prog, "UB0");
    glUniformBlockBinding(m_prog, UB0, 0);

    glGenBuffers(1, &m_vb);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UntexturedObjectsProblem::Vertex) * _vertices.size(), &*_vertices.begin(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(UntexturedObjectsProblem::Index) * _indices.size(), &*_indices.begin(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ub);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLDynamicBuffer::Render(const std::vector<Matrix>& _transforms)
{
    // Program
    // TODO: Should get camera info from problem, too. (Since the problems should be the same dataset).
    Vec3 dir = { -0.5f, -1, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 0, 1 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
    Matrix view = matrix_look_at(eye, at, up);
    Matrix view_proj = mProj * view;

    glUseProgram(m_prog);
    glUniformMatrix4fv(mUniformLocation.ViewProjection, 1, GL_TRUE, &view_proj.x.x);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ub);

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

    glBindBuffer(GL_UNIFORM_BUFFER, m_ub);
    const size_t xformCount = _transforms.size();

    for (size_t i = 0; i < xformCount; ++i)
    {
        Constants cb;
        cb.world = _transforms[i];
        glBufferData(GL_UNIFORM_BUFFER, sizeof(Constants), &cb, GL_DYNAMIC_DRAW);

        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, nullptr);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLDynamicBuffer::Shutdown()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &m_ib);
    glDeleteBuffers(1, &m_vb);
    glDeleteBuffers(1, &m_ub);
    glDeleteProgram(m_prog);
}
