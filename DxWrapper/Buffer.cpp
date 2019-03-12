#include "Buffer.h"
#include "Engine.h"
#include "DXMain.h"
#include "Device.h"
#include "MapData.h"
#include "BufferCreateException.h"
#include "BaseLog.h"
Buffer::Buffer() :Resource()
{
}
Buffer::Buffer(const Buffer &copy) : Resource(copy), numberOfElements(copy.getNumberOfElements()), sizeOfElement(copy.getSizeOfElement())
{
}
Buffer::~Buffer()
{
}
Buffer::Buffer(BufferType buffer, int numberOfElement, int sizeOfElement) :Resource(buffer), numberOfElements(numberOfElement), sizeOfElement(sizeOfElement)
{
}
BufferType Buffer::getBufferType() const
{
	return (BufferType)this->getResource();
}
void Buffer::setNumberOfElements(int numberOfElements)
{
	this->numberOfElements = numberOfElements;
}
void Buffer::setSizeOfElement(int sizeOfElement)
{
	this->sizeOfElement = sizeOfElement;
}
int Buffer::getSizeOfElement() const
{
	return this->sizeOfElement;
}
int Buffer::getNumberOfElements() const
{
	return this->numberOfElements;
}
Buffer & Buffer::operator=(const Buffer & buf)
{
	if (this->getResource() != buf.getResource())
	{
		if (this->getResource() != nullptr)
		{
			this->getResource()->Release();
		}
		this->setResource(buf.getResource());
		this->getResource()->AddRef();
		this->setNumberOfElements(buf.getNumberOfElements());
		this->setSizeOfElement(buf.getSizeOfElement());
	}
	return *this;
}

bool Buffer::operator==(const Buffer &buf) const
{
    return getBufferType()==buf.getBufferType();
}
