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

    inline virtual std::string GetName() const { return "NullSolution"; }
    inline virtual std::string GetProblemName() const { return "NullProblem"; }
    inline virtual bool SupportsApi(EGfxApi _api) const { return true; }


protected:
};
