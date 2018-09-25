#include "PipeLineRenderTarget.h"
#include "DXMain.h"
#include "Device.h"
PipeLineRenderTarget::PipeLineRenderTarget(const DXMain& dx, Texture2D && text) :PipeLineBuffer(dx.getDevice().createRenderTargetView(text), std::move(text))
{
}
PipeLineRenderTarget::PipeLineRenderTarget(const PipeLineRenderTarget &copy) : PipeLineBuffer(copy)
{
}
const RenderTargetViewType PipeLineRenderTarget::getRenderTarget() const
{
	return (RenderTargetViewType)getView();
}
BUILDER_GET_SET_METHOD_IMPL(dataFormat, DataFormat, PipeLineRenderTarget::PipeLineRenderTargetBuilder);