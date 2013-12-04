#include "gfx.h"

#if defined(USE_DX11)

#include <d3d11.h>

// Libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#define SAFE_RELEASE(x)         { if (x != nullptr) { x->Release(); x = nullptr; } }

#include "dx50_fs_code.h"
#include "dx50_vs_code.h"

struct GfxFrameBuffer
{
    ID3D11RenderTargetView* render_target_view;
    ID3D11DepthStencilView* depth_stencil_view;
};

struct UIConstants
{
    float width;
    float height;
    float pad[2];
};

namespace gfx
{
    static HRESULT create_device();
    static HRESULT create_render_target(IDXGISwapChain* dxgi_swap_chain, ID3D11RenderTargetView** out_render_target_view);
    static HRESULT create_depth_buffer(IDXGISwapChain* dxgi_swap_chain, ID3D11DepthStencilView** out_depth_stencil_view);

    static HRESULT create_constant_buffer(int size, const void* data, ID3D11Buffer** out_buffer);
    static HRESULT create_dynamic_vertex_buffer(int size, const void* data, ID3D11Buffer** out_buffer);

    static IDXGIFactory* s_factory;
    static ID3D11Device* s_device;
    static D3D_FEATURE_LEVEL s_feature_level;
    static ID3D11DeviceContext* s_context;
    static GfxFrameBuffer* s_frame_buffer;

    static ID3D11InputLayout* s_layout;
    static ID3D11Buffer* s_cb;
    static ID3D11VertexShader* s_vs;
    static ID3D11PixelShader* s_ps;
    static ID3D11SamplerState* s_sampler;
    static ID3D11RasterizerState* s_rs;
    static ID3D11BlendState* s_bs;
    static ID3D11DepthStencilState* s_dss;

    static ID3D11Buffer* s_dyn_vb;
    static int s_dyn_offset;
}

static HRESULT gfx::create_constant_buffer(int size, const void* data, ID3D11Buffer** out_buffer)
{
    D3D11_BUFFER_DESC desc = { 0 };
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = size;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initialData = { 0 };
    initialData.pSysMem = data;

    return s_device->CreateBuffer(&desc, data ? &initialData : nullptr, out_buffer);
}

static HRESULT gfx::create_dynamic_vertex_buffer(int size, const void* data, ID3D11Buffer** out_buffer)
{
    D3D11_BUFFER_DESC desc = { 0 };
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.ByteWidth = size;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    D3D11_SUBRESOURCE_DATA initialData = { 0 };
    initialData.pSysMem = data;

    return s_device->CreateBuffer(&desc, data ? &initialData : nullptr, out_buffer);
}

bool gfx::init_device()
{
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&s_factory));
    if (FAILED(hr))
        return false;

    hr = create_device();
    if (FAILED(hr))
        return false;

    return true;
}

