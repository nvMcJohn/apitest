#pragma once

#include "gfx.h"

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
class GfxApiDirect3D11 : public GfxBaseApi
{
public:
    GfxApiDirect3D11();
    virtual ~GfxApiDirect3D11();

    virtual bool Init(const std::string& _title, int _x, int _y, int _width, int _height) override;
    virtual void Shutdown() override;

    virtual void Activate() override;
    virtual void Deactivate() override;
    virtual void Clear(Vec4 _clearColor, GLfloat _clearDepth) override;
    virtual void SwapBuffers() override;

    virtual EGfxApi GetApiType() const { return EGfxApi::Direct3D11; }

    inline virtual const char* GetShortName() const override { return SGetShortName(); }
    inline virtual const char* GetLongName() const override { return SGetLongName(); }

    static const char* SGetShortName() { return "d3d11"; }
    static const char* SGetLongName() { return "Direct3D 11"; }

protected:
    IDXGISwapChain* mSwapChain;
    ID3D11RenderTargetView* mColorView; 
    ID3D11DepthStencilView* mDepthStencilView;

    virtual bool CreateSwapChain();
};

// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------
// DX11 Utilities
bool CompileProgram(const std::wstring& _vsFilename, ID3D11VertexShader** _outVertexShader,
                    const std::wstring& _psFilename, ID3D11PixelShader** _outPixelShader,
                    UINT _inputElementCount, const D3D11_INPUT_ELEMENT_DESC* _inputElementDescs, ID3D11InputLayout** _outInputLayout);

HRESULT CreateRenderTarget(IDXGISwapChain* dxgi_swap_chain, ID3D11RenderTargetView** out_render_target_view);
HRESULT CreateDepthBuffer(IDXGISwapChain* dxgi_swap_chain, ID3D11DepthStencilView** out_depth_stencil_view);
ID3D11Texture2D* NewTexture2DFromDetails(const TextureDetails& _texDetails);
ID3D11ShaderResourceView* NewTexture2DSRVFromDetails(const TextureDetails& _texDetails);

HRESULT CreateConstantBuffer(int size, const void* data, ID3D11Buffer** out_buffer);
HRESULT CreateDynamicVertexBuffer(int _size, const void* _data, ID3D11Buffer** _out_buffer);
HRESULT CreateDynamicVertexBuffer(int _size, const void* _data, D3D11_USAGE _usage, UINT _cpuAccessFlags, ID3D11Buffer** _outBuffer);

// --------------------------------------------------------------------------------------------------------------------
template <typename T>
inline ID3D11Buffer* CreateBufferFromVector(const std::vector<T>& _data, D3D11_USAGE _usage, UINT _bindFlags, UINT _cpuAccessFlags=0, UINT _miscFlags=0)
{
    // TODO: This should be refactored into a helper function that we call, but for now just do it all here.
    // D3D is so verbose. :(

    D3D11_BUFFER_DESC desc;
    desc.ByteWidth = sizeof(T) * _data.size();
    desc.Usage = _usage;
    desc.BindFlags = _bindFlags;
    desc.CPUAccessFlags = _cpuAccessFlags;
    desc.MiscFlags = _miscFlags;
    desc.StructureByteStride = sizeof(T);

    D3D11_SUBRESOURCE_DATA initialData;
    initialData.pSysMem = _data.data();
    initialData.SysMemPitch = 0;
    initialData.SysMemSlicePitch = 0;
    
    ID3D11Buffer* retBuffer = nullptr;
    if (FAILED(g_d3d_device->CreateBuffer(&desc, &initialData, &retBuffer))) {
        // TODO: Would like to log this failure--do that once this is refactored out of the header file.
        SafeRelease(retBuffer);
        return nullptr;
    }

    return retBuffer;    
}

// TODO: Cleanup formatting
// DX11 Globals
extern IDXGIFactory* g_dxgi_factory;
extern ID3D11Device* g_d3d_device;
extern D3D_FEATURE_LEVEL g_d3d_feature_level;
extern ID3D11DeviceContext* g_d3d_context;

