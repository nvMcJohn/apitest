#pragma once

#include "solutions/texturedquadssoln.h"
#include "framework/sparse_bindless_texarray.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsGLTextureArrayMultiDrawBuffer : public TexturedQuadsSolution
{
public:
    TexturedQuadsGLTextureArrayMultiDrawBuffer(bool _useShaderDraw);
    virtual ~TexturedQuadsGLTextureArrayMultiDrawBuffer() { }

    virtual bool Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                      const std::vector<TexturedQuadsProblem::Index>& _indices,
                      const std::vector<TextureDetails*>& _textures,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms) override;
    virtual void Shutdown() override;

    virtual std::string GetName() const override;
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mDrawIDBuffer;
    GLuint mCommandBuffer;
    GLuint mVertexArray;
    GLuint mProgram;
    GLuint mTransformBuffer;
    GLuint mTexAddressBuffer;

    bool mUseShaderDrawParameters;

    std::vector<DrawElementsIndirectCommand> m_commands;

    struct UniformLocations {
        GLuint ViewProjection;
        GLuint TexContainer;
        UniformLocations() { memset(this, 0, sizeof(*this)); }
    } mUniformLocation;

    std::vector<Texture2D*> mTextures;
    std::vector<GLint> mTexUnits;

    TextureManager mTexManager;
};
