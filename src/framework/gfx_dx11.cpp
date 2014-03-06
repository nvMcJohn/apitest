#include "pch.h"

#include "gfx_dx11.h"
#include <d3dcompiler.h>
// #include "streaming_vb_dx11.h"

// Libraries
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Globals
IDXGIFactory* g_dxgi_factory;
ID3D11Device* g_d3d_device;
D3D_FEATURE_LEVEL g_d3d_feature_level;
ID3D11DeviceContext* g_d3d_context;

GfxBaseApi *CreateGfxDirect3D11() { return new GfxApiDirect3D11; }

HWND GetHwnd(SDL_Window* _wnd);

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
GfxApiDirect3D11::GfxApiDirect3D11()
: mSwapChain()
, mColorView()
, mDepthStencilView()
{ }

// --------------------------------------------------------------------------------------------------------------------
GfxApiDirect3D11::~GfxApiDirect3D11()
{
}

// --------------------------------------------------------------------------------------------------------------------
bool GfxApiDirect3D11::Init(const std::string& _title, int _x, int _y, int _width, int _height)
{
    if (!GfxBaseApi::Init(_title, _x, _y, _width, _height)) {
        return false;
    }

    mWnd = SDL_CreateWindow(_title.c_str(), _x, _y, _width, _height, SDL_WINDOW_HIDDEN);

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

    if (!CreateSwapChain()) {
        return false;
    }

    // Set the render target and depth targets.
    g_d3d_context->OMSetRenderTargets(1, &mColorView, mDepthStencilView);

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiDirect3D11::Shutdown()
{
    SafeRelease(mColorView);
    SafeRelease(mDepthStencilView);
    SafeRelease(mSwapChain);

    if (g_d3d_context) {
        g_d3d_context->ClearState();
    }

    SafeRelease(g_d3d_context);
    SafeRelease(g_d3d_device);
    SafeRelease(g_dxgi_factory);

    if (mWnd) {
        SDL_DestroyWindow(mWnd);
        mWnd = nullptr;
    }
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiDirect3D11::Activate()
{
    assert(mWnd);
    SDL_ShowWindow(mWnd);
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiDirect3D11::Deactivate()
{
    assert(mWnd);
    SDL_HideWindow(mWnd);
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiDirect3D11::Clear(Vec4 _clearColor, GLfloat _clearDepth)
{
    // Should go somewhere else. 
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(GetWidth());
    vp.Height = static_cast<FLOAT>(GetHeight());
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    g_d3d_context->RSSetViewports(1, &vp);

    g_d3d_context->ClearRenderTargetView(mColorView, &_clearColor.x);
    g_d3d_context->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, _clearDepth, 0);
}

// --------------------------------------------------------------------------------------------------------------------
void GfxApiDirect3D11::SwapBuffers()
{
    mSwapChain->Present(0, 0);
}

// --------------------------------------------------------------------------------------------------------------------
bool GfxApiDirect3D11::CreateSwapChain()
{
    int width = 1;
    int height = 1;
    SDL_GetWindowSize(mWnd, &width, &height);

    // Create Swap Chain
    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    swap_chain_desc.BufferDesc.Width = UINT(width);
    swap_chain_desc.BufferDesc.Height = UINT(height);
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.OutputWindow = GetHwnd(mWnd);
    swap_chain_desc.Windowed = TRUE;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swap_chain_desc.Flags = 0;

    HRESULT hr = S_OK;
    if (FAILED(hr = g_dxgi_factory->CreateSwapChain(g_d3d_device, &swap_chain_desc, &mSwapChain))) {
        return false;
    }

    if (FAILED(hr = CreateRenderTarget(mSwapChain, &mColorView))) {
        return false;
    }

    if (FAILED(hr = CreateDepthBuffer(mSwapChain, &mDepthStencilView))) {
        return false;
    }

    return true;
}

#if 0
TestCase* GfxApiDirect3D11::create_test(TestId id)
{
    switch (id)
    {
    case TestId::StreamingVB:   return new StreamingVB_DX11;
    }

    return nullptr;
}
#endif

HRESULT CreateRenderTarget(IDXGISwapChain* dxgi_swap_chain, ID3D11RenderTargetView** out_render_target_view)
{
    // Get the back buffer from the swap chain
    ID3D11Texture2D* back_buffer;
    HRESULT hr = dxgi_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
    if (FAILED(hr))
        return hr;

    // Create a Render target view
    ID3D11RenderTargetView* render_target_view;
    hr = g_d3d_device->CreateRenderTargetView(back_buffer, nullptr, &render_target_view);
    back_buffer->Release();

    if (FAILED(hr))
        return hr;

    *out_render_target_view = render_target_view;
    return S_OK;
}

HRESULT CreateDepthBuffer(IDXGISwapChain* dxgi_swap_chain, ID3D11DepthStencilView** out_depth_stencil_view)
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

HRESULT CreateConstantBuffer(int size, const void* data, ID3D11Buffer** out_buffer)
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

HRESULT CreateDynamicVertexBuffer(int size, const void* data, ID3D11Buffer** out_buffer)
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

#if 0
bool resize_swap_chain(, int width, int height)
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
#endif

// --------------------------------------------------------------------------------------------------------------------
ID3DBlob* CompileShader(const std::wstring& _shaderFilename, const char* _shaderEntryPoint, const char* _shaderTarget)
{
    ID3DBlob* outCode = nullptr;
    ID3DBlob* errorMessages = nullptr;

    const UINT kFlags1 = D3DCOMPILE_OPTIMIZATION_LEVEL3;
    const UINT kFlags2 = 0;

    std::wstring shaderFullPath = L"Shaders/hlsl/" + _shaderFilename;


    HRESULT hr = D3DCompileFromFile(shaderFullPath.c_str(), nullptr, nullptr, _shaderEntryPoint, _shaderTarget, 
                                    kFlags1, kFlags2, &outCode, &errorMessages);

    if (FAILED(hr)) {
        console::warn("Failed to compile shader '%ls', errors follow:\n%s", _shaderFilename.c_str(), errorMessages ? errorMessages->GetBufferPointer() : "Compile failed with no errors--possible file not found.");
        SafeRelease(outCode);
        SafeRelease(errorMessages);

        return nullptr;
    }

    if (errorMessages) {
        console::log("Compilation of '%ls' succeeded, but with warnings:\n%s", _shaderFilename.c_str(), errorMessages->GetBufferPointer());
        
    }

    SafeRelease(errorMessages);

    return outCode;
}

// --------------------------------------------------------------------------------------------------------------------
bool CompileProgram(const std::wstring& _vsFilename, ID3D11VertexShader** _outVertexShader,
                    const std::wstring& _psFilename, ID3D11PixelShader** _outPixelShader,
                    UINT _inputElementCount, const D3D11_INPUT_ELEMENT_DESC* _inputElementDescs, ID3D11InputLayout** _outInputLayout)
{
    bool completeSuccess = false;

    assert(_outVertexShader && _outPixelShader);
    assert(_inputElementCount > 0 && _inputElementDescs && _outInputLayout);
    ID3DBlob* vsCode = CompileShader(_vsFilename, "vsMain", "vs_5_0");
    ID3DBlob* psCode = CompileShader(_psFilename, "psMain", "ps_5_0");

    if (!vsCode || !psCode) {
        goto CompileFailed;
    }

    g_d3d_device->CreateVertexShader(vsCode->GetBufferPointer(), vsCode->GetBufferSize(), nullptr, _outVertexShader);
    g_d3d_device->CreatePixelShader(psCode->GetBufferPointer(), psCode->GetBufferSize(), nullptr, _outPixelShader);

    if (!*_outVertexShader || !*_outPixelShader) {
        console::warn("Either the VS or the PS succeeded compilation but failed Creation, which shouldn't happen.");
        goto CreateShaderFailed;
    }
    
    if (FAILED(g_d3d_device->CreateInputLayout(_inputElementDescs, _inputElementCount, vsCode->GetBufferPointer(), vsCode->GetBufferSize(), _outInputLayout))) {
        console::warn("We failed to create an input layout to match the provided vertex shader. This will require debugging.");
        goto CreateInputLayoutFailed;
    }

    completeSuccess = true;
    goto Succeeded;

CreateInputLayoutFailed:
    SafeRelease(*_outInputLayout);

CreateShaderFailed:
    SafeRelease(*_outVertexShader);
    SafeRelease(*_outPixelShader);

    // CompileFailed and Succeeded are actually the same thing, just separated above for clarity.
CompileFailed:
Succeeded:
    SafeRelease(vsCode);
    SafeRelease(psCode);
    return completeSuccess;
}



// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
#include "SDL_syswm.h"

HWND GetHwnd(SDL_Window* _wnd)
{
#ifdef _WIN32
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);

    if (SDL_GetWindowWMInfo(_wnd, &info)) {
        return info.info.win.window;
    }
#endif
    return 0;
}
