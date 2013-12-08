#include "gfx_dx11.h"
#include "streaming_vb_dx11.h"
#include <assert.h>

// Libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

// Globals
IDXGIFactory* g_dxgi_factory;
ID3D11Device* g_d3d_device;
D3D_FEATURE_LEVEL g_d3d_feature_level;
ID3D11DeviceContext* g_d3d_context;

GfxApi *create_gfx_dx11() { return new GfxApi_DX11; }

GfxApi_DX11::GfxApi_DX11()
{}

GfxApi_DX11::~GfxApi_DX11()
{
    if (g_d3d_context)
        g_d3d_context->ClearState();

    SAFE_RELEASE(g_d3d_context);
    SAFE_RELEASE(g_d3d_device);
    SAFE_RELEASE(g_dxgi_factory);
}

bool GfxApi_DX11::init()
{
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&g_dxgi_factory));
    if (FAILED(hr))
        return false;

    IDXGIAdapter* dxgi_adapter;
    hr = g_dxgi_factory->EnumAdapters(0, &dxgi_adapter);
    if (FAILED(hr))
        return false;

    // Device flags
    UINT create_device_flags = 0;
#ifdef _DEBUG
    create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    // Feature levels we support
    D3D_FEATURE_LEVEL feature_levels[] =
    {
        D3D_FEATURE_LEVEL_11_0,
    };

    // Create Device
    hr = D3D11CreateDevice(
        dxgi_adapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        create_device_flags,
        feature_levels,
        ARRAYSIZE(feature_levels),
        D3D11_SDK_VERSION,
        &g_d3d_device,
        &g_d3d_feature_level,
        &g_d3d_context);

    dxgi_adapter->Release();

    if (FAILED(hr))
        return false;

    return true;
}


