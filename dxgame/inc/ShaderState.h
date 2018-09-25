#ifndef _SHADERSTATE_H_
#define _SHADERSTATE_H_
#include "State.h"
#include <memory>
#include <vector>
class Shader;
class ShaderState :public State
{
public:
	void apply(const DXMain &dx) override;
	bool hasChanged() const override;
	void setChange();
private:
};
#endif