#include "ShaderState.h"
void ShaderState::apply(const DXMain & dx)
{
	this->clearChange();
}
bool ShaderState::hasChanged() const
{
	return this->getChange(0);
}
void ShaderState::setChange()
{
	State::setChange(0);
}