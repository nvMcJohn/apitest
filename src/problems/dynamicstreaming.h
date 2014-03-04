#pragma once

#include "problems/problem.h"

const size_t kVertsPerParticle = 6;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class DynamicStreamingProblem : public Problem
{
public:
    DynamicStreamingProblem();
    virtual ~DynamicStreamingProblem();

    inline virtual void GetClearValues(Vec4* _outCol, GLfloat* _outDepth) const override
    {
        Vec4 clearCol = { 0.3f, 0.0f, 0.3f, 1.0f };
        (*_outCol) = clearCol;
        (*_outDepth) = 1.0f;
    }

    virtual bool Init() override;
    virtual void Render() override;
    virtual void Shutdown() override;
    inline virtual std::string GetName() override { return "DynamicStreaming"; }

    virtual bool SetSolution(Solution* _solution) override;

protected:
    void Update();

    std::vector<Vec2> mVertexData;
    unsigned int mIteration;
};
