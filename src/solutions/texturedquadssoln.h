#pragma once

class Problem;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class Solution
{
public:
    Solution() { }
    virtual ~Solution() { }

    virtual bool Init(Problem* _problem) = 0;
    // Render is provided by subclasses.
    virtual bool Shutdown() = 0;

    // The name of this solution.
    virtual std::string GetName() const = 0;

    // The name of the problem this solution addresses.
    virtual std::string GetProblemName() const = 0;

protected:
};
