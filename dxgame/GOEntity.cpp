#include "GOEntity.h"
#include "Buffer.h"
#include "DXMain.h"
#include "Device.h"
GOEntity::GOEntity() :Ibuffer(nullptr), Vbuffer(nullptr)
{
}
GOEntity::GOEntity(const Buffer & vertexBuffer, const Buffer & indexBuffer) : Vbuffer(new Buffer(vertexBuffer)), Ibuffer(setIndexBufferCtor(indexBuffer))
{
}
GOEntity::~GOEntity()
{
}
void GOEntity::render(const RenderDevice& renderDevice)
{
	this->prepareRendering(renderDevice);
	if (this->Ibuffer == NULL)
	{
		draw(renderDevice, Vbuffer->getNumberOfElements(), 0, -1);
	}
	else
	{
		renderDevice.getdx().getDevice().setIndexBuffer(this->Ibuffer->getBufferType(), 0, Ibuffer->getSizeOfElement());
		draw(renderDevice, this->Ibuffer->getNumberOfElements(), 0, 0);
	}
}
unsigned int GOEntity::getBuffers(BufferType ** buffers, unsigned int ** size) const
{
	static BufferType bt;
	static unsigned int val;
	bt = this->Vbuffer->getBufferType();
	*buffers = (BufferType*)(&bt);
	val = this->Vbuffer->getSizeOfElement();
	*size = (unsigned int*)(&val);
	return 1;
}
GOEntity & GOEntity::operator=(GOEntity & obj)
{
	if (this != &obj)
	{
		this->Vbuffer.reset(new Buffer(*obj.Vbuffer));
		this->Ibuffer.reset(new Buffer(*obj.Ibuffer));
	}
	return *this;
}
const Buffer * const GOEntity::getiBuffer() const
{
	return this->Ibuffer.get();
}
const Buffer * const GOEntity::getvBuffer() const
{
	return this->Vbuffer.get();
}
void GOEntity::setVbuffer(Buffer * buf)
{
	this->Vbuffer.reset(buf);
}
void GOEntity::setIbuffer(Buffer * buf)
{
	this->Ibuffer.reset(buf);
}
const Buffer & GOEntity::getVbuffer() const
{
	return *this->Vbuffer.get();
}
const Buffer & GOEntity::getIbuffer() const
{
	return *this->Ibuffer.get();
}
Buffer * GOEntity::setIndexBufferCtor(const Buffer & indexBuffer)
{
	if (&indexBuffer == nullptr)
	{
		return nullptr;
	}
	return new Buffer(indexBuffer);
}
void GOEntity::prepareRendering(const RenderDevice &renderDevice)
{
#ifdef RELEASEC0
	if (this->Vbuffer != NULL)
	{
#endif
		unsigned int *sizeOfBuffers;
		BufferType *buffers;
		renderDevice.getdx().getDevice().setVertexBuffer(0, Device::BufferData(buffers, sizeOfBuffers, this->getBuffers(&buffers, &sizeOfBuffers)), 0);
#ifdef RELEASEC0
	}
#endif
}
void GOEntity::draw(const RenderDevice &renderDevice, int count, int startIndex, int startVertex)
{
	if (startVertex==-1)
	{
		renderDevice.getdx().getDevice().drawBuffer(count, startIndex);
	}
	else
	{
		renderDevice.getdx().getDevice().drawIndexBuffer(count, startIndex, startVertex);
	}
}