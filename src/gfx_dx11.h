#pragma once

#include "gfx.h"

#define SAFE_RELEASE(x)         { if (x != nullptr) { x->Release(); x = nullptr; } }

struct GfxFrameBuffer
{
    ID3D11RenderTargetView* render_target_view;
    ID3D11DepthStencilView* depth_stencil_view;
};

class GfxApi_DX11 : public GfxApi
{
public:
    GfxApi_DX11();
    virtual ~GfxApi_DX11();

    virtual bool init() override;
    virtual bool create_swap_chain(void* hwnd,
        GfxSwapChain** out_swap_chain,
        GfxFrameBuffer** out_frame_buffer) override;

    virtual void destroy_swap_chain(GfxSwapChain* swap_chain) override;
    virtual void destroy_frame_buffer(GfxFrameBuffer* frame_buffer) override;

    virtual TestCase* create_test(TestId id) override;
};

// DX11 Utilities
HRESULT create_render_target(IDXGISwapChain* dxgi_swap_chain, ID3D11RenderTargetView** out_render_target_view);
HRESULT create_depth_buffer(IDXGISwapChain* dxgi_swap_chain, ID3D11DepthStencilView** out_depth_stencil_view);

bool resize_swap_chain(GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer, int width, int height);
HRESULT create_constant_buffer(int size, const void* data, ID3D11Buffer** out_buffer);
HRESULT create_dynamic_vertex_buffer(int size, const void* data, ID3D11Buffer** out_buffer);

// DX11 Globals
extern IDXGIFactory* g_dxgi_factory;
extern ID3D11Device* g_d3d_device;
extern D3D_FEATURE_LEVEL g_d3d_feature_level;
extern ID3D11DeviceContext* g_d3d_context;

