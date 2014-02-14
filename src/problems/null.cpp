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
        console::log("Solution %s - Initializing.", mActiveSolution->GetName().c_str());
        bool retVal = reinterpret_cast<NullSolution*>(mActiveSolution)->Init();
        console::log("Solution %s - Initialize complete (Success: %s).", mActiveSolution->GetName().c_str(), retVal ? "true" : "false");
        return retVal;
    }

    return true;
}
