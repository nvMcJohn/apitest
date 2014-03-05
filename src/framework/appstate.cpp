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
ApplicationState::ApplicationState(const Options& _opts)
: mActiveProblem(kInactiveProblem)
, mActiveSolution(kInactiveSolution)
, mActiveApi()
, mFrameCount()
, mBenchmarkMode(_opts.BenchmarkMode)
, mFactory(false)
{
    createGfxApis();
    mActiveApi = mGfxApis[_opts.InitialApi];
    if (mActiveApi) {
        mActiveApi->Activate();
    } else {
        console::error("Failed to select api with name '%s', run with -h to see a list of all APIs.");
    }

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
    Problem* prob = GetActiveProblem();
    if (prob) {
        prob->Shutdown();
    }

    mActiveSolution = kInactiveSolution;
    mActiveProblem = kInactiveProblem;

    destroyGfxApis();
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::NextProblem()
{
    changeProblem(1);
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::PrevProblem()
{
    changeProblem(-1);
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::NextSolution()
{
    changeSolution(1);
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::PrevSolution()
{
    changeSolution(-1);
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::NextAPI()
{
    assert(mActiveApi);
    assert(mGfxApis.size() > 0);

    // Don't do any of this if we don't have another API to move to.
    if (mGfxApis.size() == 1) {
        return;
    }

    // Shutdown the problem for now, we'll bring it back up in a minute.
    Problem* activeProb = GetActiveProblem();
    if (activeProb) {
        activeProb->SetSolution(nullptr);
        mActiveSolution = kInactiveSolution;
        activeProb->Shutdown();
        // Don't actually clear the active problem here, because we're going to try 
        // to set it back up shortly.
    }

    auto it = mGfxApis.find(mActiveApi->GetShortName());
    assert(it != mGfxApis.end());
    ++it;

    // In case this is the last one, wrap.
    if (it == mGfxApis.end()) {
        it = mGfxApis.begin();
    }

    // Activate the new before deactivating the old to avoid the window disappearing problem.
    it->second->Activate();
    mActiveApi->Deactivate();

    mActiveApi = it->second;

    // Try to select the same problem again.
    changeProblem(0);
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::Update()
{
    if (mBenchmarkMode) {
        ++mFrameCount;
        double elapsed = timer::ToSec(timer::Read() - mBenchmarkState.mBenchmarkTimeStart);
        
        if (elapsed >= kBenchmarkTime) {
            ++mBenchmarkState.mSolutionsBenchmarked;

            mBenchmarkState.mBenchmarkTimings[std::make_pair(GetActiveProblem()->GetName(), GetActiveSolution()->GetName())]
                                            = std::make_pair(mFrameCount, elapsed);
            
            if (mBenchmarkState.mBenchmarkSingle) {
                Problem* prob = GetActiveProblem();
                if (prob) {
                    prob->Shutdown();
                }

                return;
            }

            if (mBenchmarkState.mSolutionsBenchmarked >= GetSolutionCount()) {
                ++mBenchmarkState.mProblemsBenchmarked;

                if (IsBenchmarkModeComplete()) {
                    Problem* prob = GetActiveProblem();
                    if (prob) {
                        prob->Shutdown();
                    }

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
void ApplicationState::BroadcastToOtherWindows(SDL_Event* _event)
{
    // TODO: Add any other messages as necessary to keep the windows in sync.
    assert(_event->type == SDL_WINDOWEVENT 
        && (_event->window.event == SDL_WINDOWEVENT_MOVED));

    for (auto it = mGfxApis.begin(); it != mGfxApis.end(); ++it) {
        if (it->second == mActiveApi) {
            continue;
        }

        it->second->MoveWindow(_event->window.data1, _event->window.data2);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::setInitialProblemAndSolution(const std::string _probName, const std::string _solnName)
{
    // TODO: This should be cleaned up. It's error prone.
    assert(mActiveProblem == kInactiveProblem);
    if (!_probName.empty()) {
        int i = 0;
        for (auto it = mProblems.begin(); it != mProblems.end(); ++it, ++i) {
            if ((*it)->GetName() == _probName) {
                mActiveProblem = i;
                break;
            }
        }

        if (mActiveProblem == kInactiveProblem) {
            console::error("Couldn't locate problem named '%s'. Run with -h to see all valid problem names.", _probName.c_str());
        }

        if (!GetActiveProblem()->Init()) {
            console::error("Failed to initialize problem '%s', exiting.", _probName.c_str());
        }

        // We set the problem, now try to find the solution.
        mSolutions = mFactory.GetSolutions(GetActiveProblem(), mActiveApi);
        if (!_solnName.empty()) {
            int i = 0;
            for (auto it = mSolutions.begin(); it != mSolutions.end(); ++it, ++i) {
                if ((*it)->GetName() == _solnName) {
                    mActiveSolution = i;
                    if (!GetActiveProblem()->SetSolution(GetActiveSolution())) {
                        console::error("Unable to initialize solution '%s', exiting.", _solnName.c_str());
                    }

                    onSetProblemOrSolution();
                    break;
                }
            }
        } else {
            changeSolution(1);
            if (mActiveSolution == kInactiveSolution) {
                console::error("Unable to initialize any solutions for '%s', exiting.", _probName.c_str());
            }
            onSetProblemOrSolution();
        }

        if (mActiveSolution == kInactiveSolution) {
            console::error("Couldn't locate solution named '%s' for problem '%s'. Run with -h to see all valid problem and solution names.", _solnName.c_str(), _probName.c_str());
        }
    } else if (!_solnName.empty()) {
        int solution = kInactiveSolution;
        mActiveProblem = findProblemWithSolution(_solnName, &solution);

        if (solution == kInactiveSolution) {
            console::error("Unable to find solution '%s'. Run with -h to see all valid solution names.", _solnName.c_str());
        }

        if (mActiveProblem == kInactiveProblem) {
            console::error("Couldn't locate problem that had solution '%s'", _solnName.c_str());
        }

        if (!GetActiveProblem()->Init()) {
            console::error("Failed to initialize problem '%s', exiting.", _probName.c_str());
        }

        mSolutions = mFactory.GetSolutions(GetActiveProblem(), mActiveApi);
        mActiveSolution = solution;
        if (!GetActiveProblem()->SetSolution(GetActiveSolution())) {
            console::error("Unable to initialize solution '%s', exiting.", _solnName.c_str());
        }

        onSetProblemOrSolution();

        assert(mActiveProblem >= 0 && mActiveSolution >= 0);
    } else {
        console::error("You went through some effort to specify a blank initial problem and initial solution.\n"
                       "Congratulations, that doesn't work."
        );
    }
}

// --------------------------------------------------------------------------------------------------------------------
int ApplicationState::findProblemWithSolution(const std::string _solnName, int* _outSolution)
{
    int probi = 0;
    for (auto probIt = mProblems.cbegin(); probIt != mProblems.cend(); ++probIt, ++probi) {
        int solni = 0;
        auto allSolutions = mFactory.GetSolutions(*probIt, mActiveApi);
        for (auto solnIt = allSolutions.cbegin(); solnIt != allSolutions.cend(); ++solnIt, ++solni) {
            if ((*solnIt)->GetName() == _solnName) {
                if (_outSolution) {
                    (*_outSolution) = solni;
                }

                return probi;
            }
        }
    }

    return kInactiveProblem;
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::changeProblem(int _offset)
{
    int prevProblem = mActiveProblem;
    if (_offset != 0) {
        Problem* prevProblemPtr = GetActiveProblem();
        if (prevProblemPtr) {
            prevProblemPtr->SetSolution(nullptr);
            mActiveSolution = kInactiveSolution;
            prevProblemPtr->Shutdown();
            mActiveProblem = kInactiveProblem;
        }
    }

    const size_t problemCount = mProblems.size();

    // If we are going backwards, we need to pretend we have a valid problem picked already, or we won't pick the last
    // problem on the way back through.
    int curProbIndex = (_offset >= 0) ? prevProblem : std::max(0, prevProblem);
    int newProblem = (curProbIndex + problemCount + _offset) % problemCount;

    // If the offset was 0 (because we changed gfx apis) change it to 1 in case this problem is inapplicable to this
    // api and we need to look for the next one.
    if (_offset == 0) {
        _offset = 1;
    }

    for (unsigned int u = 0; u < problemCount; ++u) {
        Problem* newProb = getProblem(newProblem);
        if (newProb->Init()) {
            mActiveProblem = newProblem;
            mSolutions = mFactory.GetSolutions(newProb, mActiveApi);

            changeSolution(1);
            if (mActiveSolution != kInactiveSolution) {
                break;
            }

            // Otherwise we failed, continue along the way.
            newProb->Shutdown();
            if (mBenchmarkMode) {
                ++mBenchmarkState.mProblemsBenchmarked;
            }
        }
        
        newProblem = (newProblem + problemCount + _offset) % problemCount;
    }

    onSetProblemOrSolution();
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::changeSolution(int _offset)
{
    Problem* activeProb = GetActiveProblem();
    assert(activeProb != nullptr);

    const size_t solutionCount = mSolutions.size();
    if (solutionCount == 0) {
        return;
    }

    int prevSolution = mActiveSolution;
    activeProb->SetSolution(nullptr);
    mActiveSolution = kInactiveSolution;

    // If we are going backwards, we need to pretend we have a valid problem picked already, or we won't pick the last
    // problem on the way back through.
    int curSolnIndex = (_offset >= 0) ? prevSolution : std::max(0, prevSolution);

    int newSolution = (curSolnIndex + solutionCount + _offset) % solutionCount;
    for (unsigned int u = 0; u < solutionCount; ++u) {
        Solution* newSoln = getSolution(newSolution);
        if (activeProb->SetSolution(newSoln)) {
            mActiveSolution = newSolution;
            break;
        } else if (mBenchmarkMode) {
            mBenchmarkState.mBenchmarkTimings[std::make_pair(GetActiveProblem()->GetName(), newSoln->GetName())]
                                            = std::make_pair<unsigned int, double>(0, 0.0);
            ++mBenchmarkState.mSolutionsBenchmarked;
        }

        newSolution = (newSolution + solutionCount + _offset) % solutionCount;
    }

    onSetProblemOrSolution();
}

// --------------------------------------------------------------------------------------------------------------------
void ApplicationState::onSetProblemOrSolution()
{
    assert(mActiveApi);
    mActiveApi->OnProblemOrSolutionSet(GetActiveProblem()->GetName(), GetActiveSolution() ? GetActiveSolution()->GetName() : std::string(""));

    Solution* soln = GetActiveSolution();
    if (soln) {
        size_t width = mActiveApi->GetWidth();
        size_t height = mActiveApi->GetHeight();
        soln->SetSize(width, height);
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

// ------------------------------------------------------------------------------------------------
void ApplicationState::createGfxApis()
{
    assert(mGfxApis.size() == 0);

    GfxBaseApi* tmpApi = nullptr;
    
    tmpApi = CreateGfxOpenGLGeneric();
    if (tmpApi) { 
        if (tmpApi->Init("apitest - OpenGL (compat)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768)) {
            mGfxApis[tmpApi->GetShortName()] = tmpApi;
        } else {
            SafeDelete(tmpApi);
        }
    }

    tmpApi = CreateGfxDirect3D11();
    if (tmpApi) { 
        if (tmpApi->Init("apitest - Direct3D11", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768)) {
            mGfxApis[tmpApi->GetShortName()] = tmpApi;
        }
        else {
            SafeDelete(tmpApi);
        }
    }
}

// ------------------------------------------------------------------------------------------------
void ApplicationState::destroyGfxApis()
{
    for (auto it = mGfxApis.begin(); it != mGfxApis.end(); ++it) {
        SafeDelete(it->second);
    }

    mGfxApis.clear();
}
