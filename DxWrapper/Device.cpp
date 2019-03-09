#include "Device.h"
#include "DXMain.h"
#include "PipeLineState.h"
#include <string>
#include "Texture2D.h"
#include "DXObject.h"
#include "Data.h"
#include "Color.h"
#include "ShaderResource.h"
#include "Buffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "inputLayout.h"
#include "ShaderResourceInfo.h"
#include "ShortPoolMemory.h"
#include "DxResException.h"
#include "GeneralException.h"
#include "BaseLog.h"
#include "UnorderedView.h"
#include "UnorderedViewInfo.h"
#include "DrawCommand.h"
#include "PipeState.h"
#include "GraphicState.h"
Device::Device(DXMain &dx) :dx(&dx)
{
}
Device::~Device()
{
}
DXObject* Device::getDX() const
{
	return &dx->getDx();
}
RenderTargetViewType Device::createRenderTargetView(const Texture2D & res) const
{
	RenderTargetViewType ret;
	resValid(this->getDX()->getDevice()->CreateRenderTargetView(res.getTexture(), 0, &ret));
	return ret;
}
DepthStencilViewType Device::createDepthStencilView(const Texture2D &res) const
{
	DepthStencilViewType ret;
	resValid(this->getDX()->getDevice()->CreateDepthStencilView(res.getTexture(), 0, &ret));
	return ret;
}
void Device::applyViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth) const
{
	ViewPort viewport;
	viewport.TopLeftX = topLeftX;
	viewport.TopLeftY = topLeftY;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = minDepth;
	viewport.MaxDepth = maxDepth;
	this->getDX()->getContext()->RSSetViewports(1, &viewport);
}
void Device::setDrawCommand(const DrawCommand &draw)
{
	const PipeState& pipe = draw.getpipe();
	GraphicState *s = pipe.getstates().size() > 0 ? pipe.getstates()[0] : nullptr;
	dx->getPipeLineState()->reset(s, pipe.getshaders());
	for (const auto& state : pipe.getstates())
	{
		dx->getPipeLineState()->setGraphicState(state);
	}
	dx->getPipeLineState()->setInputLayout(draw.getlayout());
	dx->getPipeLineState()->setPrimitiveTopology(draw.gettopology());
}
PixelShaderType Device::createPixelShader(const void *data, unsigned int size) const
{
	PixelShaderType ret;
	resValid(this->getDX()->getDevice()->CreatePixelShader(data, size, 0, &ret));
	return ret;
}
VertexShaderType Device::createVertexShader(const void *data, unsigned int size) const
{
	VertexShaderType ret;
	resValid(this->getDX()->getDevice()->CreateVertexShader(data,size, 0, &ret));
	return ret;
}
GeometryShaderType Device::createGeometryShader(const void *data, unsigned int size) const
{
	GeometryShaderType ret;
	resValid(this->getDX()->getDevice()->CreateGeometryShader(data,size, 0, &ret));
	return ret;
}
ComputeShaderType Device::createComputeShader(const void *data, unsigned int size) const
{
	ComputeShaderType ret;
	resValid(this->getDX()->getDevice()->CreateComputeShader(data,size, 0, &ret));
	return ret;
}
GeometryShaderType Device::createGeometryStreamOutputShader(const void *data, unsigned int size, D3D11_SO_DECLARATION_ENTRY *entries, int count) const
{
	GeometryShaderType ret;
	resValid(this->getDX()->getDevice()->CreateGeometryShaderWithStreamOutput(data,size, entries, count, nullptr, 0, 0, nullptr, &ret));
	return ret;
}
HullShaderType Device::createHullShader(const void *data, unsigned int size) const
{
	HullShaderType ret;
	resValid(this->getDX()->getDevice()->CreateHullShader(data,size, nullptr, &ret));
	return ret;
}
DomainShaderType Device::createDomainShader(const void *data, unsigned int size) const
{
	DomainShaderType ret;
	resValid(this->getDX()->getDevice()->CreateDomainShader(data,size, nullptr, &ret));
	return ret;
}
BufferType Device::createBuffer(D3D11_BUFFER_DESC &desc, D3D11_SUBRESOURCE_DATA *res) const
{
	ID3D11Buffer* ret;
	resValid(this->getDX()->getDevice()->CreateBuffer(&desc, res, &ret));
	return ret;
}
void Device::updateSubresource(ResourceType res, const void *data, unsigned int dstSubRes, unsigned int dataRow, unsigned int dataDepth) const
{
	this->getDX()->getContext()->UpdateSubresource(res, dstSubRes, 0, data, dataRow, dataDepth);
}
ShaderResource Device::createShaderResource(const Resource &res, const ShaderResourceInfo &info) const
{
    ShaderResourceType srt;
    ShaderResourceInfoType inf;
    resValid(this->getDX()->getDevice()->CreateShaderResourceView(res.getResource(), info.getShaderResourceInfoType(&inf), &srt));
    return ShaderResource(srt);
}
UnorderedView Device::createUnorderedView(const Resource & res, const UnorderedViewInfo & info) const
{
	UnorderedViewType un;
	UnorderedResourceInfoType inf;
	this->getDX()->getDevice()->CreateUnorderedAccessView(res.getResource(), info.getUnorderedInfoType(&inf), &un);
	return UnorderedView(un);
}
UsageType Device::getUsageType(UINT access)
{
	const UINT DEFAULT = Access::GpuRead | Access::GpuWrite;
	const UINT IMMUTABLE = Access::GpuRead;
	const UINT DYNAMIC = Access::GpuRead | Access::CpuWrite;
	const UINT STAGING = Access::GpuRead | Access::CpuRead | Access::CpuWrite;
	if (access == DEFAULT)
	{
		return D3D11_USAGE::D3D11_USAGE_DEFAULT;
	}
	else if (access == IMMUTABLE)
	{
		return D3D11_USAGE::D3D11_USAGE_IMMUTABLE;
	}
	else if (access == DYNAMIC)
	{
		return D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	}
	else if (access == STAGING)
	{
		return D3D11_USAGE::D3D11_USAGE_STAGING;
	}
	else
	{
		std::string logStr("can not find access:");
		logStr.append(std::to_string((Access)access));
		BaseLog::getInstance().warn(logStr.c_str());
		throw GeneralException("error in access in buffer creation");
	}
}
void Device::setRenderState(RenderStateType renderStateType) const
{
	this->getDX()->getContext()->RSSetState(renderStateType);
}
void Device::setBlendState(BlendStateType blendStateType, Color &col) const
{
	this->getDX()->getContext()->OMSetBlendState(blendStateType, col.getFloat(*dx->getShortPoolMemory()), -1);
}
void Device::soSetBuffer(int count, BufferType * buffers) const
{
	this->getDX()->getContext()->SOSetTargets(count, buffers, nullptr);
}
void Device::setVertexBuffer(unsigned int startSlot, const Device::BufferData &buffer, unsigned int offset) const
	{
	this->getDX()->getContext()->IASetVertexBuffers(startSlot, buffer.numOfBuffers, buffer.buffers, buffer.sizeOfBuffers, &offset);
}
void Device::setIndexBuffer(const BufferType &buffer, unsigned int offset, int sizeOfIndex) const
{
	if (sizeOfIndex == 2)
	{
		this->getDX()->getContext()->IASetIndexBuffer(buffer, DXGI_FORMAT::DXGI_FORMAT_R16_UINT, offset);
	}
	else
{
		this->getDX()->getContext()->IASetIndexBuffer(buffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, offset);
	}
}
void Device::setPrimitiveTopology(PrimitiveTopology primitive) const
{
	this->getDX()->getContext()->IASetPrimitiveTopology((D3D_PRIMITIVE_TOPOLOGY)(int)primitive);
}
void Device::drawIndexBuffer(unsigned int count, unsigned int startIndex, unsigned int baseVertexIndex) const
{
	dx->getPipeLineState()->apply(*dx);
	this->getDX()->getContext()->DrawIndexed(count, startIndex, baseVertexIndex);
}
void Device::drawBuffer(unsigned int count, unsigned int startIndex) const
{
	dx->getPipeLineState()->apply(*dx);
	this->getDX()->getContext()->Draw(count, startIndex);
}
void Device::drawInstance(unsigned int vertexCountPerInstance, unsigned int instanceCount, unsigned int startVertexLocation, unsigned int startInstanceLocation) const
{
	dx->getPipeLineState()->apply(*dx);
	getDX()->getContext()->DrawInstanced(vertexCountPerInstance, instanceCount, startVertexLocation, startInstanceLocation);
}
void Device::drawIndexInstance(unsigned int indexCountPerInstance, unsigned int instanceCount, unsigned int startIndexLocation, int baseVertexLocation, unsigned int startInstanceLocation) const
{
	dx->getPipeLineState()->apply(*dx);
	getDX()->getContext()->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}
