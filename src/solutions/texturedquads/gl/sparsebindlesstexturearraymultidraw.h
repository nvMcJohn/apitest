#pragma once

#include "solutions/texturedquadssoln.h"
#include "framework/sparse_bindless_texarray.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsGLSparseBindlessTextureArrayMultiDraw : public TexturedQuadsSolution
{
public:
    TexturedQuadsGLSparseBindlessTextureArrayMultiDraw();
    virtual ~TexturedQuadsGLSparseBindlessTextureArrayMultiDraw() { }

    virtual bool Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                      const std::vector<TexturedQuadsProblem::Index>& _indices,
                      const std::vector<TextureDetails*>& _textures,
                      size_t _objectCount);

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    // The name of this solution.
    virtual std::string GetName() const { return "TexturedQuadsGLSparseBindlessTextureArrayMultiDraw"; }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mProgram;
    GLuint mTransformBuffer;
    GLuint mTexAddressBuffer;

    std::vector<Texture2D*> mTextures;
    std::vector<DrawElementsIndirectCommand> mCommands;

    TextureManager mTexManager;
};
