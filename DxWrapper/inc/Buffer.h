#ifndef _BUFFER_H_
#define _BUFFER_H_
#include "Resource.h"
#include "Engine.h"
class DXMain;
class MapData;
//this class represent buffer of the hardware
class Buffer :public Resource
{
public://public for everyone
	Buffer();
	Buffer(const Buffer &copy);
	~Buffer();
	int getSizeOfElement() const;
	int getNumberOfElements() const;
	Buffer& operator=(const Buffer& buf);
    bool operator==(const Buffer&) const;
public://public for engine
	BufferType getBufferType() const;
	Buffer(BufferType buffer, int numberOfElement, int sizeOfElement);
private:
	void setNumberOfElements(int numberOfElements);
	void setSizeOfElement(int sizeOfElement);
	int numberOfElements;
	int sizeOfElement;
};
#endif