bool GfxApi_DX11::create_swap_chain(void* window,
    GfxSwapChain** out_swap_chain,
    GfxFrameBuffer** out_frame_buffer)
{
    HWND hWnd = reinterpret_cast<HWND>(window);

    *out_swap_chain = nullptr;
    *out_frame_buffer = nullptr;

    RECT rc;
    GetClientRect(hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Create Swap Chain
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    swap_chain_desc.BufferDesc.Width = width;
    swap_chain_desc.BufferDesc.Height = height;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.OutputWindow = hWnd;
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_desc.Flags = 0;

    IDXGISwapChain* dxgi_swap_chain;
    HRESULT hr = g_dxgi_factory->CreateSwapChain(g_d3d_device, &swap_chain_desc, &dxgi_swap_chain);

    if (FAILED(hr))
        return false;

    *out_swap_chain = reinterpret_cast<GfxSwapChain*>(dxgi_swap_chain);

    GfxFrameBuffer* frame_buffer = new GfxFrameBuffer;
    frame_buffer->render_target_view = nullptr;
    frame_buffer->depth_stencil_view = nullptr;
    *out_frame_buffer = frame_buffer;

    hr = create_render_target(dxgi_swap_chain, &frame_buffer->render_target_view);
    if (FAILED(hr))
        return false;

    hr = create_depth_buffer(dxgi_swap_chain, &frame_buffer->depth_stencil_view);
    if (FAILED(hr))
        return false;

    return true;
}

void GfxApi_DX11::destroy_swap_chain(GfxSwapChain* swap_chain)
{
    IDXGISwapChain* dxgi_swap_chain = reinterpret_cast<IDXGISwapChain*>(swap_chain);

    SAFE_RELEASE(dxgi_swap_chain);
}

void GfxApi_DX11::destroy_frame_buffer(GfxFrameBuffer* frame_buffer)
{
    if (frame_buffer)
    {
        SAFE_RELEASE(frame_buffer->render_target_view);
        SAFE_RELEASE(frame_buffer->depth_stencil_view);
        delete frame_buffer;
    }
}

StreamingVB* GfxApi_DX11::create_streaming_vb()
{
    return new StreamingVB_DX11;
}


HRESULT create_constant_buffer(int size, const void* data, ID3D11Buffer** out_buffer)
{
    D3D11_BUFFER_DESC desc = { 0 };
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = size;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initialData = { 0 };
    initialData.pSysMem = data;

    return g_d3d_device->CreateBuffer(&desc, data ? &initialData : nullptr, out_buffer);
}

HRESULT create_dynamic_vertex_buffer(int size, const void* data, ID3D11Buffer** out_buffer)
{
    D3D11_BUFFER_DESC desc = { 0 };
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = size;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA initialData = { 0 };
    initialData.pSysMem = data;

    return g_d3d_device->CreateBuffer(&desc, data ? &initialData : nullptr, out_buffer);
}

HRESULT create_render_target(IDXGISwapChain* dxgi_swap_chain, ID3D11RenderTargetView** out_render_target_view)
{
    // Get the back buffer from the swap chain
    ID3D11Texture2D* back_buffer;
    HRESULT hr = dxgi_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11RenderTargetView* render_target_view;
    hr = g_d3d_device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
    back_buffer->Release();

    if (FAILED(hr))
        return hr;

    *out_render_target_view = render_target_view;
    return S_OK;
}

HRESULT create_depth_buffer(IDXGISwapChain* dxgi_swap_chain, ID3D11DepthStencilView** out_depth_stencil_view)
{
    // Get the back buffer from the swap chain
    ID3D11Texture2D* back_buffer;
    HRESULT hr = dxgi_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
    if (FAILED(hr))
        return hr;

    // Get back buffer desc
    D3D11_TEXTURE2D_DESC back_buffer_desc;
    back_buffer->GetDesc(&back_buffer_desc);
    back_buffer->Release();

    // Get swap chain desc
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    hr = dxgi_swap_chain->GetDesc(&swap_chain_desc);
    if (FAILED(hr))
        return hr;

    // Create depth stencil texture
    D3D11_TEXTURE2D_DESC tex_desc;
    ZeroMemory(&tex_desc, sizeof(tex_desc));
    tex_desc.Width = back_buffer_desc.Width;
    tex_desc.Height = back_buffer_desc.Height;
    tex_desc.MipLevels = 1;
    tex_desc.ArraySize = 1;
    tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    tex_desc.SampleDesc = swap_chain_desc.SampleDesc;
    tex_desc.Usage = D3D11_USAGE_DEFAULT;
    tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tex_desc.CPUAccessFlags = 0;
    tex_desc.MiscFlags = 0;

    ID3D11Texture2D* d3d_depth_stencil_tex;
    hr = g_d3d_device->CreateTexture2D(&tex_desc, nullptr, &d3d_depth_stencil_tex);
    if (FAILED(hr))
        return hr;

    // Create the depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
    ZeroMemory(&dsv_desc, sizeof(dsv_desc));
    dsv_desc.Format = tex_desc.Format;
    dsv_desc.ViewDimension = 
        tex_desc.SampleDesc.Count > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    dsv_desc.Texture2D.MipSlice = 0;

    ID3D11DepthStencilView* d3d_depth_stencil_view;
    hr = g_d3d_device->CreateDepthStencilView(d3d_depth_stencil_tex, &dsv_desc, &d3d_depth_stencil_view);
    d3d_depth_stencil_tex->Release();

    if (FAILED(hr))
        return hr;

    *out_depth_stencil_view = d3d_depth_stencil_view;
    return S_OK;
}

bool resize_swap_chain(GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer, int width, int height)
{
    IDXGISwapChain* dxgi_swap_chain = reinterpret_cast<IDXGISwapChain*>(swap_chain);

    DXGI_SWAP_CHAIN_DESC sd;
    HRESULT hr = dxgi_swap_chain->GetDesc(&sd);
    if (FAILED(hr))
        return false;

    if (sd.BufferDesc.Width != width || sd.BufferDesc.Height != height)
    {
        // Resize occured, release current objects and resize buffers.

        SAFE_RELEASE(frame_buffer->render_target_view);
        SAFE_RELEASE(frame_buffer->depth_stencil_view);

        BOOL bFullscreen;
        dxgi_swap_chain->GetFullscreenState(&bFullscreen, nullptr);

        g_d3d_context->OMSetRenderTargets(0, nullptr, nullptr);

        UINT flags = 0;
        if (bFullscreen)
            flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        hr = dxgi_swap_chain->ResizeBuffers(sd.BufferCount, 0, 0, sd.BufferDesc.Format, flags);
        if (FAILED(hr))
            return false;
    }

    // Recreate views
    if (!frame_buffer->render_target_view)
    {
        hr = create_render_target(dxgi_swap_chain, &frame_buffer->render_target_view);
        if (FAILED(hr))
            return false;
    }

    if (!frame_buffer->depth_stencil_view)
    {
        hr = create_depth_buffer(dxgi_swap_chain, &frame_buffer->depth_stencil_view);
        if (FAILED(hr))
            return false;
    }

    return true;
}
