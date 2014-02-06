#include "pch.h"

#include "console.h"
#include "factory.h"
#include "gfx.h"
#include "problems/problem.h"
#include "timer.h"
#include "wgl.h"

#define NAME_OPENGLGENERIC      "OpenGL (Generic)"
#define NAME_DIRECT3D11         "Direct3D 11"

#ifdef _WIN32
#   pragma comment(lib, "imm32.lib")
#   pragma comment(lib, "version.lib")
#   pragma comment(lib, "winmm.lib")
#else _WIN32
    // This is not supported on !Windows.
    GfxBaseApi *CreateGfxDirect3D11()       { return nullptr; }
#endif

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
void OnEvent(SDL_Event* _event)
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
        console::debug("FPS: %g\n", s_frame_count / dt);
        s_frame_count = 0;
        s_last_frame_time = now;
    }
}

// ------------------------------------------------------------------------------------------------
static void Render(Problem* _activeProblem, GfxBaseApi* _activeAPI)
{
    assert(_activeProblem);
    assert(_activeAPI);
    
    // This is the main entry point shared by all tests. 
    _activeProblem->Render();
    
    // Present the results.
    _activeAPI->SwapBuffers();
    
    update_fps();
}

static bool InitSDL()
{
    return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) >= 0;
}

// ------------------------------------------------------------------------------------------------
static bool Init()
{
    if (!InitSDL())
    {
        console::error("Unable to initialize SDL -- required -- so exiting.");
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

// ------------------------------------------------------------------------------------------------
int main(int argv, char* argc[])
{
    if (!Init()) {
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

    std::vector<Problem*> problems;
    std::vector<Solution*> solutions;

    ProblemFactory *factory = new ProblemFactory;
    Problem* activeProblem = factory->GetProblems()[0];
    Solution* activeSolution = factory->GetSolutions(activeProblem)[0];

    for (;;) {
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event)) {
            if (sdl_event.type == SDL_QUIT) {
                break;
            }

            OnEvent(&sdl_event);
        } else {
            Render(activeProblem, activeApi);
        }
    }

    SafeDelete(factory);

    activeApi->Deactivate();

    DestroyGfxApis(&allGfxApis);

    Cleanup();

    return 0;
}

#if 0
// ------------------------------------------------------------------------------------------------
void Textures::Render()
{
    static float angle = 0.0f;
    static Matrix* transforms;
    if (!transforms)
        transforms = new Matrix[TEXTURES_COUNT];

    Matrix *m = transforms;
    for (int x = 0; x < TEXTURES_X; ++x)
    {
        for (int y = 0; y < TEXTURES_Y; ++y)
        {
            *m = matrix_rotation_z(angle);
            m->w.x = 2.0f * x - TEXTURES_X;
            m->w.y = 2.0f * y - TEXTURES_Y;
            m->w.z = 0.0f;
            ++m;
        }
    }

    Draw(transforms, TEXTURES_COUNT);
    angle += 0.01f;
}
#endif