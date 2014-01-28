#include "pch.h"

#include "console.h"
#include "gfx.h"
#include "timer.h"

#include "wgl.h"

#define SAFE_DELETE(x)         { delete x; x = nullptr; }

#ifndef _WIN32
    // This is not supported on !Windows.
    GfxApi *create_gfx_dx11()       { return nullptr; }
#endif

// --------------------------------------------------------------------------------------------------------------------
SDL_Window *s_window; /* Our window handle */

static GfxApi* s_api;
static GfxSwapChain* s_swap_chain;
static GfxFrameBuffer* s_frame_buffer;

static TestId s_test_id = TestId::TexturesForward;
static TestCase* s_test_case;

// ------------------------------------------------------------------------------------------------
static bool set_test(TestId id)
{
    SAFE_DELETE(s_test_case);

    s_test_id = id;

    s_test_case = s_api->create_test(s_test_id);
    if (!s_test_case)
        return false;

    return s_test_case->init();
}

// ------------------------------------------------------------------------------------------------

static bool set_api(GfxApi *api)
{
    if (s_api)
    {
        s_api->destroy_frame_buffer(s_frame_buffer);
        s_api->destroy_swap_chain(s_swap_chain);
        delete s_api;
    }

    s_api = api;

    if (s_api)
    {
        if (!s_api->init())
            return false;

        if (!s_api->create_swap_chain(s_window, &s_swap_chain, &s_frame_buffer))
            return false;

        if (!set_test(s_test_id))
            return false;
    }

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void PostQuitEvent()
{
    SDL_Event quitEvent;
    quitEvent.type = SDL_QUIT;
    SDL_PushEvent(&quitEvent);
}

// --------------------------------------------------------------------------------------------------------------------
void on_event(SDL_Event* _event)
{
    assert(_event);

    switch (_event->type)
    {
#if 0
        case WM_CLOSE:
            PostQuitMessage(0);
            break;

        case WM_ERASEBKGND:
            break;

        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                EndPaint(hWnd, &ps);
            }
            break;
#endif

    case SDL_KEYDOWN:
        {
            bool handled = true;
            switch (_event->key.keysym.sym)
            {
            case 'd':
                console::debug("Initializing DX11 backend\n");
                set_api(create_gfx_dx11());
                break;
            case 'g':
                console::debug("Initializing GL backend\n");
                set_api(create_gfx_gl());
                break;

            case SDLK_F1:
                set_test(TestId::StreamingVB);
                break;

            case SDLK_F2:
                set_test(TestId::CubesUniform);
                break;

            case SDLK_F3:
                set_test(TestId::CubesDynamicBuffer);
                break;

            case SDLK_F4:
                #ifdef _WIN32
                    if (_event->key.keysym.mod & KMOD_ALT) {
                        PostQuitEvent();
                        break;
                    }
                #endif
                set_test(TestId::CubesBufferRange);
                break;

            case SDLK_F5:
                set_test(TestId::CubesTexCoord);
                break;

            case SDLK_F6:
                set_test(TestId::CubesMultiDraw);
                break;

            case SDLK_F7:
                set_test(TestId::CubesBufferStorage);
                break;

            case SDLK_F8:
                set_test(TestId::CubesBindless);
                break;

            case SDLK_F9:
                set_test(TestId::CubesBindlessIndirect);
                break;

            case SDLK_F10:
                set_test(TestId::TexturesNoTex);
                break;

            case SDLK_F11:
                set_test(TestId::TexturesForward);
                break;

            case '1':
                set_test(TestId::TexturesBindless);
                break;

            case '2':
                set_test(TestId::TexturesBindlessMultiDraw);
                break;

            case '3':
                set_test(TestId::TexturesSparseBindlessTextureArray);
                break;

            case '4':
                set_test(TestId::TexturesSparseBindlessTextureArrayMultiDraw);
                break;

            default:
                handled = false;
                break;
            }

        }
        // else drop through to DefWindowProc
        break;
    }
}

// ------------------------------------------------------------------------------------------------
static SDL_Window* create_window(const char* title, int x, int y, int width, int height)
{
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* retWnd = SDL_CreateWindow(title, x, y, width, height, 
                                          SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    return retWnd;
}


// ------------------------------------------------------------------------------------------------
static void update_fps()
{
    static int s_frame_count;
    static unsigned long long s_last_frame_time;

    ++s_frame_count;
    unsigned long long now = timer::read();
    double dt = timer::to_sec(now - s_last_frame_time);
    if (dt >= 1.0)
    {
        console::debug("FPS: %g\n", s_frame_count / dt);
        s_frame_count = 0;
        s_last_frame_time = now;
    }
}

// ------------------------------------------------------------------------------------------------
static void render()
{
    if (!s_test_case)
        return;

    if (!s_test_case->begin(s_swap_chain, s_frame_buffer))
        return;

    // This is the main entry point shared by all tests. 
    s_test_case->render();
    
    s_test_case->end(s_swap_chain);
    update_fps();
}

static bool sdl_init()
{
    return SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) >= 0;
}

// ------------------------------------------------------------------------------------------------
static bool init()
{
    if (!sdl_init())
        return false;

    timer::init();

    s_window = create_window("Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 768);
    if (!s_window)
        return false;
    
    if (!set_api(create_gfx_gl())) {
        return false;
    }
        
    return true;
}

// ------------------------------------------------------------------------------------------------
static void cleanup()
{
    set_api(nullptr);

    SDL_DestroyWindow(s_window);
    SDL_Quit();
}

// ------------------------------------------------------------------------------------------------
int main(int argv, char* argc[])
{
    if (!init()) {
        return -1;
    }

    if (!s_window) {
        console::error("Unable to create window");
    }

    for (;;) {
        SDL_Event sdl_event;
        if (SDL_PollEvent(&sdl_event)) {
            if (sdl_event.type == SDL_QUIT) {
                break;
            }

            on_event(&sdl_event);
        } else {
            render();
        }
    }

    cleanup();

    return 0;
}


// ------------------------------------------------------------------------------------------------
void StreamingVB::render()
{
    float spacing = 1.0f;
    float x = spacing;
    float y = spacing;
    float w = 1.0f;
    float h = 1.0f;

    for (int i = 0; i < 160000; ++i)
    {
        VertexPos2 verts[] =
        {
            { x, y },
            { x + w, y },
            { x, y + h },
            { x + w, y },
            { x, y + h },
            { x + w, y + h },
        };

        // TODO: It'd be ideal to get rid of this virtual call--it's 160,000 per frame. Or at least
        // make sure it's not really virtual in release builds.
        draw(verts, 6);

        x += w + spacing;
        if (x > 1000)
        {
            x = spacing;
            y += h + spacing;
        }
    }
}

// ------------------------------------------------------------------------------------------------
void Cubes::render()
{
    static float angle = 0.0f;
    static Matrix* transforms;
    if (!transforms)
        transforms = new Matrix[CUBES_COUNT];

    Matrix *m = transforms;
    for (int x = 0; x < CUBES_X; ++x)
    {
        for (int y = 0; y < CUBES_Y; ++y)
        {
            for (int z = 0; z < CUBES_Z; ++z)
            {
                *m = matrix_rotation_z(angle);
                m->w.x = 2.0f * x - CUBES_X;
                m->w.y = 2.0f * y - CUBES_Y;
                m->w.z = 2.0f * z - CUBES_Z;
                ++m;
            }
        }
    }

    draw(transforms, CUBES_COUNT);
    angle += 0.01f;
}

// ------------------------------------------------------------------------------------------------
void Textures::render()
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

    draw(transforms, TEXTURES_COUNT);
    angle += 0.01f;
}
