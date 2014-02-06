#pragma once

#include "problems/problem.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsProblem : public Problem
{
public:
    UntexturedObjectsProblem();

    virtual bool Init() override;
    virtual void Render() override;
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
