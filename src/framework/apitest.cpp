#include "pch.h"

#include "console.h"
#include "factory.h"
#include "gfx.h"
#include "problems/problem.h"
#include "solutions/solution.h"
#include "os.h"
#include "timer.h"
#include "wgl.h"

#define NAME_OPENGLGENERIC      "OpenGL (Generic)"
#define NAME_DIRECT3D11         "Direct3D 11"

#ifdef _WIN32
#   pragma comment(lib, "imm32.lib")
#   pragma comment(lib, "version.lib")
#   pragma comment(lib, "winmm.lib")
#else
    // This is not supported on !Windows.
    GfxBaseApi *CreateGfxDirect3D11()       { return nullptr; }
#endif

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
class ApplicationState
{
public:
    ApplicationState(GfxBaseApi* _activeApi)
    : mActiveProblem()
    , mActiveSolution()
    , mActiveApi(_activeApi)
    {
        mProblems = mFactory.GetProblems();
        assert(GetProblemCount() > 0);

        setProblemByIndex(0);
    }

    ~ApplicationState()
    {
        mActiveProblem->SetSolution(nullptr);
        mActiveProblem->Shutdown();

        mActiveSolution = nullptr;
        mActiveProblem = nullptr;
    }

    Problem* GetActiveProblem() const { return mActiveProblem; }

    size_t GetProblemCount() const { return mProblems.size(); }

    void NextProblem() 
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

    void PrevProblem() 
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

    size_t GetSolutionCount() const { return mSolutions.size(); }

    void NextSolution() 
    { 
        assert(mActiveProblem);
        Solution* prevSolution = mActiveSolution;
        

        mActiveSolution = setNextSolution(mSolutions.begin(), mSolutions.end(), prevSolution, true);
        if (!mActiveSolution) {
            mActiveSolution = setNextSolution(mSolutions.begin(), mSolutions.end(), prevSolution, false);
        }

        onSetProblemOrSolution();
    }

    void PrevSolution() 
    {
        assert(mActiveProblem);
        Solution* prevSolution = mActiveSolution;

        mActiveSolution = setNextSolution(mSolutions.rbegin(), mSolutions.rend(), prevSolution, true);
        if (!mActiveSolution) {
            mActiveSolution = setNextSolution(mSolutions.rbegin(), mSolutions.rend(), prevSolution, false);
        }

        onSetProblemOrSolution();
    }
    
    void SetActiveApi(GfxBaseApi* _activeApi)
    {
        mActiveApi = _activeApi;
    }

    void BenchmarkAllProblemsAndSolutions();
    void BenchmarkThisProblem();

