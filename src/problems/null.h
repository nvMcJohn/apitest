#pragma once

#include "problems/problem.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class NullProblem : public Problem
{
public:
    NullProblem();

    inline virtual void GetClearValues(Vec4* _outCol, GLfloat* _outDepth) const override 
    {
        Vec4 clearCol = { 0.1f, 0.0f, 0.0f, 1.0f };
        (*_outCol) = clearCol;
        (*_outDepth) = 1.0f;
    }

    virtual bool Init() override;
    virtual void Render() override;
    inline virtual std::string GetName() override { return "NullProblem"; }

    virtual bool SetSolution(Solution* _solution) override;

protected:
};
