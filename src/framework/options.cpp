#include "pch.h"

#include "options.h"
#include "factory.h"

#if WITH_D3D11
#   include "gfx_dx11.h"
#endif

#include "gfx_gl.h"
#include "problems/problem.h"
#include "solutions/solution.h"

const char* Options::DefaultInitialProblem = "NullProblem";
const char* Options::DefaultInitialSolution = "NullSolution";

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void PrintHelp();
void PrintHelp(int _exitCode);

// ------------------------------------------------------------------------------------------------
template <typename T>
void Dest(Options* _outOptions, size_t _byteOffset, T** _outDest)
{
    assert(_outDest);
    (*_outDest) = (T*)((uint8_t*)(_outOptions) + _byteOffset);
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
typedef int(*CommandLineProcessFunc)(int _argNum, int _argc, char* _argv[], Options* _outOptions, size_t _storageOffset);

// ------------------------------------------------------------------------------------------------
struct CommandLineOption
{
    const char* OptionShort;
    const char* OptionLong;
    CommandLineProcessFunc ProcessFunc;
    size_t DestByteOffset;
    const char* HelpText;
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
Options::Options()
: BenchmarkMode(false)
, BenchmarkTime(5.0f)
, Resolution(std::make_pair(1024, 768))
, InitialProblem(Options::DefaultInitialProblem)
, InitialSolution(Options::DefaultInitialSolution)
#if _OSX
    , InitialApi(GfxApiOpenGLCore::SGetShortName())
#else
    , InitialApi(GfxApiOpenGLGeneric::SGetShortName())
#endif
{

}

// ------------------------------------------------------------------------------------------------
Options::~Options()
{ }

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
int StoreTrue(int _argNum, int _argc, char* _argv[], Options* _outOptions, size_t _byteOffset)
{
    bool* dest; Dest(_outOptions, _byteOffset, &dest);
    (*dest) = true;
    return 1;
}

// ------------------------------------------------------------------------------------------------
int StoreFloat(int _argNum, int _argc, char* _argv[], Options* _outOptions, size_t _byteOffset)
{
    float* dest; Dest(_outOptions, _byteOffset, &dest);
    (*dest) = (float)atof(_argv[_argNum + 1]);
    return 2;
}

// ------------------------------------------------------------------------------------------------
int StoreString(int _argNum, int _argc, char* _argv[], Options* _outOptions, size_t _byteOffset)
{
    std::string* dest; Dest(_outOptions, _byteOffset, &dest);
    if (_argNum + 2 > _argc) {
        PrintHelp();
        console::error("Not enough parameters for argument '%s'", _argv[_argNum]);
    }

    (*dest) = std::string(_argv[_argNum + 1]);
    return 2;
}

// ------------------------------------------------------------------------------------------------
int StorePairInt(int _argNum, int _argc, char* _argv[], Options* _outOptions, size_t _byteOffset)
{
    std::pair<int, int>* dest; Dest(_outOptions, _byteOffset, &dest);
    if (_argNum + 3 > _argc) {
        PrintHelp();
        console::error("Not enough parameters for argument '%s'", _argv[_argNum]);
    }

    (*dest).first = atoi(_argv[_argNum + 1]);
    (*dest).second = atoi(_argv[_argNum + 2]);

    return 3;
}

// ------------------------------------------------------------------------------------------------
int PrintHelpAndExit(int _argNum, int _argc, char* _argv[], Options* _outOptions, size_t _byteOffset)
{
    PrintHelp(0);
    return 1;
}

// ------------------------------------------------------------------------------------------------
CommandLineOption gClOpts[] = {
    { "-b",     "--benchmark",      StoreTrue,          offsetof(Options, BenchmarkMode),   "Run in benchmark mode, then exit." },
    { "-t",     "--benchmarktime",  StoreFloat,         offsetof(Options, BenchmarkTime),   "Specify how long to run each solution for when running benchmarks." }, 
    { "-p",     "--problem",        StoreString,        offsetof(Options, InitialProblem),  "The next argument specifies the initial problem to use."},
    { "-s",     "--solution",       StoreString,        offsetof(Options, InitialSolution), "The next argument specifies the initial solution to show."},
    { "-a",     "--api",            StoreString,        offsetof(Options, InitialApi),      "The default API to use." },
    { "-r",     "--resolution",     StorePairInt,       offsetof(Options, Resolution),      "The horizontal and vertical surface resolution." },
    { "-h",     "--help",           PrintHelpAndExit,   size_t(0),                          "Display this help text and exit." },

    // Sentinel, must be last.
    { nullptr, nullptr, nullptr },
};

// ------------------------------------------------------------------------------------------------
Options ProcessCommandLine(int _argc, char* _argv[])
{
    Options retVal;

    for (int n = 1; n < _argc; ) {
        int oldN = n;
        for (int i = 0; gClOpts[i].OptionShort != nullptr; ++i) {
            if (strcmp(gClOpts[i].OptionShort, _argv[n]) == 0 || strcmp(gClOpts[i].OptionLong, _argv[n]) == 0) {
                n += gClOpts[i].ProcessFunc(n, _argc, _argv, &retVal, gClOpts[i].DestByteOffset);
                break;
            }
        }

        // Didn't understand a command line argument
        if (n == oldN) {
            PrintHelp();
            console::error("Unknown command line argument '%s' (or didn't understand previous arguments).", _argv[n]);
        }
    }


    // If we set the problem or solution but not both, empty out the other one.
    if (retVal.InitialProblem != retVal.DefaultInitialProblem && retVal.InitialSolution == retVal.DefaultInitialSolution) {
        retVal.InitialSolution = "";
    }

    if (retVal.InitialProblem == retVal.DefaultInitialProblem && retVal.InitialSolution != retVal.DefaultInitialSolution) {
        retVal.InitialProblem = "";
    }

    return retVal;
}

// ------------------------------------------------------------------------------------------------
void PrintHelp()
{
    console::log("apitest - a simple framework for testing various solutions to different rendering problems.");
    console::log("Usage");
    console::log("");
    console::log("  apitest [options]\n");
    console::log("");
    console::log("Options");
    for (int i = 0; gClOpts[i].OptionShort != nullptr; ++i) {
        console::log("  %s, %-22s %s", gClOpts[i].OptionShort, gClOpts[i].OptionLong, gClOpts[i].HelpText);
    }

    {
        const char* kTableEntry = "  %-27s%-20s";
        console::log("");
        console::log("Problems & Solutions");
        console::log(kTableEntry, "Problems", "Solutions");
        ProblemFactory problemsAndSolutions(true);
        auto allProblems = problemsAndSolutions.GetProblems();

        for (auto probIt = allProblems.cbegin(); probIt != allProblems.cend(); ++probIt) {
            console::log("");
            Problem* prob = (*probIt);
            // console::log(kTableFormat, prob->GetName().c_str(), "");

            auto solutions = problemsAndSolutions.GetSolutions(prob, nullptr);
            for (auto solnIt = solutions.cbegin(); solnIt != solutions.cend(); ++solnIt) {
                Solution* soln = (*solnIt);
                console::log(kTableEntry, prob->GetName().c_str(), soln->GetName().c_str());
            }
        }
        console::log("");
    }

    {
        const char* kTableEntry = "  %-12s%-20s";

        console::log("");
        console::log("APIs (use the short name on the command line)");
        console::log(kTableEntry, "Short Name", "Long Name");

        // This isn't ideal. But it'll work for now.
        console::log(kTableEntry, GfxApiOpenGLGeneric::SGetShortName(), GfxApiOpenGLGeneric::SGetLongName());
        console::log(kTableEntry, GfxApiOpenGLCore::SGetShortName(),    GfxApiOpenGLCore::SGetLongName());
        #if WITH_D3D11
            console::log(kTableEntry, GfxApiDirect3D11::SGetShortName(), GfxApiDirect3D11::SGetLongName());
        #endif
    }
}

// ------------------------------------------------------------------------------------------------
void PrintHelp(int _exitCode)
{
    PrintHelp();
    exit(_exitCode);
}
