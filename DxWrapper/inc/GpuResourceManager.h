#ifndef _GPURESOURCMANAGER_H_
#define _GPURESOURCMANAGER_H_
#include "ShaderResource.h"
#include "UnorderedView.h"
#include "PipeLineRenderTarget.h"
#include "PipeLineDepthStencil.h"
class UnorderedViewInfo;
class DXMain;
class ResourceManager;
class GpuResourceManager
{
public:
	GpuResourceManager(const DXMain&,const ResourceManager&);
	~GpuResourceManager() = default;
	PipeLineRenderTarget createRenderTarget(const PipeLineRenderTarget::PipeLineRenderTargetBuilder&) const;
	PipeLineDepthStencil createDepthStencil(const PipeLineDepthStencil::PipeLineDepthStencilBuilder&) const;
	UnorderedView createUnorderedView(const Resource &, const UnorderedViewInfo&) const;
	ShaderResource createShaderResource(const Resource &, const ShaderResourceInfo &) const;
private:
	const DXMain &dx;
	const ResourceManager &resourceManager;
};
#endif