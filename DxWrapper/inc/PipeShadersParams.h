#ifndef _PIPESHADERPARAMS_H_
#define _PIPESHADERPARAMS_H_
#include "Engine.h"
#include <vector>
#include "ShaderParams.h"
class Device;
#define createParams(varName)	ShaderParams varName;\
SAMEC_GET_SET_METHOD(varName,ShaderParams&,public);\
private:
class PipeShadersParams
{
public:
	PipeShadersParams()=default;
	~PipeShadersParams()=default;
	void apply(const Device&) const;
private:
	createParams(vertexParams);
	createParams(pixelParams);
};
#endif