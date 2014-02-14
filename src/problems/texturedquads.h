#pragma once

#include "problems/problem.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class TexturedQuadsProblem : public Problem
{
public:
    TexturedQuadsProblem();
    virtual ~TexturedQuadsProblem() override;

    inline virtual void GetClearValues(Vec4* _outCol, GLfloat* _outDepth) const override
    {
        Vec4 clearCol = { 0.0f, 0.0f, 0.1f, 1.0f };
        (*_outCol) = clearCol;
        (*_outDepth) = 1.0f;
    }

    virtual bool Init() override;
    virtual void Render() override;
    virtual void Shutdown() override;
    inline virtual std::string GetName() override { return "TexturedQuadsProblem"; }

    virtual bool SetSolution(Solution* _solution) override;

    struct Vertex
    {
        Vec3 pos;
        Vec2 tex;
    };

    typedef uint16_t Index;


protected:
    void Update();

    std::vector<Matrix> mTransforms;
    std::vector<Vertex> mVertices;
    std::vector<Index> mIndices;
    std::vector<TextureDetails*> mTextures;

    unsigned int mIteration;

    void genUnitQuad();
    bool loadTextures();
};
