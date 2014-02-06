#include "pch.h"
#include "factory.h"

#include "problems/dynamicstreaming.h"
#include "problems/null.h"
#include "problems/texturedquads.h"
#include "problems/untexturedobjects.h"

#include "solutions/nullsoln.h"
#include "solutions/untexturedobjects/gl/bindless.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
ProblemFactory::ProblemFactory()
{
    Problem* newProb = nullptr;
    // Null
    newProb = new NullProblem();
    if (newProb->Init()) {
        mProblems.push_back(newProb);
        mSolutions[mProblems.back()->GetName()].push_back(new NullSolution());
    } else {
        SafeDelete(newProb);
        console::error("Unable to create the Null Problem--exiting.");
    }

    // DynamicStreaming
    // UntexturedObjects
    newProb = new UntexturedObjectsProblem();
    if (newProb->Init()) {
        mProblems.push_back(newProb);
        mSolutions[mProblems.back()->GetName()].push_back(new UntexturedObjectsGLBindless());
    }
    else {
        SafeDelete(newProb);
    }
    // Textured Quads
}

// --------------------------------------------------------------------------------------------------------------------
ProblemFactory::~ProblemFactory()
{
    for (auto probIt = mSolutions.begin(); probIt != mSolutions.end(); ++probIt) {
        for (auto solIt = probIt->second.begin(); solIt != probIt->second.end(); ++solIt) {
            SafeDelete(*solIt);
        }
    }

    for (auto probIt = mProblems.begin(); probIt != mProblems.end(); ++probIt) {
        SafeDelete(*probIt);
    }
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
