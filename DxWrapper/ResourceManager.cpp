#include "ResourceManager.h"
#include "DXMain.h"
#include "BaseLog.h"
#include "Device.h"
#include "BufferCreateException.h"
UsageType getUsageType(const Device &device, Access access)
{
	return device.getUsageType(access);
}
UINT getCpuAccess(Access access)
{
	UINT cpuAccess = CPU_ACCESS::NO_ACCESS;
	if (access&Access::CpuWrite)
	{
		cpuAccess |= CPU_ACCESS::CPU_WRITE;
	}
	if (access&Access::CpuRead)
	{
		cpuAccess |= CPU_ACCESS::CPU_READ;
	}
	return cpuAccess;
}
ResourceManager::ResourceManager(const DXMain &dx) :dx(dx)
{
}
Texture2D ResourceManager::createTexture2D(const Texture2D::Texture2DBuilder &builder) const
{
	auto &device = dx.getDevice();
	D3D11_TEXTURE2D_DESC depthTexDesc;
	ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
	depthTexDesc.Width = builder.getwidth();
	depthTexDesc.Height = builder.getheight();
	depthTexDesc.MipLevels = builder.getmipLevel();
	depthTexDesc.ArraySize = builder.getarraySize();
	depthTexDesc.Format = (DXGI_FORMAT)builder.getdFormat();
	depthTexDesc.SampleDesc.Count = builder.getsampleCount();
	depthTexDesc.SampleDesc.Quality = builder.getsampleQuality();
	depthTexDesc.Usage = getUsageType(device, builder.getaccess());
	depthTexDesc.BindFlags = (UINT)builder.getformat();
	depthTexDesc.CPUAccessFlags = getCpuAccess(builder.getaccess());
	depthTexDesc.MiscFlags = builder.getmiscFlags();
	return Texture2D(device.createTexture2D(depthTexDesc));
}
Buffer ResourceManager::createBuffer(unsigned int numberOfElements, unsigned int sizeOfElement, ResourceFormat bufferFormat, Access access, const void * data) const
{
	auto &device = dx.getDevice();
	D3D11_BUFFER_DESC vertexDesc;
	ZeroMemory(&vertexDesc, sizeof(vertexDesc));
	D3D11_SUBRESOURCE_DATA *resourceData = NULL;
	UsageType u = getUsageType(device, access);
	if (u == UsageType::D3D11_USAGE_IMMUTABLE && !data)
	{
		BaseLog::getInstance().error("can not create buffer that is not accessible by cpu without initialized data");
		throw BufferCreateException("can not create buffer that is not accessible by cpu without initialized data");
	}
	if (u == UsageType::D3D11_USAGE_STAGING)
	{
		if (bufferFormat != ResourceFormat::NONE)
		{
			BaseLog::getInstance().error("can not create read only buffer with resource format other than none");
			throw BufferCreateException("can not create read only buffer with resource format other than none");
		}
	}
	vertexDesc.CPUAccessFlags = getCpuAccess(access);
	vertexDesc.Usage = u;
	vertexDesc.BindFlags = (UINT)bufferFormat;
	vertexDesc.StructureByteStride = sizeOfElement;
	vertexDesc.ByteWidth = sizeOfElement * numberOfElements;
	if (data != NULL)
	{
		resourceData = &D3D11_SUBRESOURCE_DATA();
		resourceData->pSysMem = data;
	}
	return Buffer(device.createBuffer(vertexDesc, resourceData), numberOfElements, sizeOfElement);
}