#pragma once

#include "solutions/dynamicstreamingsoln.h"
#include "framework/gfx_dx11.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class DynamicStreamingD3D11UpdateSubresource : public DynamicStreamingSolution
{
public:
    DynamicStreamingD3D11UpdateSubresource();
    virtual ~DynamicStreamingD3D11UpdateSubresource();

    virtual bool Init(size_t _maxVertexCount) override;
    virtual void Render(const std::vector<Vec2>& _vertices) override;
    virtual void Shutdown() override;

    virtual std::string GetName() const override { return "DynamicStreamingD3D11UpdateSubresource"; }
    virtual bool SupportsApi(EGfxApi _api) const override { return _api == EGfxApi::Direct3D11; }

private:
    struct Constants
    {
        float width;
        float height;
        float pad[2];
    };

    ID3D11InputLayout* mInputLayout;
    ID3D11Buffer* mConstantBuffer;
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;
    ID3D11SamplerState* mSamplerState;
    ID3D11RasterizerState* mRasterizerState;
    ID3D11BlendState* mBlendState;
    ID3D11DepthStencilState* mDepthStencilState;

    ID3D11Buffer* mDynamicVertexBuffer;

    size_t mParticleBufferSize;
    size_t mStartDestOffset;
};
