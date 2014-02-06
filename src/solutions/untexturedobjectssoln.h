#pragma once

#include "solutions/solution.h"
#include "problems/untexturedobjects.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsSolution : public Solution
{
public:
    UntexturedObjectsSolution() { }
    virtual ~UntexturedObjectsSolution() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices, 
                      const std::vector<UntexturedObjectsProblem::Index>& _indices);

    virtual void Render(const std::vector<Matrix>& _transforms) = 0;
    virtual void Shutdown() = 0;

    // The name of this solution.
    virtual std::string GetName() const = 0;

    // The name of the problem this solution addresses.
    virtual std::string GetProblemName() const { return "UntexturedObjects"; }

protected:
};
