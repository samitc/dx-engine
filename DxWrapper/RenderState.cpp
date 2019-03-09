#include "RenderState.h"
#include "DXMain.h"
#include "Device.h"
RenderState::RenderState():renderState(NULL)
{
}
RenderState::RenderState(const RenderState &copy):renderState(copy.renderState)
{
	#if CLDLEVEL >= 0
	if(this->getRenderState()!=NULL)
	{
	#endif
	 this->getRenderState()->AddRef();
	#if CLDLEVEL >= 0
	}
	#endif
}
RenderState::RenderState(RenderStateType renderState) :renderState(renderState)
{
#if CLDLEVEL >= 4
	static int number = 0;
	char name[] = "Render state:   ";
	char *p = name + strlen(name) - 1;
	int num = number++;
	while (num > 0)
	{
		*p = (num % 10) + 48;
		num /= 10;
		p--;
	}
	renderState->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
#endif
}
RenderState RenderState::RenderStateBuilder::build(const DXMain & dx) const
{
	auto &device = dx.getDevice();
	D3D11_RASTERIZER_DESC renderStateDesc;
	ZeroMemory(&renderStateDesc, sizeof(renderStateDesc));
	renderStateDesc.FillMode = (D3D11_FILL_MODE)(int) this->getfill();
	renderStateDesc.CullMode = (D3D11_CULL_MODE)(int)this->getcull();
	renderStateDesc.FrontCounterClockwise = this->getfrontCounterClockwise();
	renderStateDesc.DepthBias = this->getdepthBias();
	renderStateDesc.DepthBiasClamp = this->getdepthBiasClamp();
	renderStateDesc.SlopeScaledDepthBias = this->getslopeScaledDepthBias();
	renderStateDesc.DepthClipEnable = this->getdepthClipEnable();
	renderStateDesc.ScissorEnable = this->getscissorEnable();
	renderStateDesc.MultisampleEnable = this->getmultisampleEnable();
	renderStateDesc.AntialiasedLineEnable = this->getantialiasedLineEnable();
	return RenderState(device.createRenderState(renderStateDesc));
}
RenderState::RenderStateBuilder::RenderStateBuilder() :RenderStateBuilder(WIREFRAME, BACK, false, 0)
{
}
RenderState::RenderStateBuilder::RenderStateBuilder(FillMode fill):RenderStateBuilder(fill,BACK,false,0)
{
}
RenderState::RenderStateBuilder::RenderStateBuilder(FillMode fill, CullMode cull) :RenderStateBuilder(fill, cull, false, 0)
{
}
RenderState::RenderStateBuilder::RenderStateBuilder(FillMode fill, CullMode cull, bool frontCounterClockwise):RenderStateBuilder(fill,cull,frontCounterClockwise,0)
{
}
RenderState::RenderStateBuilder::RenderStateBuilder(FillMode fill, CullMode cull, bool frontCounterClockwise, int depthBias, float depthBiasClamo, float slopeScaledDepthBias,bool depthClipEnable, bool scissorEnable, bool multisampleEnable, bool antialiasedLineEnable)
{
	this->setfill(fill);
	this->setcull(cull);
	this->setfrontCounterClockwise(frontCounterClockwise);
	this->setdepthBias(depthBias);
	this->setdepthBiasClamp(depthBiasClamp);
	this->setslopeScaledDepthBias(slopeScaledDepthBias);
	this->setdepthClipEnable(depthClipEnable);
	this->setscissorEnable(scissorEnable);
	this->setmultisampleEnable(multisampleEnable);
	this->setantialiasedLineEnable(antialiasedLineEnable);
}
RenderState::~RenderState()
{
	#if CLDLEVEL >= 0
	if(this->getRenderState()!=NULL)
	{
	#endif
		this->getRenderState()->Release();
	#if CLDLEVEL >= 0
	}
	#endif
}
void RenderState::apply(const DXMain &dx) const
{
	dx.getDevice().setRenderState(this->getRenderState());
}
RenderState & RenderState::operator=(RenderState & obj)
{
	if (this != &obj)
	{
	 #if CLDLEVEL >= 0
  	if(this->getRenderState()!=NULL)
	 {
	 #endif
	  	this->getRenderState()->Release();
	 #if CLDLEVEL >= 0
	 }
	 #endif
		this->setRenderState(obj.getRenderState());
	 #if CLDLEVEL >= 0
  	if(this->getRenderState()!=NULL)
  	{
  	#endif
  		this->getRenderState()->AddRef();
	 #if CLDLEVEL >= 0
		}
		#endif
	}
	return *this;
}
void RenderState::setRenderState(RenderStateType state)
{
	this->renderState = state;
}
RenderStateType RenderState::getRenderState() const
{
	return this->renderState;
}
int RenderState::getId() const
{
	return 2;
}
RenderState *RenderState::clone() const
{
	return new RenderState(*this);
}
BUILDER_GET_SET_METHOD_IMPL(fill, RenderState::RenderStateBuilder::FillMode, RenderState::RenderStateBuilder)
BUILDER_GET_SET_METHOD_IMPL(cull, RenderState::RenderStateBuilder::CullMode, RenderState::RenderStateBuilder)
BUILDER_GET_SET_METHOD_IMPL(frontCounterClockwise, bool, RenderState::RenderStateBuilder)
BUILDER_GET_SET_METHOD_IMPL(depthBias, int, RenderState::RenderStateBuilder)
BUILDER_GET_SET_METHOD_IMPL(depthBiasClamp, float, RenderState::RenderStateBuilder)
BUILDER_GET_SET_METHOD_IMPL(slopeScaledDepthBias, float, RenderState::RenderStateBuilder)
BUILDER_GET_SET_METHOD_IMPL(depthClipEnable, bool, RenderState::RenderStateBuilder)
BUILDER_GET_SET_METHOD_IMPL(scissorEnable, bool, RenderState::RenderStateBuilder)
BUILDER_GET_SET_METHOD_IMPL(multisampleEnable, bool, RenderState::RenderStateBuilder)
BUILDER_GET_SET_METHOD_IMPL(antialiasedLineEnable, bool, RenderState::RenderStateBuilder)