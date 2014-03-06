#pragma once

#include "solutions/texturedquadssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsGLBindlessMultiDraw : public TexturedQuadsSolution
{
public:
    TexturedQuadsGLBindlessMultiDraw();
    virtual ~TexturedQuadsGLBindlessMultiDraw() { }

    virtual bool Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                      const std::vector<TexturedQuadsProblem::Index>& _indices,
                      const std::vector<TextureDetails*>& _textures,
                      size_t _objectCount);

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const { return "TexturedQuadsGLBindlessMultiDraw"; }
    virtual bool SupportsApi(EGfxApi _api) const override { return IsOpenGL(_api); }

private:
    GLuint mIndexBuffer;
    GLuint mVertexBuffer;
    GLuint mProgram;
    GLuint mTransformBuffer;
    GLuint mTexAddressBuffer;

    std::vector<GLuint> mTextures;
    std::vector<GLuint64> mTexHandles;
    std::vector<DrawElementsIndirectCommand> mCommands;
};

