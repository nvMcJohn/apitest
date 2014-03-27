#include "pch.h"

#include "naive.h"
#include "framework/gfx_dx11.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
UntexturedObjectsD3D11Naive::UntexturedObjectsD3D11Naive()
: mInputLayout()
, mConstantBufferPerDraw()
, mConstantBufferPerFrame()
, mVertexShader()
, mPixelShader()
, mRasterizerState()
, mBlendState()
, mDepthStencilState()
, mVertexBuffer()
{}

// --------------------------------------------------------------------------------------------------------------------
bool UntexturedObjectsD3D11Naive::Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices, 
                                       const std::vector<UntexturedObjectsProblem::Index>& _indices,
                                       size_t _objectCount)
{
    if (!UntexturedObjectsSolution::Init(_vertices, _indices, _objectCount)) {
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC elements[] =
    {
        { "ObjPos",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,                            D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "Color",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    if (!CompileProgram(L"cubes_d3d11_naive_vs.hlsl", &mVertexShader, 
                        L"cubes_d3d11_naive_ps.hlsl", &mPixelShader,
                        ArraySize(elements), elements, &mInputLayout)) {
        return false;
    }

    // Constant Buffer
    HRESULT hr = CreateConstantBuffer(sizeof(ConstantsPerFrame), nullptr, &mConstantBufferPerFrame);
    if (FAILED(hr)) {
        return false;
    }

    hr = CreateConstantBuffer(sizeof(ConstantsPerDraw), nullptr, &mConstantBufferPerDraw);
    if (FAILED(hr)) {
        return false;
    }

    // Render States
    {
        D3D11_RASTERIZER_DESC desc;
        desc.FillMode = D3D11_FILL_SOLID;
        desc.CullMode = D3D11_CULL_NONE;
        desc.FrontCounterClockwise = TRUE;
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
        desc.DepthEnable = TRUE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
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

    mVertexBuffer = CreateBufferFromVector(_vertices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER);
    if (!mVertexBuffer) {
        return false;
    }

    mIndexBuffer = CreateBufferFromVector(_indices, D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER);
    if (!mVertexBuffer) {
        return false;
    }

    mIndexCount = _indices.size();

    return true;
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsD3D11Naive::Render(const std::vector<Matrix>& _transforms)
{
    // Program
    Vec3 dir = { -0.5f, -1, 1 };
    Vec3 at = { 0, 0, 0 };
    Vec3 up = { 0, 0, 1 };
    dir = normalize(dir);
    Vec3 eye = at - 250 * dir;
    Matrix view = matrix_look_at(eye, at, up);
    ConstantsPerFrame cFrame;
    cFrame.ViewProjection = mProj * view;
    
    g_d3d_context->UpdateSubresource(mConstantBufferPerFrame, 0, nullptr, &cFrame, 0, 0);

    ID3D11Buffer* ia_buffers[] = { mVertexBuffer };
    UINT ia_strides[] = { sizeof(UntexturedObjectsProblem::Vertex) };
    UINT ia_offsets[] = { 0 };

    float blendFactor[4] = { 0, 0, 0, 0 };

    g_d3d_context->IASetInputLayout(mInputLayout);
    g_d3d_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_d3d_context->IASetVertexBuffers(0, 1, ia_buffers, ia_strides, ia_offsets);
    g_d3d_context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    g_d3d_context->VSSetShader(mVertexShader, nullptr, 0);
    g_d3d_context->VSSetConstantBuffers(0, 1, &mConstantBufferPerFrame);
    g_d3d_context->VSSetConstantBuffers(1, 1, &mConstantBufferPerDraw);
    g_d3d_context->GSSetShader(nullptr, nullptr, 0);
    g_d3d_context->RSSetState(mRasterizerState);
    g_d3d_context->PSSetShader(mPixelShader, nullptr, 0);
    g_d3d_context->OMSetBlendState(mBlendState, blendFactor, 0xffffffff);
    g_d3d_context->OMSetDepthStencilState(mDepthStencilState, 0);

    size_t xformCount = _transforms.size();
    assert(xformCount <= mObjectCount);

    ConstantsPerDraw cDraw;
    for (size_t u = 0; u < xformCount; ++u) {
        cDraw.World = transpose(_transforms[u]);
        g_d3d_context->UpdateSubresource(mConstantBufferPerDraw, 0, nullptr, &cDraw, 0, 0);

        g_d3d_context->DrawIndexed(mIndexCount, 0, 0);
    }
}

// --------------------------------------------------------------------------------------------------------------------
void UntexturedObjectsD3D11Naive::Shutdown()
{
    SafeRelease(mVertexBuffer);

    SafeRelease(mInputLayout);
    SafeRelease(mConstantBufferPerDraw);
    SafeRelease(mConstantBufferPerFrame);
    SafeRelease(mVertexShader);
    SafeRelease(mPixelShader);
    SafeRelease(mRasterizerState);
    SafeRelease(mBlendState);
    SafeRelease(mDepthStencilState);
}

