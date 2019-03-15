#include "ShaderParams.h"
#include "Buffer.h"
#include "ShaderResource.h"
#include "Sampler.h"
template <class T> int addData(std::vector<T> &vec, const T& data)
{
	int ps = vec.size();
	vec.push_back(data);
	return ps;
}
int ShaderParams::addConstantBuffer(const Buffer &buf)
{
	return addData(constantBuffers, buf);
}

void ShaderParams::updateConstantBuffer(int handle, const Buffer &buf)
{
	constantBuffers[handle] = buf;
}

void ShaderParams::updateConstantBuffer(int handle, const DXMain& main, const void *data)
{
    constantBuffers[handle].updateResourceData(main, data);
}

void ShaderParams::clearConstantBuffer()
{
	constantBuffers.clear();
}

int ShaderParams::addShaderResource(const ShaderResource &shaderResource)
{
	return addData(shaderResources, shaderResource);
}

void ShaderParams::updateShaderResource(int handle, const ShaderResource &shaderResource)
{
	shaderResources[handle] = shaderResource;
}

void ShaderParams::clearShaderResource()
{
	shaderResources.clear();
}

int ShaderParams::addSampler(const Sampler &sampler)
{
	return addData(samplers, sampler);
}

void ShaderParams::updateSampler(int handle, const Sampler &sampler)
{
	samplers[handle] = sampler;
}

void ShaderParams::clearSampler()
{
	samplers.clear();
}
