#ifndef _EObject_H_
#define _EObject_H_
#include "Engine.h"
class EObject
{
public:
	EObject();
	virtual ~EObject() = default;
protected://for engine
	bool getBit(char bit) const;
	void setBit(char bit);
	void clearBit(char bit);
private:
	unsigned char boolsValue;
};
#endif