#include "InstanceModel.h"
#include "Device.h"
#include "DXMain.h"
#include "RenderDevice.h"
InstanceModel::InstanceModel() :instanceCount(1), buf()
{
}
void InstanceModel::setInstanceBuffer(const Buffer & instance)
{
	buf = instance;
}
void InstanceModel::prepareRendering(const RenderDevice &renderDevice)
{
#ifdef RELEASEC0
	if (this->getvBuffer() != nullptr)
	{
#endif
		unsigned int *sizeOfBuffers = (unsigned int*)_alloca(2 * sizeof(unsigned int));
		BufferType *buffers = (BufferType *)_alloca(2 * sizeof(BufferType));
		buffers[0] = getVbuffer().getBufferType();
		buffers[1] = buf.getBufferType();
		sizeOfBuffers[0] = getVbuffer().getSizeOfElement();
		sizeOfBuffers[1] = buf.getSizeOfElement();
		renderDevice.getdx().getDevice().setVertexBuffer(0, Device::BufferData(buffers, sizeOfBuffers, 2), 0);
#ifdef RELEASEC0
	}
#endif
}
void InstanceModel::draw(const RenderDevice &renderDevice, int count, int startIndex, int startVertex)
{
	if (startVertex==-1)
	{
		renderDevice.getdx().getDevice().drawInstance(count, getinstanceCount(), startIndex, 0);
	}
	else
	{
		renderDevice.getdx().getDevice().drawIndexInstance(count, getinstanceCount(), startIndex, startVertex, 0);
	}
}
GET_SET_METHOD_IMPL(instanceCount, int, InstanceModel);