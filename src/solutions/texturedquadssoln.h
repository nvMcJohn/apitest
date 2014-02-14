#pragma once

#include "solutions/solution.h"
#include "problems/texturedquads.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsSolution : public Solution
{
public:
    TexturedQuadsSolution();
    virtual ~TexturedQuadsSolution();

    virtual bool Init(const std::vector<TexturedQuadsProblem::Vertex>& _vertices,
                      const std::vector<TexturedQuadsProblem::Index>& _indices,
                      const std::vector<TextureDetails*>& _textures,
                      size_t _objectCount);

    virtual void Render(const std::vector<Matrix>& _transforms) = 0;
    virtual void Shutdown() = 0;

    // The name of this solution.
    virtual std::string GetName() const = 0;

    // The name of the problem this solution addresses.
    virtual std::string GetProblemName() const { return "TexturedQuadsProblem"; }

protected:
    size_t mObjectCount;
    size_t mIndexCount;
    size_t mTextureCount;
};
