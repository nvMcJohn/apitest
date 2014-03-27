#pragma once

#include "framework/gfx.h"

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

    // Whether this solution could conceivably run on this Graphics API. 
    // If the support is conditional on an extension, the function should return
    // true and then test for specific support in the Init function (returning false if unsupported).
    virtual bool SupportsApi(EGfxApi _api) const = 0;

    inline void SetSize(size_t width, size_t height)
    {
        mWidth = width;
        mHeight = height;
        mProj = matrix_perspective_rh_gl(radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);
    }

protected:
    Matrix mProj;
    size_t mWidth;
    size_t mHeight;
};
