#include "BlendState.h"
#include <iterator>
#include <new>
#include "Device.h"
#include "DXMain.h"
#include "Color.h"
BlendState BlendState::BlendStateBuilder::build(DXMain &dx) const
{
	D3D11_BLEND_DESC blendStateDesc = { 0 };
	blendStateDesc.AlphaToCoverageEnable = this->getalphaToCoverageEnable();
	blendStateDesc.IndependentBlendEnable = this->getindependentBlendEnable();
	std::list<BlendStateTarget>::const_iterator s = this->getBlendState().begin(), e = this->getBlendState().end();
	register unsigned char i = 0;
	while (s != e)
	{
		blendStateDesc.RenderTarget[i] = (*s).getStateTarget();
		++s;
	}
	return BlendState(dx.getDevice().createBlendState(blendStateDesc), this->color);
}
BlendState::BlendStateBuilder::BlendStateBuilder() :BlendStateBuilder(false, false, 0)
{
}
BlendState::BlendStateBuilder::BlendStateBuilder(bool alphaToCoverageEnable, bool independentBlendEnable, Color &col) : BlendStateBuilder(alphaToCoverageEnable, independentBlendEnable, 0, NULL, col)
{
}
BlendState::BlendStateBuilder::BlendStateBuilder(bool alphaToCoverageEnable, bool independentBlendEnable, int numOfBlendStateTarget, const BlendStateTarget* const blendState, Color &col)
	:alphaToCoverageEnable(alphaToCoverageEnable),independentBlendEnable(independentBlendEnable), color(col)
{
	for (char i = 0; i < numOfBlendStateTarget; i++)
	{
		this->getblendState().push_back(blendState[i]);
	}
}
BlendState::BlendStateBuilder::BlendStateBuilder(bool alphaToCoverageEnable, bool independentBlendEnable, std::vector<BlendStateTarget> blendState, Color & col) :BlendStateBuilder(alphaToCoverageEnable, independentBlendEnable, blendState.size(), &blendState[0], col)
{
}
BlendState::BlendStateBuilder::BlendStateBuilder(const BlendStateBuilder &copy) : BlendStateBuilder(copy.getalphaToCoverageEnable(), copy.getindependentBlendEnable(), copy.getcolor())
{
	std::copy(copy.blendState.begin(), copy.blendState.end(), std::back_insert_iterator<std::list<BlendStateTarget>>(this->blendState));
	}
BlendState::BlendStateBuilder& BlendState::BlendStateBuilder::addBlendStateTarget(const BlendStateTarget &blendStateTarget)
{
	if (this->getblendState().size() < 8)
	{
		this->getblendState().push_back(blendStateTarget);
	}
	return *this;
}
BlendState::BlendStateBuilder::~BlendStateBuilder()
{
}
Color BlendState::BlendStateBuilder::getcolor() const
{
	return this->color;
}
BlendState::BlendStateBuilder& BlendState::BlendStateBuilder::setcolor(const Color& col)
{
	this->color = col;
	return *this;
}
BlendState::BlendStateBuilder& BlendState::BlendStateBuilder::operator=(BlendStateBuilder & obj)
{
	if (this != &obj)
	{
		this->setalphaToCoverageEnable(obj.getalphaToCoverageEnable());
		this->setcolor(obj.getcolor());
		this->setindependentBlendEnable(obj.getindependentBlendEnable());
		std::copy(obj.blendState.begin(), obj.blendState.end(), std::back_insert_iterator<std::list<BlendStateTarget>>(this->blendState));
	}
		return *this;
	}
const std::list<BlendState::BlendStateBuilder::BlendStateTarget>& BlendState::BlendStateBuilder::getBlendState() const
{
	return this->blendState;
}
std::list<BlendState::BlendStateBuilder::BlendStateTarget>& BlendState::BlendStateBuilder::getblendState()
{
	return this->blendState;
}
BUILDER_GET_SET_METHOD_IMPL(alphaToCoverageEnable, bool, BlendState::BlendStateBuilder);
BUILDER_GET_SET_METHOD_IMPL(independentBlendEnable, bool, BlendState::BlendStateBuilder);
BlendState::BlendStateBuilder::BlendStateTarget::BlendStateTarget()
{
}
BlendState::BlendStateBuilder::BlendStateTarget::BlendStateTarget(const BlendStateTarget& copy)
{
	this->enable = copy.enable;
	this->srcBlend = copy.srcBlend;
	this->destBlend = copy.destBlend;
	this->blendOp = copy.blendOp;
	this->srcBlendAlpha = copy.srcBlendAlpha;
	this->destBlendAlpha = copy.destBlendAlpha;
	this->blendOpAlpha = copy.blendOpAlpha;
	this->renderTargetWriteMask = copy.renderTargetWriteMask;
}
D3D11_RENDER_TARGET_BLEND_DESC BlendState::BlendStateBuilder::BlendStateTarget::getStateTarget() const
{
	D3D11_RENDER_TARGET_BLEND_DESC ret;
	ret.BlendEnable = this->enable;
	ret.SrcBlend = (D3D11_BLEND)(int)this->srcBlend;
	ret.DestBlend = (D3D11_BLEND)(int)this->destBlend;
	ret.BlendOp = (D3D11_BLEND_OP)(int)this->blendOp;
	ret.SrcBlendAlpha = (D3D11_BLEND)(int) this->srcBlendAlpha;
	ret.DestBlendAlpha = (D3D11_BLEND)(int)this->srcBlendAlpha;
	ret.BlendOpAlpha = (D3D11_BLEND_OP)(int)this->blendOpAlpha;
	ret.RenderTargetWriteMask = this->renderTargetWriteMask;
	return ret;
}
BlendState::BlendState() :blendType(NULL), color(new Color((unsigned char)255, 255, 255))
{
}
BlendState::~BlendState()
{
 if(this->getBlendState()!=NULL)
 {
	 this->getBlendState()->Release();
	}
}
BlendState::BlendState(const BlendState &copy) :blendType(copy.blendType), color(new Color(*copy.color))
{
	if (this->getBlendState() != NULL)
	{
		this->getBlendState()->AddRef();
	}
}
BlendState::BlendState(const BlendStateType state, const Color &color) :blendType(state), color(new Color(color))
{
	#if CLDLEVEL >= 4
	static int number=0;
	char name[]="Blend state:   ";
	char *p=name+strlen(name)-1;
	int num=number++;
	while (num>0)
{
	 *p=(num%10)+48;
	 num/=10;
	 p--;
	}
	blendType->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
	#endif
}
BlendState *BlendState::clone() const
{
	return new BlendState(*this);
}
void BlendState::apply(const DXMain &dx) const
{
	dx.getDevice().setBlendState(this->getBlendState(),this->getColor());
}
BlendStateType BlendState::getBlendState() const
{
	return this->blendType;
}
int BlendState::getId() const
{
	return 1;
}
void BlendState::setBlendState(const BlendStateType state)
{
	this->blendType = state;
}
void BlendState::setColor(const Color &color)
{
	this->color.reset(new Color(color));
}
Color BlendState::getColor() const
{
	return *this->color;
}

BlendState & BlendState::operator=(BlendState & obj)
{
	if (this != &obj)
	{
	 if(this->getBlendState()!=NULL)
	 {
	  	this->blendType->Release();
		}
		this->setBlendState(obj.getBlendState());
		this->setColor(obj.getColor());
		if(this->getBlendState()!=NULL)
		{
  		this->getBlendState()->AddRef();
		}
	}
	return *this;
}