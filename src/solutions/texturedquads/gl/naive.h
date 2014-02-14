#pragma once

#include "solutions/texturedquadssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsGLNaive : public TexturedQuadsSolution
{
public:
    TexturedQuadsGLNaive();
    virtual ~TexturedQuadsGLNaive() { }

    virtual bool Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                      const std::vector<TexturedQuadsProblem::Index>& _indices,
                      const std::vector<TextureDetails*>& _textures,
                      size_t _objectCount);

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    // The name of this solution.
    virtual std::string GetName() const { return "TexturedQuadsGLNaive"; }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mProgram;
    GLuint mTransformBuffer;

    std::vector<GLuint> mTextures;
};
