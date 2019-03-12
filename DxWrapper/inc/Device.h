#ifndef _DEVICE_H_
#define _DEVICE_H_
#include <memory>
#include "DXGame.h"
#include "DXTypes.h"
class DXObject;
class Color;
class Resource;
class Texture2D;
class Texture;
class ShaderResource;
class ShaderResourceInfo;
class UnorderedViewInfo;
class RenderResource;
class UnorderedView;
class Shader;
class Data;
class VertexShader;
class PixelShader;
class InputLayout;
class GOEntity;
class Buffer;
class PipeLineState;
class ShortPoolMemory;
class DxHandler;
class DXMain;
class DrawCommand;
enum MapType;
//enum that represent available draw option
enum PrimitiveTopology
{
	UNDEFINED = 0,
	POINTLIST = 1,
	LINELIST = 2,
	LINESTRIP = 3,
	TRIANGLELIST = 4,
	TRIANGLESTRIP = 5,
	LINELIST_ADJ = 10,
	LINESTRIP_ADJ = 11,
	TRIANGLELIST_ADJ = 12,
	TRIANGLESTRIP_ADJ = 13,
	CONTROL_POINT_1_PATCHLIST = 33,
	CONTROL_POINT_2_PATCHLIST = 34,
	CONTROL_POINT_3_PATCHLIST = 35,
	CONTROL_POINT_4_PATCHLIST = 36,
	CONTROL_POINT_5_PATCHLIST = 37,
	CONTROL_POINT_6_PATCHLIST = 38,
	CONTROL_POINT_7_PATCHLIST = 39,
	CONTROL_POINT_8_PATCHLIST = 40,
	CONTROL_POINT_9_PATCHLIST = 41,
	CONTROL_POINT_10_PATCHLIST = 42,
	CONTROL_POINT_11_PATCHLIST = 43,
	CONTROL_POINT_12_PATCHLIST = 44,
	CONTROL_POINT_13_PATCHLIST = 45,
	CONTROL_POINT_14_PATCHLIST = 46,
	CONTROL_POINT_15_PATCHLIST = 47,
	CONTROL_POINT_16_PATCHLIST = 48,
	CONTROL_POINT_17_PATCHLIST = 49,
	CONTROL_POINT_18_PATCHLIST = 50,
	CONTROL_POINT_19_PATCHLIST = 51,
	CONTROL_POINT_20_PATCHLIST = 52,
	CONTROL_POINT_21_PATCHLIST = 53,
	CONTROL_POINT_22_PATCHLIST = 54,
	CONTROL_POINT_23_PATCHLIST = 55,
	CONTROL_POINT_24_PATCHLIST = 56,
	CONTROL_POINT_25_PATCHLIST = 57,
	CONTROL_POINT_26_PATCHLIST = 58,
	CONTROL_POINT_27_PATCHLIST = 59,
	CONTROL_POINT_28_PATCHLIST = 60,
	CONTROL_POINT_29_PATCHLIST = 61,
	CONTROL_POINT_30_PATCHLIST = 62,
	CONTROL_POINT_31_PATCHLIST = 63,
	CONTROL_POINT_32_PATCHLIST = 64,
};
//public for engine
class Device
{
public://public for everyone
	Device(DXMain &dx);
	~Device();
	void applyViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth) const;
public://public for engine
	void setDrawCommand(const DrawCommand&);
	struct BufferData
	{
	public:
		BufferData(BufferType* buffers, unsigned int *sizeOfBuffers, unsigned int numOfBuffers);
		BufferType* buffers;
		unsigned int *sizeOfBuffers;
		unsigned int numOfBuffers;
	};
	ShaderResource createShaderResource(const Resource &res, const ShaderResourceInfo &info) const;
	UnorderedView createUnorderedView(const Resource &res, const UnorderedViewInfo& info) const;
	void setVertexShader(const VertexShaderType shader) const;
	void setComputeShader(const ComputeShaderType shader) const;
	void setPixelShader(const PixelShaderType shader) const;
	void setGeometryShader(const GeometryShaderType shader) const;
	void setHullShader(const HullShaderType shader) const;
	void setDomainShader(const DomainShaderType shader) const;
	void dispatchComputeShader(unsigned int x, unsigned int y, unsigned int z) const;
	void setInputLayout(const InputLayoutType layout) const;
	void setVertexBuffer(unsigned int startSlot, const Device::BufferData &buffer, unsigned int offset) const;
	void setIndexBuffer(const BufferType &buffer, unsigned int offset, int sizeOfIndex) const;
	void setPrimitiveTopology(PrimitiveTopology primitive) const;
	void drawIndexBuffer(unsigned int count, unsigned int startIndex, unsigned int baseVertexIndex) const;
	void drawBuffer(unsigned int count, unsigned int startIndex) const;
	void drawInstance(unsigned int vertexCountPerInstance, unsigned int instanceCount, unsigned int startVertexLocation, unsigned int startInstanceLocation) const;
	void drawIndexInstance(unsigned int indexCountPerInstance, unsigned int instanceCount, unsigned int startIndexLocation, int baseVertexLocation, unsigned int startInstanceLocation) const;
	DXObject* getDX() const;
	RenderTargetViewType createRenderTargetView(const Texture2D&) const;
	DepthStencilViewType createDepthStencilView(const Texture2D&) const;
	PixelShaderType createPixelShader(const void *data, unsigned int size) const;
	VertexShaderType createVertexShader(const void *data,unsigned int size) const;
	GeometryShaderType createGeometryShader(const void *data, unsigned int size) const;
	ComputeShaderType createComputeShader(const void *data, unsigned int size) const;
	GeometryShaderType createGeometryStreamOutputShader(const void *data, unsigned int size, D3D11_SO_DECLARATION_ENTRY*, int) const;
	HullShaderType createHullShader(const void *data, unsigned int size) const;
	DomainShaderType createDomainShader(const void *data, unsigned int size) const;
	BufferType createBuffer(D3D11_BUFFER_DESC &desc, D3D11_SUBRESOURCE_DATA *res) const;
	void updateSubresource(ResourceType res, const void *data, unsigned int dstSubRes = 0, unsigned int dataRow = 0, unsigned int dataDepth = 0) const;
	static UsageType getUsageType(UINT access);
	void soSetBuffer(int count, BufferType * buffers) const;
	void setRenderState(RenderStateType renderStateType) const;
	void setBlendState(BlendStateType blendStateType,Color &col) const;
	SamplerStateType createSamplerState(D3D11_SAMPLER_DESC &desc) const;
	RenderStateType createRenderState(D3D11_RASTERIZER_DESC &desc) const;
	BlendStateType createBlendState(D3D11_BLEND_DESC &desc);
	InputLayoutType createInputLayout(D3D11_INPUT_ELEMENT_DESC *elements, int numberOfElements, BYTE * data, SIZE_T l) const;
	MapSubResourceType createMapSubResourceType(ResourceType resource, UINT subResource, MapType mapType, bool waitForGpu) const;
	void applyMapSubResource(ResourceType resource, UINT subResource) const;
	DxEffect createEffect(const void* data, SIZE_T l) const;
	Texture2DType createTexture2D(const TextureDesc &desc) const;
	void copyResource(ResourceType resTo, ResourceType resFrom) const;
protected:
private:
	Device& operator=(Device& obj) = delete;
	DXMain* dx;
    std::unique_ptr<PipeLineState> pipeLine;
};
#endif