
#include "pch.h"
#include "framework/gfx_gl.h"
#include "problems/problem.h"
#include "solutions/solution.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
bool Problem::SetSolution(Solution* _solution)
{
    assert(_solution == nullptr || _solution->GetProblemName() == GetName());

    if (mActiveSolution) {
        console::log("Solution %s - shutdown beginning.", mActiveSolution->GetName().c_str());
        mActiveSolution->Shutdown();
        auto err = GLRenderer::GetApiError();
        console::log("Solution %s shutdown complete.", mActiveSolution->GetName().c_str());
    }

    mActiveSolution = _solution;

    // The parameters to be handed off by Init are specific to the problem being solved, so is called after this
    // by the derived class.
    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void Problem::Shutdown()
{
    SetSolution(nullptr);
}

