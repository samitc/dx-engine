#include "EObject.h"
EObject::EObject()
{
	boolsValue = 0xff;
}
bool EObject::getBit(char bit) const
{
	return (boolsValue&(1 << bit)) != 0;
}
void EObject::setBit(char bit)
{
	boolsValue |= 1 << bit;
}
void EObject::clearBit(char bit)
{
	boolsValue &= !(1 << bit);
}