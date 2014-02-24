#pragma once

class GfxBaseApi;
class Problem;
class Solution;

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// The temptation to call this ProblemFactory "Java" was *nearly* overwhelming.
class ProblemFactory
{
public:
    ProblemFactory(bool _skipInit);
    ~ProblemFactory();

    std::vector<Problem*> GetProblems();
    std::vector<Solution*> GetSolutions(Problem* _problem);

private:
    std::vector<Problem*> mProblems;
    std::map<std::string, std::vector<Solution*> > mSolutions;
};
