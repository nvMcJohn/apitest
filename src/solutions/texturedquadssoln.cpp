
#include "pch.h"
#include "texturedquadssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsSolution::TexturedQuadsSolution()
: mObjectCount()
, mIndexCount()
{ }

// --------------------------------------------------------------------------------------------------------------------
TexturedQuadsSolution::~TexturedQuadsSolution()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool TexturedQuadsSolution::Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                                 const std::vector<TexturedQuadsProblem::Index>& _indices,
                                 const std::vector<TextureDetails*>& _textures,
                                 size_t _objectCount)
{
    mObjectCount = _objectCount;
    mIndexCount = _indices.size();
    mTextureCount = _textures.size();

    return true;
}

