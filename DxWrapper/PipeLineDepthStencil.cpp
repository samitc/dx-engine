#include "PipeLineDepthStencil.h"
#include <utility>
#include "DXMain.h"
#include "Device.h"
#include "DxEnums.h"
const DepthStencilViewType PipeLineDepthStencil::getDepthStencil() const
{
	return (DepthStencilViewType)this->getView();
}
PipeLineDepthStencil::PipeLineDepthStencil(const PipeLineDepthStencil &copy) :PipeLineBuffer(copy)
{
}
PipeLineDepthStencil::PipeLineDepthStencil(const DXMain &dx, Texture2D &&tex) : PipeLineBuffer(dx.getDevice().createDepthStencilView(tex), std::move(tex))
{
}
BUILDER_GET_SET_METHOD_IMPL(dataFormat, DataFormat, PipeLineDepthStencil::PipeLineDepthStencilBuilder);