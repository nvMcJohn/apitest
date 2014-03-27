#pragma once

#include "solutions/solution.h"
#include "problems/untexturedobjects.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsSolution : public Solution
{
public:
    UntexturedObjectsSolution();
    virtual ~UntexturedObjectsSolution();

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices, 
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount);

    virtual void Render(const std::vector<Matrix>& _transforms) = 0;
    virtual void Shutdown() = 0;

    virtual std::string GetName() const = 0;

    virtual std::string GetProblemName() const { return "UntexturedObjects"; }

protected:
    size_t mObjectCount;
    size_t mIndexCount;
};
