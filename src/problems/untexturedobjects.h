#pragma once

#include "problems/problem.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsProblem : public Problem
{
public:
    UntexturedObjectsProblem();

    inline virtual void GetClearValues(Vec4* _outCol, GLfloat* _outDepth) const override
    {
        Vec4 clearCol = { 0.0f, 0.1f, 0.0f, 1.0f };
        (*_outCol) = clearCol;
        (*_outDepth) = 1.0f;
    }

    virtual bool Init() override;
    virtual void Render() override;
    virtual void Shutdown() override;
    inline virtual std::string GetName() override { return "UntexturedObjects"; }

    virtual bool SetSolution(Solution* _solution) override;

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

    void genUnitCube();
};
