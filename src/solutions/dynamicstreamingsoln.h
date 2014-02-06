#pragma once

#include "solutions/solution.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class DynamicStreamingSolution : public Solution
{
public:
    DynamicStreamingSolution() { }
    virtual ~DynamicStreamingSolution() { }

    virtual bool Init(Problem* _problem) = 0;
    virtual void Render(const std::vector<Vec2>& _vertices) = 0;
    virtual void Shutdown() = 0;

    // The name of this solution.
    virtual std::string GetName() const = 0;

    // The name of the problem this solution addresses.
    virtual std::string GetProblemName() const { return "DynamicStreaming"; }

protected:
};
