#include "MapData.h"
#include "Resource.h"
#include "DXMain.h"
#include "Device.h"
#include "ResourceException.h"
#include "BaseLog.h"
MapData::MapData(const Resource &res, D3D11_MAPPED_SUBRESOURCE &subRes) :
	MapData(new Resource(res), subRes.pData, subRes.RowPitch, subRes.DepthPitch, 0)
{
}
Resource * MapData::getres()
{
	return this->res.get();
}
void MapData::setres(Resource * res)
{
	this->res.reset(res);
}
MapData::MapData(const Resource &res) :MapData(new Resource(res), 0, 0, 0, 0)
{
}
MapData::MapData(const MapData &copy) :MapData(new Resource(copy.getRes()), copy.getdata(), copy.getrowSize(), copy.getpageSize(), copy.getsubRes())
{
}
MapData::MapData(Resource *res, void* data, UINT rowSize, UINT pageSize, UINT subRes)
	:res(res), data(data), rowSize(rowSize), pageSize(pageSize), subRes(subRes)
{
}
MapData::~MapData()
{
	if (this->getdata() != 0)
	{
		this->applyData(*dx);
	}
}
void* MapData::getData(const DXMain &dx, MapType mapType, bool shuoldWaitForGpu, unsigned int subResource)
{
	if (this->getdata() == NULL)
	{
		if (!shuoldWaitForGpu && (mapType == MapType::WRITE_DISCARD || mapType == MapType::WRITE_NO_OVERWRITE))
		{
			shuoldWaitForGpu = true;
		}
		MapSubResourceType mapRes = dx.getDevice().createMapSubResourceType(this->getres()->getResource(), subResource, mapType, shuoldWaitForGpu);
		if (mapRes.pData != nullptr)
		{
			this->setdata(mapRes.pData);
			this->setrowSize(mapRes.RowPitch);
			this->setpageSize(mapRes.DepthPitch);
			this->setsubRes(subResource);
			this->dx = &dx;
		}
		else
		{
			this->setdata(nullptr);
		}
		return this->getdata();
	}
	else
	{
		BaseLog::getInstance().warn("data is allready maped");
		throw ResourceException("data is allready maped");
	}
}
void MapData::applyData(const DXMain &dx)
{
	if (this->getdata() != 0)
	{
		dx.getDevice().applyMapSubResource(this->getres()->getResource(), getsubRes());
		this->setdata(NULL);
	}
}
const Resource & MapData::getRes() const
{
	return *this->res;
}
GET_SET_METHOD_IMPL(rowSize, UINT, MapData)
GET_SET_METHOD_IMPL(pageSize, UINT, MapData)
SAME_GET_SET_METHOD_IMPL(data, void*, MapData)
GET_SET_METHOD_IMPL(subRes, unsigned int, MapData);