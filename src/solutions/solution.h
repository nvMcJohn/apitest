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

    virtual void Shutdown() = 0;

    // The name of this solution.
    virtual std::string GetName() const = 0;

    // The name of the problem this solution addresses.
    virtual std::string GetProblemName() const = 0;

    inline void SetProjectionMatrix(const Matrix& _proj) { mProj = _proj; }

protected:
    Matrix mProj;
};
