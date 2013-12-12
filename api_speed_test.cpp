#include "console.h"
#include "gfx.h"
#include "timer.h"

#include <Windows.h>

#define SAFE_DELETE(x)         { delete x; x = nullptr; }

// --------------------------------------------------------------------------------------------------------------------
static HINSTANCE s_instance;
static HWND s_window;
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
LRESULT CALLBACK wnd_proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (message)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    case WM_ERASEBKGND:
        result = 1;
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case 'D':
            console::debug("Initializing DX11 backend\n");
            set_api(create_gfx_dx11());
            break;
        case 'G':
            console::debug("Initializing GL backend\n");
            set_api(create_gfx_gl());
            break;

        case VK_F1:
            set_test(TestId::StreamingVB);
            break;

        case VK_F2:
            set_test(TestId::CubesUniform);
            break;

        case VK_F3:
            set_test(TestId::CubesDynamicBuffer);
            break;

        case VK_F4:
            set_test(TestId::CubesBufferRange);
            break;

        case VK_F5:
            set_test(TestId::CubesTexCoord);
            break;

        case VK_F6:
            set_test(TestId::CubesMultiDraw);
            break;

        case VK_F7:
            set_test(TestId::CubesBufferStorage);
            break;

        case VK_F8:
            set_test(TestId::CubesBindless);
            break;

        case VK_F9:
            set_test(TestId::CubesBindlessIndirect);
            break;

        case VK_F10:
            set_test(TestId::TexturesForward);
            break;

        }
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return result;
}

// ------------------------------------------------------------------------------------------------
static HWND create_window(const char* title, int x, int y, int width, int height)
{
    // Initialize window class description
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = wnd_proc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = s_instance;
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_BACKGROUND + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = "api_speed_test";
    wcex.hIconSm = NULL;
    if (!RegisterClassEx(&wcex))
    {
        MessageBox(nullptr, "Failed to register root window class", "Init Failed", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    // Create window
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD dwExStyle = 0;
    const char* className = "api_speed_test";

    RECT rc = { 0, 0, width, height };
    AdjustWindowRect(&rc, dwStyle, FALSE);
    HWND hWnd = CreateWindowEx(
        dwExStyle,
        className,
        title,
        dwStyle,
        x,
        y,
        rc.right - rc.left,
        rc.bottom - rc.top,
        NULL,
        NULL,
        s_instance,
        nullptr);

    ShowWindow(hWnd, SW_SHOW);
    return hWnd;
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
    static float angle;
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
    static float angle;
    static Matrix* transforms;
    if (!transforms)
        transforms = new Matrix[CUBES_COUNT];

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

// ------------------------------------------------------------------------------------------------
static void render()
{
    if (!s_test_case)
        return;

    if (!s_test_case->begin(s_window, s_swap_chain, s_frame_buffer))
        return;

    // This is the main entry point shared by all tests. 
    s_test_case->render();
    
    s_test_case->end(s_swap_chain);
    update_fps();
}

// ------------------------------------------------------------------------------------------------
static bool init()
{
    timer::init();

    s_window = create_window("Test Window", 50, 50, 1024, 748);
    if (!s_window)
        return false;

    if (!set_api(create_gfx_gl()))
        return false;

    return true;
}

// ------------------------------------------------------------------------------------------------
static void cleanup()
{
    set_api(nullptr);
    DestroyWindow(s_window);
}

// ------------------------------------------------------------------------------------------------
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    s_instance = hInstance;

    if (!init())
        return 1;

    MSG msg;

    for (;;)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                break;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
            render();
    }

    cleanup();

    return 0;
}
