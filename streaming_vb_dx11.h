// Streaming VB test - DX11 Implementation

#pragma once

#include "gfx_dx11.h"

class StreamingVB_DX11 : public StreamingVB
{
public:
    StreamingVB_DX11();
    virtual ~StreamingVB_DX11();

    virtual bool init() override;

    virtual bool begin(void* hwnd, GfxSwapChain* swap_chain, GfxFrameBuffer* frame_buffer) override;
    virtual void end(GfxSwapChain* swap_chain) override;

    virtual void draw(Vert* vertices, int count) override;

private:
    ID3D11InputLayout* m_layout;
    ID3D11Buffer* m_cb;
    ID3D11VertexShader* m_vs;
    ID3D11PixelShader* m_ps;
    ID3D11SamplerState* m_sampler;
    ID3D11RasterizerState* m_rs;
    ID3D11BlendState* m_bs;
    ID3D11DepthStencilState* m_dss;

    ID3D11Buffer* m_dyn_vb;
    int m_dyn_offset;
};
