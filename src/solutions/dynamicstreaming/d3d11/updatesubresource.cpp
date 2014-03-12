#include "pch.h"

#include "updatesubresource.h"
#include "problems/dynamicstreaming.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
DynamicStreamingD3D11UpdateSubresource::DynamicStreamingD3D11UpdateSubresource()
: mInputLayout()
, mConstantBuffer()
, mVertexShader()
, mPixelShader()
, mSamplerState()
, mRasterizerState()
, mBlendState()
, mDepthStencilState()
, mDynamicVertexBuffer()
, mParticleBufferSize()
, mStartDestOffset()
{}

// --------------------------------------------------------------------------------------------------------------------
DynamicStreamingD3D11UpdateSubresource::~DynamicStreamingD3D11UpdateSubresource()
{
}

// --------------------------------------------------------------------------------------------------------------------
bool DynamicStreamingD3D11UpdateSubresource::Init(size_t _maxVertexCount)
{
    D3D11_INPUT_ELEMENT_DESC elements[] =
    {
        { "ATTR", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    if (!CompileProgram(L"streaming_vb_d3d11_vs.hlsl", &mVertexShader, 
                        L"streaming_vb_d3d11_ps.hlsl", &mPixelShader,
                        ArraySize(elements), elements, &mInputLayout)) {
        return false;
    }

    // Constant Buffer
    HRESULT hr = CreateConstantBuffer(sizeof(Constants), nullptr, &mConstantBuffer);
    if (FAILED(hr)) {
        return false;
    }

    // Render States
    {
        D3D11_RASTERIZER_DESC desc;
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_NONE;
        desc.FrontCounterClockwise = FALSE;
        desc.DepthBias = 0;
        desc.SlopeScaledDepthBias = 0.0f;
        desc.DepthBiasClamp = 0.0f;
        desc.DepthClipEnable = FALSE;
        desc.ScissorEnable = FALSE;
        desc.MultisampleEnable = FALSE;
        desc.AntialiasedLineEnable = FALSE;

        hr = g_d3d_device->CreateRasterizerState(&desc, &mRasterizerState);
        if (FAILED(hr)) {
            return false;
        }
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

        hr = g_d3d_device->CreateBlendState(&desc, &mBlendState);
        if (FAILED(hr)) {
            return false;
        }
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

        hr = g_d3d_device->CreateDepthStencilState(&desc, &mDepthStencilState);
        if (FAILED(hr)) {
            return false;
        }
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

        hr = g_d3d_device->CreateSamplerState(&desc, &mSamplerState);
        if (FAILED(hr)) {
            return false;
        }
    }

    // Dynamic vertex buffer
    mParticleBufferSize = kTripleBuffer * sizeof(Vec2) * _maxVertexCount;
    hr = CreateDynamicVertexBuffer(mParticleBufferSize, nullptr, D3D11_USAGE_DEFAULT, 0, &mDynamicVertexBuffer);
    if (FAILED(hr)) {
        return false;
    }

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void DynamicStreamingD3D11UpdateSubresource::Render(const std::vector<Vec2>& _vertices)
{
    Constants cb;
    cb.width = 2.0f / mWidth;
    cb.height = -2.0f / mHeight;

    g_d3d_context->UpdateSubresource(mConstantBuffer, 0, nullptr, &cb, 0, 0);

    ID3D11Buffer* ia_buffers[] = { mDynamicVertexBuffer };
    UINT ia_strides[] = { sizeof(Vec2) };
    UINT ia_offsets[] = { 0 };

    float blendFactor[4] = { 0, 0, 0, 0 };

    g_d3d_context->IASetInputLayout(mInputLayout);
    g_d3d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_d3d_context->IASetVertexBuffers(0, 1, ia_buffers, ia_strides, ia_offsets);
    g_d3d_context->VSSetShader(mVertexShader, nullptr, 0);
    g_d3d_context->VSSetConstantBuffers(0, 1, &mConstantBuffer);
    g_d3d_context->GSSetShader(nullptr, nullptr, 0);
    g_d3d_context->RSSetState(mRasterizerState);
    g_d3d_context->PSSetShader(mPixelShader, nullptr, 0);
    g_d3d_context->PSSetSamplers(0, 1, &mSamplerState);
    g_d3d_context->OMSetBlendState(mBlendState, blendFactor, 0xffffffff);
    g_d3d_context->OMSetDepthStencilState(mDepthStencilState, 0);

    const int kVertexSizeBytes = sizeof(Vec2);
    const int kParticleCount = int(_vertices.size()) / kVertsPerParticle;
    const size_t kParticleSizeBytes = int(kVertsPerParticle * kVertexSizeBytes);
    const int kStartIndex = mStartDestOffset / kVertexSizeBytes;

    for (int i = 0; i < kParticleCount; ++i)
    {
        const int vertexOffset = i * kVertsPerParticle;
        const int srcOffset = vertexOffset;
        size_t dstOffset = mStartDestOffset + (i * kParticleSizeBytes);

        D3D11_BOX dstBox;
        dstBox.left = dstOffset;
        dstBox.right = dstOffset + kParticleSizeBytes;
        dstBox.top  = dstBox.front  = 0;
        dstBox.back = dstBox.bottom = 1;

        g_d3d_context->UpdateSubresource(mDynamicVertexBuffer, 0, &dstBox, &_vertices[srcOffset], 0, 0);
        g_d3d_context->Draw(kVertsPerParticle, dstOffset / kVertexSizeBytes);
    }

    mStartDestOffset = (mStartDestOffset + (kParticleCount * kParticleSizeBytes)) % mParticleBufferSize;
}

// --------------------------------------------------------------------------------------------------------------------
void DynamicStreamingD3D11UpdateSubresource::Shutdown()
{
    SafeRelease(mDynamicVertexBuffer);

    SafeRelease(mInputLayout);
    SafeRelease(mConstantBuffer);
    SafeRelease(mVertexShader);
    SafeRelease(mPixelShader);
    SafeRelease(mRasterizerState);
    SafeRelease(mBlendState);
    SafeRelease(mDepthStencilState);
    SafeRelease(mSamplerState);
}

