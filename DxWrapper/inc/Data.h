#ifndef _DATA_H_
#define _DATA_H_
#include "Engine.h"
#include "DXTypes.h"
//class that represent some general data(memory location and size)
class Data//private class for the engine
{
public://public for everyone
public://public for engine
	Data(Blob* data);
	Data(const Data &data);
	~Data();
	LPVOID getPointer() const;
	SIZE_T getSize() const;
	Data& operator=(const Data& obj);
private:
	Blob* data;
};
#endif