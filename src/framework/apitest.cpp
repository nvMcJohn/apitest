#include "pch.h"

#include "appstate.h"
#include "console.h"
#include "factory.h"
#include "gfx.h"
#include "problems/problem.h"
#include "solutions/solution.h"
#include "options.h"
#include "os.h"
#include "timer.h"
#include "wgl.h"

#include <stdio.h>

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

#ifdef _WIN32
#   define snprintf _snprintf
#endif


std::string asTable(BenchmarkResults _results);

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
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

// --------------------------------------------------------------------------------------------------------------------
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
                if (!_appState->IsBenchmarkMode()) {
                    _appState->PrevProblem();
                }
                break;

            case SDLK_RIGHT:
                if (!_appState->IsBenchmarkMode()) {
                    _appState->NextProblem();
                }
                break;

            case SDLK_UP:
                if (!_appState->IsBenchmarkMode()) {
                    _appState->PrevSolution();
                }
                break;

            case SDLK_DOWN:
                if (!_appState->IsBenchmarkMode()) {
                    _appState->NextSolution();
                }
                break;

            default:
                break;
            }

            break;
        }
    };
}

// --------------------------------------------------------------------------------------------------------------------
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
}

// --------------------------------------------------------------------------------------------------------------------
static bool InitSDL()
{
    return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) >= 0;
}

// --------------------------------------------------------------------------------------------------------------------
static bool Init(const char* _exeName)
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

// --------------------------------------------------------------------------------------------------------------------
static void Cleanup()
{
    SDL_Quit();
}

#include "framework/gfx_gl.h"

// --------------------------------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    if (argc == 0) {
        console::error("Cannot tell where the executable is, exiting.");
    }

    Options opts = ProcessCommandLine(argc, argv);

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
    ApplicationState* app = new ApplicationState(activeApi, opts);

    bool shouldQuit = false;
    for (;;) {
        SDL_Event sdl_event;
        shouldQuit = shouldQuit || app->IsBenchmarkMode() && app->IsBenchmarkModeComplete();
        if (shouldQuit) {
            break;
        }

        if (SDL_PollEvent(&sdl_event)) {
            shouldQuit = shouldQuit || sdl_event.type == SDL_QUIT;
            OnEvent(&sdl_event, app);
        } else {
            app->Update();
            Render(app->GetActiveProblem(), activeApi);
        }
    }

    if (app->IsBenchmarkMode()) {
        console::log("\n\nResults");
        console::log("%s", asTable(app->GetBenchmarkResults()).c_str());
    }

    SafeDelete(app);
    activeApi->Deactivate();
    DestroyGfxApis(&allGfxApis);
    Cleanup();

    return 0;
}

// --------------------------------------------------------------------------------------------------------------------
std::string asTable(BenchmarkResults _results)
{
    char buffer[1024];
    std::string retStr;
    const char* kHeaderFmt =  " %-23s %-51s %-7s %-15s %-9s %-7s\n";
    const char* kRowFmt =     " %-23s %-51s %-7d %-15.3f %-9.3f %-7.3f\n";
    const char* kRowFailFmt = " %-23s %-51s %-7s %-15s %-9s %-7s\n";

    snprintf(buffer, sizeof(buffer)-1, kHeaderFmt, "Problem", "Solution", "Frames", "Elapsed (s)", "fps", "ms/f");
    retStr += buffer;

    for (auto it = _results.cbegin(); it != _results.cend(); ++it) {
        const char* problemName = it->first.first.c_str();
        const char* solutionName = it->first.second.c_str();
        const unsigned int frameCount = it->second.first;
        const double elapsedS = it->second.second;

        if (frameCount != 0 && elapsedS != 0.0) {
            const double fps = frameCount / elapsedS;
            const double mspf = elapsedS * 1000.0 / frameCount;

            snprintf(buffer, sizeof(buffer), kRowFmt, problemName, solutionName, frameCount, elapsedS, fps, mspf);
        } else {
            snprintf(buffer, sizeof(buffer), kRowFailFmt, problemName, solutionName, "N/A", "N/A", "N/A", "N/A");
        }
        retStr += buffer;
    }

    return retStr;
}
