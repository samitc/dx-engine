#ifndef _SHADERPARAMS_H_
#define _SHADERPARAMS_H_
#include "Engine.h"
#include <vector>
class Buffer;
class ShaderResource;
class Sampler;
class PipeShadersParams;
class ShaderParams
{
public:
	ShaderParams() = default;
	~ShaderParams() = default;
	int addConstantBuffer(const Buffer &);
	void updateConstantBuffer(int handle, const Buffer &);
	void clearConstantBuffer();
	int addShaderResource(const ShaderResource&);
	void updateShaderResource(int handle, const ShaderResource&);
	void clearShaderResource();
	int addSampler(const Sampler&);
	void updateSampler(int handle, const Sampler&);
	void clearSampler();
private:
	std::vector<Buffer> constantBuffers;
	std::vector<ShaderResource> shaderResources;
	std::vector<Sampler> samplers;
	friend PipeShadersParams;
};
#endif