
#include "pch.h"
#include "problems/problem.h"
#include "solutions/solution.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
bool Problem::SetSolution(Solution* _solution)
{
    assert(_solution == nullptr || _solution->GetProblemName() == GetName());

    if (mActiveSolution) {
        mActiveSolution->Shutdown();
    }

    mActiveSolution = _solution;

    // The parameters to be handed off by Init are specific to the problem being solved, so is called after this
    // by the derived class.
    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void Problem::Shutdown()
{
    if (mActiveSolution) {
        mActiveSolution->Shutdown();
    }
}

