#include "pch.h"
#include "factory.h"

#include "problems/dynamicstreaming.h"
#include "problems/null.h"
#include "problems/texturedquads.h"
#include "problems/untexturedobjects.h"

#include "solutions/nullsoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
ProblemFactory::ProblemFactory()
{
    mProblems.push_back(new NullProblem());
    mSolutions[mProblems.back()->GetName()].push_back(new NullSolution());
}

// --------------------------------------------------------------------------------------------------------------------
std::vector<Problem*> ProblemFactory::GetProblems()
{
    return mProblems;
}

// --------------------------------------------------------------------------------------------------------------------
std::vector<Solution*> ProblemFactory::GetSolutions(Problem* _problem)
{
    assert(_problem);
    return mSolutions[_problem->GetName()];
}
