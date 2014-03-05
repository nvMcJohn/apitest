#pragma once

#include "solutions/texturedquadssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsGLNaiveUniform : public TexturedQuadsSolution
{
public:
    TexturedQuadsGLNaiveUniform();
    virtual ~TexturedQuadsGLNaiveUniform() { }

    virtual bool Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                      const std::vector<TexturedQuadsProblem::Index>& _indices,
                      const std::vector<TextureDetails*>& _textures,
                      size_t _objectCount);

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    // The name of this solution.
    virtual std::string GetName() const { return "TexturedQuadsGLNaiveUniform"; }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mProgram;
    GLuint mTransformBuffer;

    struct UniformLocations {
        GLuint ViewProjection;
        GLuint DrawID;
        GLuint gTex;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;

    std::vector<GLuint> mTextures;
};
