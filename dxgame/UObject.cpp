#include "UObject.h"
#define UPDATE_BIT	1
bool UObject::getShouldUpdate() const
{
	return getBit(UPDATE_BIT);
}
void UObject::setShouldUpdate(bool update)
{
	if (update)
		setBit(UPDATE_BIT);
	else
		clearBit(UPDATE_BIT);
}
bool UObject::beforeUpdate(const GameTime & time)
{
	return true;
}
bool UObject::afterUpdate(const GameTime & time)
{
	return true;
}