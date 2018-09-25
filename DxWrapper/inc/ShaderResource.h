#ifndef _SHADERREDOURCES_H_
#define _SHADERREDOURCES_H_
#include "ResourceView.h"
class ShaderResourceInfo;
//this class represent resource that can be use from shader
class ShaderResource:public ResourceView
{
public://public for everyone
	ShaderResource(const ShaderResource &copy);
	~ShaderResource() = default;
	ShaderResource& operator=(const ShaderResource&);
	ShaderResourceType getResView() const;
public://public for engine
	ShaderResource(const ShaderResourceType res);
private:
};
#endif