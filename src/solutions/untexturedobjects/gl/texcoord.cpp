#include "pch.h"

#include "texcoord.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLTexCoord::UntexturedObjectsGLTexCoord()
: m_ib()
, m_vb()
, m_prog()
{}


// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsGLTexCoord::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                       const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                       size_t _objectCount)
{
    if (!UntexturedObjectsSolution::Init(_vertices, _indices, _objectCount)) {
        return false;
    }

    // Program
    const char* kUniformNames[] = { "ViewProjection", nullptr };

    m_prog = CreateProgramT("cubes_gl_tex_coord_vs.glsl",
                            "cubes_gl_tex_coord_fs.glsl",
                            kUniformNames, &mUniformLocation);

    if (m_prog == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    // Buffers
    glGenBuffers(1, &m_vb);
    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(UntexturedObjectsProblem::Vertex) * _vertices.size(), &*_vertices.begin(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(UntexturedObjectsProblem::Index) * _indices.size(), &*_indices.begin(), GL_STATIC_DRAW);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLTexCoord::Render(const std::vector<Matrix>& _transforms)
{
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

    for (auto it = _transforms.begin(); it != _transforms.end(); ++it) {
        const Matrix* m = &*it;
    
        glVertexAttrib4f(2, m->x.x, m->x.y, m->x.z, m->x.w);
        glVertexAttrib4f(3, m->y.x, m->y.y, m->y.z, m->y.w);
        glVertexAttrib4f(4, m->z.x, m->z.y, m->z.z, m->z.w);
        glVertexAttrib4f(5, m->w.x, m->w.y, m->w.z, m->w.w);

        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, nullptr);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLTexCoord::Shutdown()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glDeleteBuffers(1, &m_ib);
    glDeleteBuffers(1, &m_vb);
    glDeleteProgram(m_prog);
}