bool gfx::init_objects()
{
    // Constant Buffer
    HRESULT hr = create_constant_buffer(sizeof(UIConstants), nullptr, &s_cb);
    if (FAILED(hr))
        return false;

    // Shaders
    hr = s_device->CreateVertexShader(s_vs_code, sizeof(s_vs_code), nullptr, &s_vs);
    if (FAILED(hr))
        return false;

    hr = s_device->CreatePixelShader(s_fs_code, sizeof(s_fs_code), nullptr, &s_ps);
    if (FAILED(hr))
        return false;

    D3D11_INPUT_ELEMENT_DESC elements[] =
    {
        { "ATTR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = s_device->CreateInputLayout(elements, 1, s_vs_code, sizeof(s_vs_code), &s_layout);
    if (FAILED(hr))
        return false;

    // Render States
    {
        D3D11_RASTERIZER_DESC desc;
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_NONE;
        desc.FrontCounterClockwise = FALSE;
        desc.DepthBias = 0;
        desc.SlopeScaledDepthBias = 0.0f;
        desc.DepthBiasClamp = 0.0f;
        desc.DepthClipEnable = TRUE;
        desc.ScissorEnable = FALSE;
        desc.MultisampleEnable = FALSE;
        desc.AntialiasedLineEnable = FALSE;

        hr = s_device->CreateRasterizerState(&desc, &s_rs);
        if (FAILED(hr))
            return false;
    }

    {
        D3D11_BLEND_DESC desc;
        desc.AlphaToCoverageEnable = FALSE;
        desc.IndependentBlendEnable = FALSE;

        D3D11_RENDER_TARGET_BLEND_DESC& rtDesc = desc.RenderTarget[0];
        rtDesc.BlendEnable = TRUE;
        rtDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
        rtDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        rtDesc.BlendOp = D3D11_BLEND_OP_ADD;
        rtDesc.SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        rtDesc.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
        rtDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
        rtDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        hr = s_device->CreateBlendState(&desc, &s_bs);
        if (FAILED(hr))
            return false;
    }

    {
        D3D11_DEPTH_STENCIL_DESC desc;
        desc.DepthEnable = FALSE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        desc.DepthFunc = D3D11_COMPARISON_LESS;
        desc.StencilEnable = FALSE;
        desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        hr = s_device->CreateDepthStencilState(&desc, &s_dss);
        if (FAILED(hr))
            return false;
    }

    {
        D3D11_SAMPLER_DESC desc;
        desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
        desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        desc.MipLODBias = 0.0f;
        desc.MaxAnisotropy = 0;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.BorderColor[0] = 0.0f;
        desc.BorderColor[1] = 0.0f;
        desc.BorderColor[2] = 0.0f;
        desc.BorderColor[3] = 0.0f;
        desc.MinLOD = 0;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        hr = s_device->CreateSamplerState(&desc, &s_sampler);
        if (FAILED(hr))
            return false;
    }

    // Dynamic vertex buffer
    hr = create_dynamic_vertex_buffer(DYN_VB_SIZE, nullptr, &s_dyn_vb);
    if (FAILED(hr))
        return false;

    return true;
}

void gfx::destroy_objects()
{
    SAFE_RELEASE(s_dyn_vb);

    SAFE_RELEASE(s_layout);
    SAFE_RELEASE(s_cb);
    SAFE_RELEASE(s_vs);
    SAFE_RELEASE(s_ps);
    SAFE_RELEASE(s_rs);
    SAFE_RELEASE(s_bs);
    SAFE_RELEASE(s_dss);
    SAFE_RELEASE(s_sampler);
}

void gfx::shutdown_device()
{
    if (s_context)
        s_context->ClearState();

    SAFE_RELEASE(s_context);
    SAFE_RELEASE(s_device);
    SAFE_RELEASE(s_factory);
}

HRESULT gfx::create_device()
{
    IDXGIAdapter* dxgi_adapter;

    HRESULT hr = s_factory->EnumAdapters(0, &dxgi_adapter);
    if (FAILED(hr))
        return hr;

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
        &s_device,
        &s_feature_level,
        &s_context);

    dxgi_adapter->Release();

    if (FAILED(hr))
        return hr;

    return S_OK;
}

bool gfx::create_swap_chain(void* window, 
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
    HRESULT hr = s_factory->CreateSwapChain(s_device, &swap_chain_desc, &dxgi_swap_chain);

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

HRESULT gfx::create_render_target(IDXGISwapChain* dxgi_swap_chain, ID3D11RenderTargetView** out_render_target_view)
{
    // Get the back buffer from the swap chain
    ID3D11Texture2D* back_buffer;
    HRESULT hr = dxgi_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
    if (FAILED(hr))
        return hr;

    // Create a render target view
    ID3D11RenderTargetView* render_target_view;
    hr = s_device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
    back_buffer->Release();

    if (FAILED(hr))
        return hr;

    *out_render_target_view = render_target_view;
    return S_OK;
}

HRESULT gfx::create_depth_buffer(IDXGISwapChain* dxgi_swap_chain, ID3D11DepthStencilView** out_depth_stencil_view)
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
    hr = s_device->CreateTexture2D(&tex_desc, nullptr, &d3d_depth_stencil_tex);
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
    hr = s_device->CreateDepthStencilView(d3d_depth_stencil_tex, &dsv_desc, &d3d_depth_stencil_view);
    d3d_depth_stencil_tex->Release();

    if (FAILED(hr))
        return hr;

    *out_depth_stencil_view = d3d_depth_stencil_view;
    return S_OK;
}

void gfx::destroy_swap_chain(GfxSwapChain* swap_chain)
{
    IDXGISwapChain* dxgi_swap_chain = reinterpret_cast<IDXGISwapChain*>(swap_chain);

    SAFE_RELEASE(dxgi_swap_chain);
}

void gfx::destroy_frame_buffer(GfxFrameBuffer* frame_buffer)
{
    if (frame_buffer)
    {
        SAFE_RELEASE(frame_buffer->render_target_view);
        SAFE_RELEASE(frame_buffer->depth_stencil_view);
        delete frame_buffer;
    }
}

void gfx::set_frame_buffer(GfxFrameBuffer* frame_buffer)
{
    s_context->OMSetRenderTargets(1, &frame_buffer->render_target_view, frame_buffer->depth_stencil_view);
    s_frame_buffer = frame_buffer;
}

void gfx::clear_color(const float c[4])
{
    s_context->ClearRenderTargetView(s_frame_buffer->render_target_view, c);
}

void gfx::clear_depth(float d)
{
    s_context->ClearDepthStencilView(s_frame_buffer->depth_stencil_view, D3D11_CLEAR_DEPTH, d, 0);
}

bool gfx::begin(void* window, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer)
{
    HWND hWnd = reinterpret_cast<HWND>(window);
    IDXGISwapChain* dxgi_swap_chain = reinterpret_cast<IDXGISwapChain*>(swap_chain);

    // Check for resize
    RECT rc;
    GetClientRect(hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

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

        s_context->OMSetRenderTargets(0, nullptr, nullptr);

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

    // Setup pipeline state
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(width);
    vp.Height = static_cast<FLOAT>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    UIConstants cb;
    cb.width = 2.0f / width;
    cb.height = -2.0f / height;
    s_context->UpdateSubresource(s_cb, 0, nullptr, &cb, 0, 0);

    ID3D11Buffer* ia_buffers[] = { s_dyn_vb };
    UINT ia_strides[] = { sizeof(Vert) };
    UINT ia_offsets[] = { 0 };

    float blendFactor[4] = { 0, 0, 0, 0 };

    s_context->IASetInputLayout(s_layout);
    s_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    s_context->IASetVertexBuffers(0, 1, ia_buffers, ia_strides, ia_offsets);
    s_context->VSSetShader(s_vs, nullptr, 0);
    s_context->VSSetConstantBuffers(0, 1, &s_cb);
    s_context->GSSetShader(nullptr, nullptr, 0);
    s_context->RSSetState(s_rs);
    s_context->RSSetViewports(1, &vp);
    s_context->PSSetShader(s_ps, nullptr, 0);
    s_context->PSSetSamplers(0, 1, &s_sampler);
    s_context->OMSetBlendState(s_bs, blendFactor, 0xffffffff);
    s_context->OMSetDepthStencilState(s_dss, 0);

    return true;
}

void gfx::end(GfxSwapChain* swap_chain)
{
    IDXGISwapChain* dxgi_swap_chain = reinterpret_cast<IDXGISwapChain*>(swap_chain);

    dxgi_swap_chain->Present(0, 0);
}

void gfx::draw(Vert* vertices, int count)
{
    int stride = sizeof(Vert);
    int vertex_offset = (s_dyn_offset + stride - 1) / stride;
    int byte_offset = vertex_offset * stride;
    int size = count * stride;

    D3D11_MAP mapType = D3D11_MAP_WRITE_NO_OVERWRITE;
    if (byte_offset + size > DYN_VB_SIZE)
    {
        mapType = D3D11_MAP_WRITE_DISCARD;
        vertex_offset = byte_offset = 0;
    }

    s_dyn_offset = byte_offset + size;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = s_context->Map(s_dyn_vb, 0, mapType, 0, &mappedResource);
    if (FAILED(hr))
        return;

    void* dst = static_cast<unsigned char*>(mappedResource.pData) + byte_offset;

    memcpy(dst, vertices, count * sizeof(Vert));

    s_context->Unmap(s_dyn_vb, 0);

    s_context->Draw(count, vertex_offset);
}

#endif
