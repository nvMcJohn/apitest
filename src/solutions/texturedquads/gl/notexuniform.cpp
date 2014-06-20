#include "pch.h"

#include "notexuniform.h"
#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsGLNoTexUniform::TexturedQuadsGLNoTexUniform()
: mIndexBuffer()
, mVertexBuffer()
, mVertexArrayObject()
, mProgram()
, mTransformBuffer()
{}

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsGLNoTexUniform::Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                                       const std::vector<TexturedQuadsProblem::Index>& _indices,
                                       const std::vector<TextureDetails*>& _textures,
                                       size_t _objectCount)
{
    if (!TexturedQuadsSolution::Init(_vertices, _indices, _textures, _objectCount)) {
        return false;
    }

    // Program
    const char* kUniformNames[] = { "ViewProjection", "DrawID", nullptr };

    mProgram = CreateProgramT("textures_gl_notex_uniform_vs.glsl",
                              "textures_gl_notex_uniform_fs.glsl",
                              kUniformNames, &mUniformLocation);

    if (mProgram == 0) {
        console::warn("Unable to initialize solution '%s', shader compilation/linking failed.", GetName().c_str());
        return false;
    }

    glGenVertexArrays(1, &mVertexArrayObject);
    glBindVertexArray(mVertexArrayObject);

    // Buffers
    mVertexBuffer = NewBufferFromVector(GL_ARRAY_BUFFER, _vertices, GL_STATIC_DRAW);
    mIndexBuffer = NewBufferFromVector(GL_ELEMENT_ARRAY_BUFFER, _indices, GL_STATIC_DRAW);

    glGenBuffers(1, &mTransformBuffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mTransformBuffer);

    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);

    return glGetError() == GL_NO_ERROR;
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLNoTexUniform::Render(const std::vector<Matrix>& _transforms)
{
    // Program
    Vec3 dir = { 0, 0, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 1, 0 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
    Matrix view = matrix_look_at(eye, at, up);
    Matrix view_proj = mProj * view;

    glUseProgram(mProgram);
    glUniformMatrix4fv(mUniformLocation.ViewProjection, 1, GL_TRUE, &view_proj.x.x);

    // Input Layout. First the IB
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

    // Then the VBs.
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadsProblem::Vertex), (void*)offsetof(TexturedQuadsProblem::Vertex, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedQuadsProblem::Vertex), (void*)offsetof(TexturedQuadsProblem::Vertex, tex));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Rasterizer State
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    glDisable(GL_SCISSOR_TEST);

    // Blend State
    glDisable(GL_BLEND);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // Depth Stencil State
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, mTransformBuffer);
    BufferData(GL_SHADER_STORAGE_BUFFER, _transforms, GL_DYNAMIC_DRAW);
    size_t xformCount = _transforms.size();
    assert(xformCount <= mObjectCount);

    for (size_t u = 0; u < xformCount; ++u) {
        // Update the Draw ID (since we cannot use multi_draw here
        glUniform1i(mUniformLocation.DrawID, u);

        glDrawElements(GL_TRIANGLES, mIndexCount, GL_UNSIGNED_SHORT, 0);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void TexturedQuadsGLNoTexUniform::Shutdown()
{
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    // No textures to clean up here.

    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mTransformBuffer);
    glDeleteVertexArrays(1, &mVertexArrayObject);
    glDeleteProgram(mProgram);
}
