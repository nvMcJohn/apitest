#pragma once

#include "factory.h"
#include "gfx.h"
#include "options.h"
#include "problems/problem.h"
#include "solutions/solution.h"

class Solution;

typedef std::map<std::pair<std::string, std::string>, std::pair<unsigned int, double>> BenchmarkResults;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class ApplicationState
{
public:
    ApplicationState(GfxBaseApi* _activeApi, const Options& _opts);
    ~ApplicationState();

    inline Problem* GetActiveProblem() const { return mActiveProblem; }
    inline size_t GetProblemCount() const { return mProblems.size(); }
    inline size_t GetSolutionCount() const { return mSolutions.size(); }

    void NextProblem();
    void PrevProblem();

    void NextSolution();
    void PrevSolution();

    void SetActiveApi(GfxBaseApi* _activeApi);
    void Update();

    bool IsBenchmarkMode() const { return mBenchmarkMode; }
    bool IsBenchmarkModeComplete() const;

    BenchmarkResults GetBenchmarkResults() const;

private:
    void setInitialProblemAndSolution(const std::string _prob, const std::string _soln);
    Problem* findProblemWithSolution(const std::string _soln, Solution** _outSolution);

    template<typename ItType>
    Problem* setNextProblem(ItType _beginIt, ItType _endIt, Problem* _curProblem, bool _after)
    {
        bool processing = !_after;
        for (auto it = _beginIt; it != _endIt; ++it) {
            if (*it == _curProblem) {
                processing = !processing;
                continue;
            }

            if (!processing) {
                continue;
            }

            if ((*it)->Init()) {
                return *it;
            }
            else {
                (*it)->Shutdown();
            }
        }

        return nullptr;
    }

    template<typename ItType>
    Solution* setNextSolution(ItType _beginIt, ItType _endIt, Solution* _curProblem, bool _after)
    {
        bool processing = !_after;
        for (auto it = _beginIt; it != _endIt; ++it) {
            if (*it == _curProblem) {
                processing = !processing;
                continue;
            }

            if (!processing) {
                continue;
            }

            if (mActiveProblem->SetSolution(*it)) {
                return *it;
            }
        }

        return nullptr;
    }

    Solution* findFirstValidSolution();
    void onSetProblemOrSolution();
    void updateFPS();
    void resetTimer();

    // ----------------------------------------------------------------------------------------------------------------
    ProblemFactory mFactory;

    std::vector<Problem*> mProblems;
    std::vector<Solution*> mSolutions;

    Problem* mActiveProblem;
    Solution* mActiveSolution;

    GfxBaseApi* mActiveApi;

    unsigned int mFrameCount;
    unsigned long long mTimerStart;

    bool mBenchmarkMode;
    struct BenchmarkState {
        size_t mProblemsBenchmarked;
        size_t mSolutionsBenchmarked;
        unsigned long long mBenchmarkTimeStart;
        bool mBenchmarkSingle;

        BenchmarkState();

        BenchmarkResults mBenchmarkTimings;
    } mBenchmarkState;
};
