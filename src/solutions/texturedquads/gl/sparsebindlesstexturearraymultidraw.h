#pragma once

#include "solutions/texturedquadssoln.h"
#include "framework/sparse_bindless_texarray.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsGLSparseBindlessTextureArrayMultiDraw : public TexturedQuadsSolution
{
public:
    TexturedQuadsGLSparseBindlessTextureArrayMultiDraw(bool _useShaderDraw);
    virtual ~TexturedQuadsGLSparseBindlessTextureArrayMultiDraw() { }

    virtual bool Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                      const std::vector<TexturedQuadsProblem::Index>& _indices,
                      const std::vector<TextureDetails*>& _textures,
                      size_t _objectCount);

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const;
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mDrawIDBuffer;
    GLuint mProgram;
    GLuint mTransformBuffer;
    GLuint mTexAddressBuffer;

    bool mUseShaderDrawParameters;

    std::vector<Texture2D*> mTextures;
    std::vector<DrawElementsIndirectCommand> mCommands;

    TextureManager mTexManager;
};
