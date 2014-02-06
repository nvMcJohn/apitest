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
    console::debug("Initializing NullProblem\n");
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
        console::debug("NullProblem::SetSolution(%s)\n", mActiveSolution->GetName().c_str());

        return reinterpret_cast<NullSolution*>(mActiveSolution)->Init();
    }

    console::debug("NullProblem::SetSolution(%s)\n", "nullptr");

    return true;
}
