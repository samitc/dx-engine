#include "PipeShadersParams.h"
#include "Buffer.h"
#include "ShaderResource.h"
#include "Sampler.h"
#include "Device.h"
#include "DXObject.h"
#define createParamsImpl(varName) SAMEC_GET_SET_METHOD_IMPL(varName,ShaderParams&,PipeShadersParams);
createParamsImpl(vertexParams);
createParamsImpl(pixelParams);
template <class T, class U> void apply(const Device &device, int count, U* data)
{
	throw "Not Implement";
//#error all should be write manualy
}
#define applyTemplateImpl(type,shortName)\
template <> void apply<type,BufferType>(const Device &device, int count,BufferType *data){device.getDX()->getContext()->shortName##SetConstantBuffers(0,count,data);}\
template <> void apply<type, SamplerStateType>(const Device &device, int count, SamplerStateType *data) { device.getDX()->getContext()->shortName##SetSamplers(0, count, data); }\
template <> void apply<type, ShaderResourceType>(const Device &device, int count, ShaderResourceType *data) { device.getDX()->getContext()->shortName##SetShaderResources(0, count, data); }
applyTemplateImpl(VertexShader, VS);
applyTemplateImpl(PixelShader, PS);
template <class T> void applyShaderParam(const std::vector<Buffer>& constantBuffers, const std::vector<ShaderResource>& shaderResources,const std::vector<Sampler>&samplers, const Device& device)
{
	if (constantBuffers.size()>0)
	{
		int index = 0;
		BufferType *bufferType = allocStack(BufferType, constantBuffers.size());
		auto &s = constantBuffers.cbegin(), &e = constantBuffers.cend();
		while (s != e)
		{
			bufferType[index] = (*s).getBufferType();
			index++;
			++s;
		}
		apply<T, BufferType>(device, index, bufferType);
	}
	if (samplers.size()>0)
	{
		int index = 0;
		SamplerStateType *samplerType= allocStack(SamplerStateType, samplers.size());
		auto &s = samplers.cbegin(), &e = samplers.cend();
		while (s != e)
		{
			samplerType[index] = (*s).getsamplerState();
			index++;
			++s;
		}
		apply<T, SamplerStateType>(device, index, samplerType);
	}
	if (shaderResources.size()>0)
	{
		int index = 0;
		ShaderResourceType *samplerType = allocStack(ShaderResourceType, shaderResources.size());
		auto &s = shaderResources.cbegin(), &e = shaderResources.cend();
		while (s != e)
		{
			samplerType[index] = (*s).getResView();
			index++;
			++s;
		}
		apply<T, ShaderResourceType>(device, index, samplerType);
	}
}
#define callApply(className,varName) applyShaderParam<className>(varName.constantBuffers,varName.shaderResources,varName.samplers, device);
void PipeShadersParams::apply(const Device &device) const
{
	callApply(VertexShader, vertexParams);
	callApply(PixelShader, pixelParams);
}