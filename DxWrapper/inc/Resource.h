#ifndef _RESOURCE_H_
#define _RESOURCE_H_
#include "Engine.h"
#include "DXTypes.h"
class DXMain;
class Device;
enum Access;
enum ResourceFormat;
enum MapType
{
	MAP_READ = 1,
	MAP_WRITE = 2,
	READ_WRITE = 3,
	WRITE_DISCARD = 4,
	WRITE_NO_OVERWRITE = 5
};
enum CPU_ACCESS//for engine
{
	NO_ACCESS = 0,
	CPU_WRITE = 0x10000L,
	CPU_READ = 0x20000L
};
//the access rights to resource
enum Access
{
	CpuRead = 0x1,
	CpuWrite = 0x2,
	GpuRead = 0x4,
	GpuWrite = 0x8
};
//the use of the resource
enum ResourceFormat
{
	NONE = 0x0,
	VERTEX_BUFFER = 0x1,
	INDEX_BUFFER = 0x2,
	CONSTANT_BUFFER = 0x4,
	SHADER_RESOURCE = 0x8,
	STREAM_OUTPUT = 0x10L,
	RENDER_TARGET = 0x20L,
	DEPTH_STENCIL = 0x40L,
	UNORDERED_ACCESS = 0x80L
};
//this class represent gpu resource
class Resource
{
public://public for everyone
	class ResourceBuilder
	{
	protected:
		ResourceBuilder();
		ResourceBuilder(Access access, ResourceFormat format);
		ResourceBuilder(const ResourceBuilder& copy);
		UsageType getUsageType(const Device &device) const;//TODO:delete
		UINT getCpuAccess() const;//TODO:delete
		UINT getBindFlags() const;
		static CONSTEXPRN Access ACCESS_DEFAULT = (Access)(Access::GpuRead | Access::GpuWrite);
		static CONSTEXPRN ResourceFormat FORMAT_DEFAULT = ResourceFormat::NONE;
	private:
		ResourceFormat format;
		Access access;
		BUILDER_GET_SET_METHOD(format, ResourceFormat, public, ResourceBuilder);
		BUILDER_GET_SET_METHOD(access, Access, public, ResourceBuilder);
	};
	Resource(const Resource &copy);
	~Resource();
	//update the gpu resource by new data
	void updateResourceData(const DXMain &dx, const void *data, unsigned int subRes = 0, unsigned int dataRow = 0, unsigned int dataDepth = 0);
	//copy the data from other resource
	void copyFromResource(const DXMain &dx, const Resource *resource);
public://public for engine
	ResourceType getResource() const;
	Resource(ResourceType resource);
	void setResource(ResourceType resource);
protected:
	Resource();
private:
	ResourceType resource;
};
#endif