#include "Resource.h"
#include "DXMain.h"
#include "Device.h"
Resource::Resource(const Resource &copy):resource(copy.resource)
{
	this->getResource()->AddRef();
}
Resource::Resource(ResourceType resource):resource(resource)
{
 	#ifdef DEBUGC
	static int number=0;
	char name[]="Resource:   ";
	char *p=name+strlen(name)-1;
	int num=number++;
	while (num>0)
	{
	 *p=(num%10)+48;
	 num/=10;
	 p--;
	}
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
	#endif
}
Resource::~Resource()
{
#ifdef RELEASEC3
	if (this->getResource() != nullptr)
	{
#endif
		this->getResource()->Release();
#ifdef RELEASEC3
	}
#endif
}
void Resource::updateResourceData(const DXMain &dx, const void *data, unsigned int subRes, unsigned int dataRow, unsigned int dataDepth)
{
#ifdef RELEASEC3
	if (this->getResource() != NULL)
	{
#endif
		dx.getDevice().updateSubresource(this->getResource(), data, subRes, dataRow, dataDepth);
#ifdef RELEASEC3
	}
#endif
}
void Resource::copyFromResource(const DXMain &dx, const Resource * resource)
{
	dx.getDevice().copyResource(this->getResource(), resource->getResource());
}
Resource::Resource() :resource(NULL)
{

}
ResourceType Resource::getResource() const
{
	return this->resource;
}
void Resource::setResource(ResourceType resource)
{
	this->resource = resource;
}
BUILDER_GET_SET_METHOD_IMPL(format, ResourceFormat, Resource::ResourceBuilder);
BUILDER_GET_SET_METHOD_IMPL(access, Access, Resource::ResourceBuilder);
Resource::ResourceBuilder::ResourceBuilder() :access((Access)(Access::GpuRead | Access::GpuWrite)), format(ResourceFormat::NONE)
{
}
Resource::ResourceBuilder::ResourceBuilder(Access access, ResourceFormat format):access(access),format(format)
{
}
Resource::ResourceBuilder::ResourceBuilder(const ResourceBuilder & copy) :format(copy.format), access(copy.access)
{
}
UsageType Resource::ResourceBuilder::getUsageType(const Device & device) const
{
	return device.getUsageType(access);
}
UINT Resource::ResourceBuilder::getCpuAccess() const
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
UINT Resource::ResourceBuilder::getBindFlags() const
{
	return (UINT)this->getformat();
}