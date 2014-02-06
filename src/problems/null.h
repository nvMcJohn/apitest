#pragma once

#include "problems/problem.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class NullProblem : public Problem
{
public:
    NullProblem();

    virtual bool Init() override;
    virtual void Render() override;
    inline virtual std::string GetName() override { return "NullProblem"; }

    virtual bool SetSolution(Solution* _solution) override;

protected:
};
