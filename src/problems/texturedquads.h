#pragma once

#include "problems/problem.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsProblem : public Problem
{
public:
    TexturedQuadsProblem();

    virtual bool Init() override;
    virtual void Render() override;
    inline virtual std::string GetName() override { return "TexturedQuads"; }

    struct Vertex
    {
        Vec3 pos;
        Vec3 color;
    };

    typedef uint16_t Index;

protected:
    void Update();

    std::vector<Matrix> mTransforms;
    std::vector<Vertex> mVertices;
    std::vector<Index> mIndices;

    unsigned int mIteration;
};
