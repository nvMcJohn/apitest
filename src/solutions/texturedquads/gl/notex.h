#pragma once

#include "solutions/texturedquadssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsGLNoTex : public TexturedQuadsSolution
{
public:
    TexturedQuadsGLNoTex();
    virtual ~TexturedQuadsGLNoTex() { }

    virtual bool Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                      const std::vector<TexturedQuadsProblem::Index>& _indices,
                      const std::vector<TextureDetails*>& _textures,
                      size_t _objectCount);

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const { return "GLNoTex"; }
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mDrawIDBuffer;
    GLuint mVertexArray;
    GLuint mProgram;

    // If SSBO is supported, this is used.
    GLuint mTransformStorageBuffer;

    // Otherwise on OSX, we use this.
    std::vector<GLuint> mTransformUniformBuffers;
    GLint mMaxDrawsPerKickoff;

    struct UniformLocations {
        GLuint ViewProjection;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
