#include "pch.h"
#include "problems/null.h"

#include "solutions/nullsoln.h"


// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
NullProblem::NullProblem()
{ }

// --------------------------------------------------------------------------------------------------------------------
bool NullProblem::Init()
{
    // Nothing to initialize
    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void NullProblem::Render()
{
    // Nothing, because it's the NULL solution.
}

// --------------------------------------------------------------------------------------------------------------------
bool NullProblem::SetSolution(Solution* _solution)
{
    if (!Problem::SetSolution(_solution)) {
        return false;
    }
    
    if (mActiveSolution) {
        return reinterpret_cast<NullProblem*>(mActiveSolution)->Init();
    }

    return true;
}
