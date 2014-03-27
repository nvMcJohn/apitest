#pragma once

#include "solutions/untexturedobjectssoln.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class UntexturedObjectsD3D11Naive : public UntexturedObjectsSolution
{
public:
    UntexturedObjectsD3D11Naive();
    virtual ~UntexturedObjectsD3D11Naive() { }

    virtual bool Init(const std::vector<UntexturedObjectsProblem::Vertex>& _vertices, 
                      const std::vector<UntexturedObjectsProblem::Index>& _indices,
                      size_t _objectCount) override;

    virtual void Render(const std::vector<Matrix>& _transforms);
    virtual void Shutdown();

    virtual std::string GetName() const { return "D3D11Naive"; }

    virtual bool SupportsApi(EGfxApi _api) const override { return _api == EGfxApi::Direct3D11; }

private:
    struct ConstantsPerDraw
    {
        Matrix World;
    };

    struct ConstantsPerFrame
    {
        Matrix ViewProjection;
    };

    ID3D11InputLayout* mInputLayout;
    ID3D11Buffer* mConstantBufferPerFrame;
    ID3D11Buffer* mConstantBufferPerDraw;
    ID3D11VertexShader* mVertexShader;
    ID3D11PixelShader* mPixelShader;
    ID3D11RasterizerState* mRasterizerState;
    ID3D11BlendState* mBlendState;
    ID3D11DepthStencilState* mDepthStencilState;

    ID3D11Buffer* mVertexBuffer;
    ID3D11Buffer* mIndexBuffer;
};
