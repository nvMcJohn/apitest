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

    virtual bool Init(size_t _maxVertexCount) = 0;
    virtual void Render(const std::vector<Vec2>& _vertices) = 0;
    virtual void Shutdown() = 0;

    virtual std::string GetName() const = 0;

    virtual std::string GetProblemName() const { return "DynamicStreaming"; }

protected:
};
