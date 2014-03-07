#pragma once

#include "factory.h"
#include "gfx.h"
#include "options.h"
#include "problems/problem.h"
#include "solutions/solution.h"

class Solution;

typedef std::map<std::pair<std::string, std::string>, std::tuple<unsigned int, double, unsigned int>> BenchmarkResults;

const int kInactiveProblem = -1;
const int kInactiveSolution = -1;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class ApplicationState
{
public:
    ApplicationState(const Options& _opts);
    ~ApplicationState();

    inline Problem* GetActiveProblem() const { return getProblem(mActiveProblem); }
    inline Solution* GetActiveSolution() const { return getSolution(mActiveSolution); }
    inline GfxBaseApi* GetActiveApi() const { return mActiveApi; }

    inline size_t GetProblemCount() const { return mProblems.size(); }
    inline size_t GetSolutionCount() const { return mSolutions.size(); }
    inline size_t GetActiveApiCount() const { return mGfxApis.size(); }

    void NextProblem();
    void PrevProblem();

    void NextSolution();
    void PrevSolution();

    void NextAPI();

    void Update();

    bool IsBenchmarkMode() const { return mBenchmarkMode; }
    bool IsBenchmarkModeComplete() const;

    BenchmarkResults GetBenchmarkResults() const;

    void BroadcastToOtherWindows(SDL_Event* _event);

private:
    inline Problem* getProblem(int _index) const { return _index != kInactiveProblem ? mProblems[_index] : nullptr; }
    inline Solution* getSolution(int _index) const { return _index != kInactiveSolution ? mSolutions[_index] : nullptr; }

    void setInitialProblemAndSolution(const std::string _probName, const std::string _solnName);
    int findProblemWithSolution(const std::string _solnName, int* _outSolution);

    void changeProblem(int _offset);
    void changeSolution(int _offset);

    void onSetProblemOrSolution();
    void updateFPS();
    void resetTimer();

    void createGfxApis();
    void destroyGfxApis();

    // ----------------------------------------------------------------------------------------------------------------
    std::map<std::string, GfxBaseApi*> mGfxApis;

    ProblemFactory mFactory;

    std::vector<Problem*> mProblems;
    std::vector<Solution*> mSolutions;

    int mActiveProblem;
    int mActiveSolution;

    GfxBaseApi* mActiveApi;

    unsigned int mFrameCount;
    unsigned long long mTimerStart;

    float mBenchmarkTime;
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
