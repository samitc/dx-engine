#ifndef _DXTYPES_H_
#define _DXTYPES_H_
#ifdef DX_11
#include<d3d11.h>
#include<d3dx11effect.h>
typedef ID3D11Device* DxDevice;
typedef ID3D11DeviceContext* DxContext;
typedef ID3D11PixelShader* PixelShaderType;
typedef ID3D11GeometryShader* GeometryShaderType;
typedef ID3D11HullShader* HullShaderType;
typedef ID3D11DomainShader* DomainShaderType;
typedef IDXGISwapChain* RenderDeviceType;
typedef ID3D11Resource*	ResourceType;
typedef	ID3D11ShaderResourceView*	ShaderResourceType;
typedef ID3D11UnorderedAccessView* UnorderedViewType;
typedef ID3D11View* ResourceViewType;
typedef ResourceViewType* ResourceViewsType;
typedef ID3D11Texture2D* Texture2DType;
typedef ID3D11Texture2D Texture2DTypeS;
typedef ID3D11VertexShader* VertexShaderType;
typedef ID3D11ComputeShader* ComputeShaderType;
typedef IDXGIAdapter* VideoAdapterType;
typedef DXGI_MODE_DESC VideoModeType;
typedef IDXGIOutput *VideoOutputType;
typedef ID3D11Buffer* BufferType;
typedef D3D11_SHADER_RESOURCE_VIEW_DESC ShaderResourceInfoType;
typedef D3D11_UNORDERED_ACCESS_VIEW_DESC UnorderedResourceInfoType;
typedef D3D11_USAGE UsageType;
typedef ID3D11SamplerState *SamplerStateType;
typedef ID3D11RasterizerState* RenderStateType;
typedef ID3D11BlendState* BlendStateType;
typedef ID3D11InputLayout *InputLayoutType;
typedef ID3D11RenderTargetView* RenderTargetViewType;
typedef ID3D11DepthStencilView* DepthStencilViewType;
typedef D3D11_MAPPED_SUBRESOURCE MapSubResourceType;
typedef D3D11_VIEWPORT ViewPort;
typedef DXGI_FORMAT DxFormat;
typedef D3D11_SRV_DIMENSION DxShaderType;
typedef D3D11_UAV_DIMENSION DxUnoederedType;
typedef ID3DX11Effect *DxEffect;
typedef ID3DX11EffectTechnique *efTech;
typedef ID3DX11EffectMatrixVariable* efMatrixVar;
typedef ID3DX11EffectVectorVariable* efVectorVar;
typedef ID3DX11EffectScalarVariable* efScalarVar;
typedef D3DX11_PASS_DESC	EffectPassDesc;
typedef D3D11_TEXTURE2D_DESC TextureDesc;
typedef ID3DBlob	Blob;
#else ifdef DX_9
#endif
#endif