    void Update()
    {

    }

private:
    void setProblemByIndex(size_t _index)
    {
        // Turn off the existing solution, if running one.
        if (mActiveProblem) {
            mActiveProblem->SetSolution(nullptr);
        }

        // null it out, we're not running one now.
        mActiveSolution = nullptr;
        
        // Set the new active problem
        mActiveProblem = mProblems[_index];

        // And init it.
        mActiveProblem->Init();

        // Get the list of possible solutions.
        mSolutions = mFactory.GetSolutions(mActiveProblem);

        // And set one as active
        mActiveSolution = findFirstValidSolution();

        onSetProblemOrSolution();
    }

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
            } else {
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

    Solution* findFirstValidSolution()
    {
        for (auto it = mSolutions.begin(); it != mSolutions.end(); ++it) {
            if (mActiveProblem->SetSolution(*it)) {
                return *it;
            }
        }

        return nullptr;
    }

    void onSetProblemOrSolution()
    {
        assert(mActiveApi);
        mActiveApi->OnProblemOrSolutionSet(mActiveProblem->GetName(), mActiveSolution ? mActiveSolution->GetName() : std::string(""));

        if (mActiveSolution) {
            size_t width = mActiveApi->GetWidth();
            size_t height = mActiveApi->GetHeight();
            Matrix proj = matrix_perspective_rh_gl(radians(45.0f), (float)width / (float)height, 0.1f, 10000.0f);

            mActiveSolution->SetProjectionMatrix(proj);
        }
    }

    ProblemFactory mFactory;

    std::vector<Problem*> mProblems;
    std::vector<Solution*> mSolutions;

    Problem* mActiveProblem;
    Solution* mActiveSolution;

    GfxBaseApi* mActiveApi;
};

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
std::map<std::string, GfxBaseApi*> CreateGfxApis()
{
    std::map<std::string, GfxBaseApi*> retMap;
    GfxBaseApi* tmpApi = nullptr;
    
    tmpApi = CreateGfxOpenGLGeneric();
    if (tmpApi) { 
        if (tmpApi->Init("apitest - OpenGL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768)) {
            retMap[NAME_OPENGLGENERIC] = tmpApi;
        } else {
            SafeDelete(tmpApi);
        }
    }

    tmpApi = CreateGfxDirect3D11();
    if (tmpApi) { 
        if (tmpApi->Init("apitest - Direct3D11", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768)) {
            retMap[NAME_DIRECT3D11] = tmpApi;
        }
        else {
            SafeDelete(tmpApi);
        }
    }

    return retMap;
}

// ------------------------------------------------------------------------------------------------
void DestroyGfxApis(std::map<std::string, GfxBaseApi*>* _apis)
{
    assert(_apis);
    for (auto it = _apis->begin(); it != _apis->end(); ++it) {
        SafeDelete(it->second);
    }

    _apis->clear();
}

// --------------------------------------------------------------------------------------------------------------------
void PostQuitEvent()
{
    SDL_Event quitEvent;
    quitEvent.type = SDL_QUIT;
    SDL_PushEvent(&quitEvent);
}

// --------------------------------------------------------------------------------------------------------------------
void OnEvent(SDL_Event* _event, ApplicationState* _appState)
{
    assert(_event);

    switch (_event->type)
    {
        case SDL_WINDOWEVENT:
        {
            switch (_event->window.event) 
            {
                // TODO: Need to deal with moving (to keep windows together).
                // TODO: Need to deal with resizing (to keep windows together, and to resize BB).
                case SDL_WINDOWEVENT_CLOSE:
                {
                    PostQuitEvent();
                    break;

                }

                default:
                {
                    break;
                }
            }
            break;
        }

        case SDL_KEYUP:
        {
            switch (_event->key.keysym.sym)
            {
            case SDLK_F4:
                #ifdef _WIN32
                    if (_event->key.keysym.mod & KMOD_ALT) {
                        PostQuitEvent();
                        break;
                    }
                #endif
                break;

            case SDLK_LEFT:
                _appState->PrevProblem();
                break;

            case SDLK_RIGHT:
                _appState->NextProblem();
                break;

            case SDLK_UP:
                _appState->PrevSolution();
                break;

            case SDLK_DOWN:
                _appState->NextSolution();
                break;

            default:
                break;
            }

            break;
        }
    };
}

// ------------------------------------------------------------------------------------------------
static void update_fps()
{
    static int s_frame_count;
    static unsigned long long s_last_frame_time;

    ++s_frame_count;
    unsigned long long now = timer::Read();
    double dt = timer::ToSec(now - s_last_frame_time);
    if (dt >= 1.0)
    {
        console::log("FPS: %g", s_frame_count / dt);
        s_frame_count = 0;
        s_last_frame_time = now;
    }
}

// ------------------------------------------------------------------------------------------------
static void Render(Problem* _activeProblem, GfxBaseApi* _activeApi)
{
    assert(_activeProblem);
    assert(_activeApi);
    
    Vec4 clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat clearDepth = 1.0f;
    _activeProblem->GetClearValues(&clearColor, &clearDepth);

    _activeApi->Clear(clearColor, clearDepth);

    // This is the main entry point shared by all tests. 
    _activeProblem->Render();
    
    // Present the results.
    _activeApi->SwapBuffers();
    
    update_fps();
}

static bool InitSDL()
{
    return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) >= 0;
}

// ------------------------------------------------------------------------------------------------
static bool Init(char* _exeName)
{
    // This forces the working directory to the directory the executable is in. This is necessary
    // to deal with people running from the wrong place (or debuggers).
    std::string dirName = os::path::dirname(_exeName);
    os::chdir(dirName);    

    if (!InitSDL())
    {
        console::error("SDL Error at Initialize: '%s'.\nUnable to initialize SDL -- required -- so exiting.", SDL_GetError());
        return false;
    }

    if (!timer::Init())
    {
        console::error("Unable to initialize timer facilities -- required -- so exiting.");
        return false;
    }

    return true;
}

// ------------------------------------------------------------------------------------------------
static void Cleanup()
{
    SDL_Quit();
}

#include "framework/gfx_gl.h"

// ------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    if (argc == 0) {
        console::error("Cannot tell where the executable is, exiting.");
    }

    if (!Init(argv[0])) {
        // Technically shouldn't get here--error should exit() if called, and all false cases
        // should emit a message as to why they are exiting. But better safe than sorry.
        return -1;
    }

    auto allGfxApis = CreateGfxApis();
    if (allGfxApis.find(NAME_OPENGLGENERIC) == allGfxApis.cend()) {
        console::error("Unable to create at least an OpenGL renderer--exiting");
    }

    GfxBaseApi* activeApi = allGfxApis[NAME_OPENGLGENERIC];
    activeApi->Activate();

    // TODO: Move all Api management into ApplicationState
    ApplicationState* app = new ApplicationState(activeApi);

    for (;;) {
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event)) {
            if (sdl_event.type == SDL_QUIT) {
                break;
            }

            OnEvent(&sdl_event, app);
        } else {
            Render(app->GetActiveProblem(), activeApi);
        }
    }

    SafeDelete(app);
    activeApi->Deactivate();
    DestroyGfxApis(&allGfxApis);
    Cleanup();

    return 0;
}

#if 0
#endif