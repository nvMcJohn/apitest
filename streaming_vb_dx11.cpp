#include "streaming_vb_dx11.h"
#include "streaming_vb_dx11_ps.h"
#include "streaming_vb_dx11_vs.h"

#include <assert.h>

StreamingVB_DX11::StreamingVB_DX11()
    : m_layout()
    , m_cb()
    , m_vs()
    , m_ps()
    , m_sampler()
    , m_rs()
    , m_bs()
    , m_dss()
    , m_dyn_vb()
    , m_dyn_offset()
{}

StreamingVB_DX11::~StreamingVB_DX11()
{
    SAFE_RELEASE(m_dyn_vb);

    SAFE_RELEASE(m_layout);
    SAFE_RELEASE(m_cb);
    SAFE_RELEASE(m_vs);
    SAFE_RELEASE(m_ps);
    SAFE_RELEASE(m_rs);
    SAFE_RELEASE(m_bs);
    SAFE_RELEASE(m_dss);
    SAFE_RELEASE(m_sampler);
}

bool StreamingVB_DX11::init()
{
    // Constant Buffer
    HRESULT hr = create_constant_buffer(sizeof(Constants), nullptr, &m_cb);
    if (FAILED(hr))
        return false;

    // Shaders
    hr = g_d3d_device->CreateVertexShader(s_vs_code, sizeof(s_vs_code), nullptr, &m_vs);
    if (FAILED(hr))
        return false;

    hr = g_d3d_device->CreatePixelShader(s_fs_code, sizeof(s_fs_code), nullptr, &m_ps);
    if (FAILED(hr))
        return false;

    D3D11_INPUT_ELEMENT_DESC elements[] =
    {
        { "ATTR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    hr = g_d3d_device->CreateInputLayout(elements, 1, s_vs_code, sizeof(s_vs_code), &m_layout);
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

        hr = g_d3d_device->CreateRasterizerState(&desc, &m_rs);
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

        hr = g_d3d_device->CreateBlendState(&desc, &m_bs);
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

        hr = g_d3d_device->CreateDepthStencilState(&desc, &m_dss);
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

        hr = g_d3d_device->CreateSamplerState(&desc, &m_sampler);
        if (FAILED(hr))
            return false;
    }

    // Dynamic vertex buffer
    hr = create_dynamic_vertex_buffer(DYN_VB_SIZE, nullptr, &m_dyn_vb);
    if (FAILED(hr))
        return false;

    return true;
}

bool StreamingVB_DX11::begin(GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer)
{
    int width = 0;
    int height = 0;

#if 0
    SDL_GetWindowSize(swap_chain->wnd, &width, &height);
#else
    assert(0);
#endif

    if (!resize_swap_chain(swap_chain, frame_buffer, width, height))
        return false;

    // Bind and clear frame buffer
    float c[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
    g_d3d_context->OMSetRenderTargets(1, &frame_buffer->render_target_view, frame_buffer->depth_stencil_view);
    g_d3d_context->ClearRenderTargetView(frame_buffer->render_target_view, c);
    g_d3d_context->ClearDepthStencilView(frame_buffer->depth_stencil_view, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // Setup pipeline state
    D3D11_VIEWPORT vp;
    vp.Width = static_cast<FLOAT>(width);
    vp.Height = static_cast<FLOAT>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;

    Constants cb;
    cb.width = 2.0f / width;
    cb.height = -2.0f / height;
    g_d3d_context->UpdateSubresource(m_cb, 0, nullptr, &cb, 0, 0);

    ID3D11Buffer* ia_buffers[] = { m_dyn_vb };
    UINT ia_strides[] = { sizeof(VertexPos2) };
    UINT ia_offsets[] = { 0 };

    float blendFactor[4] = { 0, 0, 0, 0 };

    g_d3d_context->IASetInputLayout(m_layout);
    g_d3d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_d3d_context->IASetVertexBuffers(0, 1, ia_buffers, ia_strides, ia_offsets);
    g_d3d_context->VSSetShader(m_vs, nullptr, 0);
    g_d3d_context->VSSetConstantBuffers(0, 1, &m_cb);
    g_d3d_context->GSSetShader(nullptr, nullptr, 0);
    g_d3d_context->RSSetState(m_rs);
    g_d3d_context->RSSetViewports(1, &vp);
    g_d3d_context->PSSetShader(m_ps, nullptr, 0);
    g_d3d_context->PSSetSamplers(0, 1, &m_sampler);
    g_d3d_context->OMSetBlendState(m_bs, blendFactor, 0xffffffff);
    g_d3d_context->OMSetDepthStencilState(m_dss, 0);

    return true;
}

void StreamingVB_DX11::end(GfxSwapChain* swap_chain)
{
    IDXGISwapChain* dxgi_swap_chain = reinterpret_cast<IDXGISwapChain*>(swap_chain);

    dxgi_swap_chain->Present(0, 0);
}

void StreamingVB_DX11::draw(VertexPos2* vertices, int count)
{
    int stride = sizeof(VertexPos2);
    int vertex_offset = (m_dyn_offset + stride - 1) / stride;
    int byte_offset = vertex_offset * stride;
    int size = count * stride;

    D3D11_MAP mapType = D3D11_MAP_WRITE_NO_OVERWRITE;
    if (byte_offset + size > DYN_VB_SIZE)
    {
        mapType = D3D11_MAP_WRITE_DISCARD;
        vertex_offset = byte_offset = 0;
    }

    m_dyn_offset = byte_offset + size;

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = g_d3d_context->Map(m_dyn_vb, 0, mapType, 0, &mappedResource);
    if (FAILED(hr))
        return;

    void* dst = static_cast<unsigned char*>(mappedResource.pData) + byte_offset;

    memcpy(dst, vertices, count * sizeof(VertexPos2));

    g_d3d_context->Unmap(m_dyn_vb, 0);

    g_d3d_context->Draw(count, vertex_offset);
}
