#include "Sampler.h"
#include "Device.h"
#include "DXMain.h"
Sampler::Sampler(SamplerStateType sampler) :samplerState(sampler)
{
#if CLDLEVEL >= 4
	static int number = 0;
	char name[] = "Sampler:   ";
	char *p = name + strlen(name) - 1;
	int num = number++;
	while (num > 0)
	{
		*p = (num % 10) + 48;
		num /= 10;
		p--;
	}
	sampler->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
#endif
}
Sampler::SamplerBuilder::SamplerBuilder():SamplerBuilder(TextureAddressMode::WRAP, TextureAddressMode::WRAP, TextureAddressMode::WRAP, Filter::MIN_MAG_MIP_LINEAR, Comparison::NEVER)
{
}
Sampler::SamplerBuilder::SamplerBuilder(TextureAddressMode u, TextureAddressMode v, TextureAddressMode w, Filter filter, Comparison comparison) :
	u(u), v(v), w(w), filter(filter), comparison(comparison)
{
}
Sampler::Sampler(const Sampler &copy):samplerState(copy.samplerState)
{
	this->getsamplerState()->AddRef();
}
Sampler::~Sampler()
{
	if (this->getsamplerState() != 0)
	{
		this->getsamplerState()->Release();
	}
}
Sampler & Sampler::operator=(const Sampler & obj)
{
	if (this != &obj)
	{
		this->getsamplerState()->Release();
		this->setsamplerState(obj.getsamplerState());
		this->getsamplerState()->AddRef();
	}
	return *this;
}
Sampler Sampler::SamplerBuilder::build(const DXMain & dx) const
{
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	samplerDesc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)(int) this->getu();
	samplerDesc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)(int) this->getv();
	samplerDesc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)(int) this->getw();
	samplerDesc.ComparisonFunc = (D3D11_COMPARISON_FUNC)(int)this->getcomparison();
	samplerDesc.Filter = (D3D11_FILTER)(int)this->getfilter();
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	return Sampler(dx.getDevice().createSamplerState(samplerDesc));
}
BUILDER_GET_SET_METHOD_IMPL(u, TextureAddressMode, Sampler::SamplerBuilder);
BUILDER_GET_SET_METHOD_IMPL(v, TextureAddressMode, Sampler::SamplerBuilder);
BUILDER_GET_SET_METHOD_IMPL(w, TextureAddressMode, Sampler::SamplerBuilder);
BUILDER_GET_SET_METHOD_IMPL(filter, Filter, Sampler::SamplerBuilder);
BUILDER_GET_SET_METHOD_IMPL(comparison, Comparison, Sampler::SamplerBuilder);
GET_SET_METHOD_IMPL(samplerState, SamplerStateType, Sampler);