#include "pch.h"

#include "bufferrange.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsGLBufferRange::UntexturedObjectsGLBufferRange()
    : m_ib()
    , m_vb()
    , m_ub()
    , m_prog()
{}

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsGLBufferRange::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices,
                                          const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                          size_t _objectCount)
{
    if (!UntexturedObjectsSolution::Init(_vertices, _indices, _objectCount)) {
        return false;
    }

    // Program
    const char* kUniformNames[] = { "ViewProjection", nullptr };

    m_prog = CreateProgramT("cubes_gl_buffer_range_vs.glsl",
                            "cubes_gl_buffer_range_fs.glsl",
                            kUniformNames, &mUniformLocation);

    if (m_prog == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    GLuint UB0 = glGetUniformBlockIndex(m_prog, "UB0");
    glUniformBlockBinding(m_prog, UB0, 0);

    glGenBuffers(1, &m_vb);
    glGenBuffers(1, &m_ib);

    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(UntexturedObjectsProblem::Vertex), &*_vertices.begin(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(UntexturedObjectsProblem::Index), &*_indices.begin(), GL_STATIC_DRAW);


    glGenBuffers(1, &m_ub);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBufferRange::Render(const std::vector<Matrix>& _transforms)
{
    const auto xformCount = _transforms.size();

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

    GLint align;
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
    int stride = align;
    assert(stride >= sizeof(Matrix));

    GLint maxSize;
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &maxSize);
    m_storage.resize(maxSize);

    glBindBuffer(GL_UNIFORM_BUFFER, m_ub);

    int batchMax = maxSize / stride;
    for (size_t batchStart = 0; batchStart < xformCount; batchStart += batchMax)
    {
        int batchCount = xformCount - batchStart;
        if (batchCount > batchMax)
            batchCount = batchMax;

        for (int i = 0; i < batchCount; ++i)
        {
            char *dst = &m_storage[stride * i];
            *(Matrix *)dst = _transforms[batchStart + i];
        }

        glBufferData(GL_UNIFORM_BUFFER, stride * batchCount, m_storage.data(), GL_DYNAMIC_DRAW);

        for (int i = 0; i < batchCount; ++i)
        {
            glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_ub, stride * i, sizeof(Matrix));

            glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, nullptr);
        }
    }
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsGLBufferRange::Shutdown()
{
    glDeleteBuffers(1, &m_ib);
    glDeleteBuffers(1, &m_vb);
    glDeleteBuffers(1, &m_ub);
    glDeleteProgram(m_prog);
}
