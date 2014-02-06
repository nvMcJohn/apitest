#pragma once

#include "solutions/solution.h"
#include "problems/null.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class NullSolution : public Solution
{
public:
    NullSolution() { }
    virtual ~NullSolution() { }

    bool Init();

    void Render();
    virtual void Shutdown() override;

    // The name of this solution.
    inline virtual std::string GetName() const { return "NullSolution"; }

    // The name of the problem this solution addresses.
    inline virtual std::string GetProblemName() const { return "NullProblem"; }

protected:
};
