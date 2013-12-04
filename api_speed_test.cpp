#include "console.h"
#include "gfx.h"
#include "timer.h"

#include <Windows.h>

// --------------------------------------------------------------------------------------------------------------------
static HINSTANCE s_instance;
static HWND s_window;
static GfxSwapChain* s_swap_chain;
static GfxFrameBuffer* s_frame_buffer;

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

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return result;
}

// ------------------------------------------------------------------------------------------------
HWND create_window(const char* title, int x, int y, int width, int height)
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
static void render()
{
    if (!gfx::begin(s_window, s_swap_chain, s_frame_buffer))
        return;

    update_fps();

    float c[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    //float c[4] = { 0.5f, 0.0f, 0.0f, 1.0f };

    gfx::set_frame_buffer(s_frame_buffer);
    gfx::clear_color(c);
    gfx::clear_depth(1.0f);

    float spacing = 1.0f;
    float x = spacing;
    float y = spacing;
    float w = 1.0f;
    float h = 1.0f;

    for (int i = 0; i < 160000; ++i)
    {
        Vert verts[] =
        {
            { x, y },
            { x + w, y },
            { x, y + h },
            { x + w, y },
            { x, y + h },
            { x + w, y + h },
        };

        gfx::draw(verts, 6);

        x += w + spacing;
        if (x > 1000)
        {
            x = spacing;
            y += h + spacing;
        }
    }

    gfx::end(s_swap_chain);
}

// ------------------------------------------------------------------------------------------------
static bool init()
{
    timer::init();

    if (!gfx::init_device())
        return false;

    s_window = create_window("Test Window", 50, 50, 1024, 748);
    if (!s_window)
        return false;

    if (!gfx::create_swap_chain(s_window, &s_swap_chain, &s_frame_buffer))
        return false;

    if (!gfx::init_objects())
        return false;

    return true;
}

// ------------------------------------------------------------------------------------------------
static void cleanup()
{
    gfx::destroy_objects();
    gfx::destroy_frame_buffer(s_frame_buffer);
    gfx::destroy_swap_chain(s_swap_chain);
    gfx::shutdown_device();
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