void Device::setVertexShader(const VertexShaderType shader) const
{
	this->getDX()->getContext()->VSSetShader(shader, 0, 0);
}
void Device::setComputeShader(const ComputeShaderType shader) const
{
	this->getDX()->getContext()->CSSetShader(shader, 0, 0);
}
void Device::setPixelShader(const PixelShaderType shader) const
{
	this->getDX()->getContext()->PSSetShader(shader, 0, 0);
}
void Device::setGeometryShader(const GeometryShaderType shader) const
{
	this->getDX()->getContext()->GSSetShader(shader, 0, 0);
}
void Device::setHullShader(const HullShaderType shader) const
{
	this->getDX()->getContext()->HSSetShader(shader, nullptr, 0);
}
void Device::setDomainShader(const DomainShaderType shader) const
{
	this->getDX()->getContext()->DSSetShader(shader, nullptr, 0);
}
void Device::dispatchComputeShader(unsigned int x, unsigned int y, unsigned int z) const
{
	this->getDX()->getContext()->Dispatch(x, y, z);
}
void Device::setInputLayout(const InputLayoutType layout) const
{
	this->getDX()->getContext()->IASetInputLayout(layout);
}
SamplerStateType Device::createSamplerState(D3D11_SAMPLER_DESC &desc) const
{
	SamplerStateType ret;
	resValid(this->getDX()->getDevice()->CreateSamplerState(&desc, &ret));
	return ret;
}
RenderStateType Device::createRenderState(D3D11_RASTERIZER_DESC &desc) const
{
	RenderStateType ret;
	resValid(this->getDX()->getDevice()->CreateRasterizerState(&desc, &ret));
	return ret;
}
BlendStateType Device::createBlendState(D3D11_BLEND_DESC &desc)
{
	BlendStateType ret;
	resValid(this->getDX()->getDevice()->CreateBlendState(&desc, &ret));
	return ret;
}
InputLayoutType Device::createInputLayout(D3D11_INPUT_ELEMENT_DESC * elements, int numberOfElements, BYTE * data, SIZE_T l) const
{
	InputLayoutType ret;
	resValid(this->getDX()->getDevice()->CreateInputLayout(elements, numberOfElements, data, l, &ret));
	return ret;
}
MapSubResourceType Device::createMapSubResourceType(ResourceType resource, UINT subResource, MapType mapType, bool waitForGpu) const
{
	D3D11_MAPPED_SUBRESOURCE ret;
	HRESULT res = this->getDX()->getContext()->Map(resource, subResource, (D3D11_MAP)(int)mapType, waitForGpu ? 0 : 0x100000L, &ret);
	if (SUCCEEDED(res))
	{
		return ret;
	}
	else if (res == DXGI_ERROR_WAS_STILL_DRAWING)
	{
		ret.pData = nullptr;
	}
	else
	{
		resValid(res);
	}
	return ret;
}
void Device::applyMapSubResource(ResourceType resource, UINT subResource) const
{
	this->getDX()->getContext()->Unmap(resource, subResource);
}
DxEffect Device::createEffect(const void * data, SIZE_T l) const
{
	DxEffect effect;
	resValid(D3DX11CreateEffectFromMemory(data, l, 0, this->getDX()->getDevice(), &effect));
	return effect;
}
Texture2DType Device::createTexture2D(const TextureDesc & desc) const
{
	Texture2DType ret;
	resValid(this->getDX()->getDevice()->CreateTexture2D(&desc, nullptr, &ret));
	return ret;
}
void Device::copyResource(ResourceType resTo, ResourceType resFrom) const
{
#if CLDLEVEL >= 4
	if (this->getDX()->getContext() != NULL)
	{
#endif
		this->getDX()->getContext()->CopyResource(resTo, resFrom);
#if CLDLEVEL >= 4
	}
#endif
}
Device::BufferData::BufferData(BufferType * buffers, unsigned int * sizeOfBuffers, unsigned int numOfBuffers) :buffers(buffers), sizeOfBuffers(sizeOfBuffers), numOfBuffers(numOfBuffers)
{
}
