#include "pch.h"

#include "appstate.h"
#include "timer.h"

const double kBenchmarkTime = 5.0;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
ApplicationState::BenchmarkState::BenchmarkState()
: mProblemsBenchmarked()
, mSolutionsBenchmarked()
, mBenchmarkTimeStart()
, mBenchmarkSingle()
{ }

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
ApplicationState::ApplicationState(GfxBaseApi* _activeApi, const Options& _opts)
: mActiveProblem()
, mActiveSolution()
, mActiveApi(_activeApi)
, mFrameCount()
, mBenchmarkMode(_opts.BenchmarkMode)
, mFactory(false)
{
    mProblems = mFactory.GetProblems();
    assert(GetProblemCount() > 0);

    setInitialProblemAndSolution(_opts.InitialProblem, _opts.InitialSolution);

    // This logic currently means you cannot benchmark the NullProblem/NullSolution
    mBenchmarkState.mBenchmarkSingle = mBenchmarkMode
                                    && (_opts.InitialProblem != _opts.DefaultInitialProblem || _opts.InitialSolution != _opts.DefaultInitialSolution);

    resetTimer();

    if (mBenchmarkMode) {
        mBenchmarkState.mBenchmarkTimeStart = timer::Read();
    }
}

// --------------------------------------------------------------------------------------------------------------------
ApplicationState::~ApplicationState()
{
    mActiveProblem->SetSolution(nullptr);
    mActiveProblem->Shutdown();

    mActiveSolution = nullptr;
    mActiveProblem = nullptr;
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::NextProblem()
{
    assert(mActiveProblem);
    Problem* prevProblem = mActiveProblem;
    mActiveProblem->SetSolution(nullptr);
    mActiveSolution = nullptr;

    mActiveProblem = setNextProblem(mProblems.begin(), mProblems.end(), prevProblem, true);
    if (!mActiveProblem) {
        mActiveProblem = setNextProblem(mProblems.begin(), mProblems.end(), prevProblem, false);
    }

    // The previous logic also breaks if the prevProblem and mActiveProblem are the same (double call to Init).
    assert(prevProblem != mActiveProblem);
    prevProblem->Shutdown();

    // Must have a valid problem now.
    assert(mActiveProblem);

    // Get the list of possible solutions.
    mSolutions = mFactory.GetSolutions(mActiveProblem);

    // And set one as active
    mActiveSolution = findFirstValidSolution();

    onSetProblemOrSolution();
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::PrevProblem()
{
    assert(mActiveProblem);
    Problem* prevProblem = mActiveProblem;
    mActiveProblem->SetSolution(nullptr);
    mActiveSolution = nullptr;

    mActiveProblem = setNextProblem(mProblems.rbegin(), mProblems.rend(), prevProblem, true);
    if (!mActiveProblem) {
        mActiveProblem = setNextProblem(mProblems.rbegin(), mProblems.rend(), prevProblem, false);
    }

    // The previous logic also breaks if the prevProblem and mActiveProblem are the same (double call to Init).
    assert(prevProblem != mActiveProblem);
    prevProblem->Shutdown();

    // Must have a valid problem now.
    assert(mActiveProblem);

    // Get the list of possible solutions.
    mSolutions = mFactory.GetSolutions(mActiveProblem);

    // And set one as active
    mActiveSolution = findFirstValidSolution();

    onSetProblemOrSolution();
}


// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::NextSolution()
{
    assert(mActiveProblem);
    Solution* prevSolution = mActiveSolution;


    mActiveSolution = setNextSolution(mSolutions.begin(), mSolutions.end(), prevSolution, true);
    if (!mActiveSolution) {
        mActiveSolution = setNextSolution(mSolutions.begin(), mSolutions.end(), prevSolution, false);
    }

    onSetProblemOrSolution();
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::PrevSolution()
{
    assert(mActiveProblem);
    Solution* prevSolution = mActiveSolution;

    mActiveSolution = setNextSolution(mSolutions.rbegin(), mSolutions.rend(), prevSolution, true);
    if (!mActiveSolution) {
        mActiveSolution = setNextSolution(mSolutions.rbegin(), mSolutions.rend(), prevSolution, false);
    }

    onSetProblemOrSolution();
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::SetActiveApi(GfxBaseApi* _activeApi)
{
    mActiveApi = _activeApi;
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::Update()
{
    if (mBenchmarkMode) {
        ++mFrameCount;
        double elapsed = timer::ToSec(timer::Read() - mBenchmarkState.mBenchmarkTimeStart);
        
        if (elapsed >= kBenchmarkTime) {
            ++mBenchmarkState.mSolutionsBenchmarked;

            mBenchmarkState.mBenchmarkTimings[std::make_pair(mActiveProblem->GetName(), mActiveSolution->GetName())] 
                                            = std::make_pair(mFrameCount, elapsed);
            
            if (mBenchmarkState.mBenchmarkSingle) {
                return;
            }

            if (mBenchmarkState.mSolutionsBenchmarked >= GetSolutionCount()) {
                ++mBenchmarkState.mProblemsBenchmarked;

                if (IsBenchmarkModeComplete()) {
                    return;
                } else {
                    mBenchmarkState.mSolutionsBenchmarked = 0;
                    
                    NextProblem();
                    mBenchmarkState.mBenchmarkTimeStart = timer::Read();
                }
            } else {
                NextSolution();
                mBenchmarkState.mBenchmarkTimeStart = timer::Read();
            }
        }
    } else {
        updateFPS();
    }
}

// --------------------------------------------------------------------------------------------------------------------
bool ApplicationState::IsBenchmarkModeComplete() const
{
    return mBenchmarkState.mProblemsBenchmarked >= mProblems.size()
        || (mBenchmarkState.mBenchmarkSingle && mBenchmarkState.mSolutionsBenchmarked >= 1);
}

// --------------------------------------------------------------------------------------------------------------------
BenchmarkResults ApplicationState::GetBenchmarkResults() const
{
    return mBenchmarkState.mBenchmarkTimings;
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::setInitialProblemAndSolution(const std::string _prob, const std::string _soln)
{
    // TODO: This should be cleaned up. It's error prone.
    assert(mActiveProblem == nullptr);
    if (!_prob.empty()) {
        for (auto it = mProblems.begin(); it != mProblems.end(); ++it) {
            if ((*it)->GetName() == _prob) {
                mActiveProblem = *it;
                break;
            }
        }

        if (mActiveProblem == nullptr) {
            console::error("Couldn't locate problem named '%s'. Run with -h to see all valid problem names.", _prob.c_str());
        }

        if (!mActiveProblem->Init()) {
            console::error("Failed to initialize problem '%s', exiting.", _prob.c_str());
        }

        // We set the problem, now try to find the solution.
        mSolutions = mFactory.GetSolutions(mActiveProblem);
        if (!_soln.empty()) {
            for (auto it = mSolutions.begin(); it != mSolutions.end(); ++it) {
                if ((*it)->GetName() == _soln) {
                    mActiveSolution = *it;
                    if (!mActiveProblem->SetSolution(mActiveSolution)) {
                        console::error("Unable to initialize solution '%s', exiting.", _soln.c_str());
                    }
                    onSetProblemOrSolution();
                    break;
                }
            }
        } else {
            mActiveSolution = findFirstValidSolution();
            onSetProblemOrSolution();
        }

        if (mActiveSolution == nullptr) {
            console::error("Couldn't locate solution named '%s' for problem '%s'. Run with -h to see all valid problem and solution names.", _soln.c_str(), _prob.c_str());
        }
    } else if (!_soln.empty()) {
        Solution* solution = nullptr;
        mActiveProblem = findProblemWithSolution(_soln, &solution);

        if (solution == nullptr) {
            console::error("Unable to find solution '%s'. Run with -h to see all valid solution names.", _soln.c_str());
        }

        if (mActiveProblem == nullptr) {
            console::error("Couldn't locate problem that had solution '%s'", _soln.c_str());
        }

        if (!mActiveProblem->Init()) {
            console::error("Failed to initialize problem '%s', exiting.", _prob.c_str());
        }

        mSolutions = mFactory.GetSolutions(mActiveProblem);
        mActiveSolution = solution;
        if (!mActiveProblem->SetSolution(mActiveSolution)) {
            console::error("Unable to initialize solution '%s', exiting.", _soln.c_str());
        }

        onSetProblemOrSolution();

        assert(mActiveProblem && mActiveSolution);
    } else {
        console::error("You went through some effort to specify a blank initial problem and initial solution.\n"
                       "Congratulations, that doesn't work."
        );
    }
}

// --------------------------------------------------------------------------------------------------------------------
Problem* ApplicationState::findProblemWithSolution(const std::string _soln, Solution** _outSolution)
{
    for (auto probIt = mProblems.cbegin(); probIt != mProblems.cend(); ++probIt) {
        auto allSolutions = mFactory.GetSolutions(*probIt);
        for (auto solnIt = allSolutions.cbegin(); solnIt != allSolutions.cend(); ++solnIt) {
            if ((*solnIt)->GetName() == _soln) {
                if (_outSolution) {
                    (*_outSolution) = (*solnIt);
                }

                return (*probIt);
            }
        }
    }

    return nullptr;
}


// --------------------------------------------------------------------------------------------------------------------
Solution* ApplicationState::findFirstValidSolution()
{
    for (auto it = mSolutions.begin(); it != mSolutions.end(); ++it) {
        if (mActiveProblem->SetSolution(*it)) {
            return *it;
        }
    }

    return nullptr;
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::onSetProblemOrSolution()
{
    assert(mActiveApi);
    mActiveApi->OnProblemOrSolutionSet(mActiveProblem->GetName(), mActiveSolution ? mActiveSolution->GetName() : std::string(""));

    if (mActiveSolution) {
        size_t width = mActiveApi->GetWidth();
        size_t height = mActiveApi->GetHeight();
        mActiveSolution->SetSize(width, height);
    }

    resetTimer();
}

// ------------------------------------------------------------------------------------------------
void ApplicationState::updateFPS()
{
    ++mFrameCount;
    unsigned long long now = timer::Read();
    double dt = timer::ToSec(now - mTimerStart);
    if (dt >= 1.0) {
        console::log("FPS: %g", mFrameCount / dt);
        mFrameCount = 0;
        mTimerStart = now;
    }
}

// ------------------------------------------------------------------------------------------------
void ApplicationState::resetTimer()
{
    mFrameCount = 0;
    mTimerStart = timer::Read();
}
