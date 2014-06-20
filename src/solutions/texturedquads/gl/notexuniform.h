#pragma once

#include "solutions/texturedquadssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsGLNoTexUniform : public TexturedQuadsSolution
{
public:
    TexturedQuadsGLNoTexUniform();
    virtual ~TexturedQuadsGLNoTexUniform() { }

    virtual bool Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                      const std::vector<TexturedQuadsProblem::Index>& _indices,
                      const std::vector<TextureDetails*>& _textures,
                      size_t _objectCount);

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const { return "GLNoTexUniform"; }
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mVertexArrayObject;
    GLuint mProgram;
    GLuint mTransformBuffer;
    GLuint mVAO;

    struct UniformLocations {
        GLuint ViewProjection;
        GLuint DrawID;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;
};
