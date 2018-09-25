#include "GpuResourceManager.h"
#include "DXMain.h"
#include "Device.h"
#include "ResourceManager.h"
#include "DxEnums.h"
template <class T> Texture2D createPipeBuilder(const ResourceManager& resourceManager, const T& build, DataFormat format, ResourceFormat resFormat)
{
	Texture2D::Texture2DBuilder builder(build.getwidth(), build.getheight(), format);
	builder.setaccess((Access)(Access::GpuRead | Access::GpuWrite));
	builder.setformat(resFormat);
	builder.setsampleCount(build.getsampleCount());
	builder.setsampleQuality(build.getsampleQuality());
	return resourceManager.createTexture2D(builder);
}
GpuResourceManager::GpuResourceManager(const DXMain &dx, const ResourceManager& resourceManager) :dx(dx),resourceManager(resourceManager)
{
}
PipeLineRenderTarget GpuResourceManager::createRenderTarget(const PipeLineRenderTarget::PipeLineRenderTargetBuilder &build) const
{
	
	return PipeLineRenderTarget(dx, createPipeBuilder(resourceManager,build, build.getdataFormat(),ResourceFormat::RENDER_TARGET));
}
PipeLineDepthStencil GpuResourceManager::createDepthStencil(const PipeLineDepthStencil::PipeLineDepthStencilBuilder &build) const
{
	return PipeLineDepthStencil(dx, createPipeBuilder(resourceManager, build, DataFormat::D24_UNORM_S8_UINT, ResourceFormat::DEPTH_STENCIL));
}
UnorderedView GpuResourceManager::createUnorderedView(const Resource &res, const UnorderedViewInfo &info) const
{
	return dx.getDevice().createUnorderedView(res, info);
}
ShaderResource GpuResourceManager::createShaderResource(const Resource &res, const ShaderResourceInfo &info) const
{
	return dx.getDevice().createShaderResource(res, info);
}