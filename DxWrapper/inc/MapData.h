#ifndef _MAPDATA_H_
#define _MAPDATA_H_
#include <memory>
#include "Engine.h"
class Resource;
class DXMain;
struct D3D11_MAPPED_SUBRESOURCE;
enum MapType;
class MapData
{
public://public for everyone
	//create map data from resource
	MapData(const Resource &res);
	MapData(const MapData &copy);
	~MapData();
	//get pointer to the data of the resource
	//parameters:
	//	dx:
	//	mapType:how to map the data of the gpu resource 
	//	shuoldWaitForGpu:wait for gpu to finish
	//	subResource:subresource of the resource(if any)
	void* getData(const DXMain &dx, MapType mapType, bool shuoldWaitForGpu = true, unsigned int subResource = 0);
	//finish with the map data
	void applyData(const DXMain &dx);
	const Resource& getRes()const;
public://public for engine
	MapData(const Resource &res, D3D11_MAPPED_SUBRESOURCE &subRes);
private:
	MapData(Resource *res, void* data, unsigned int rowSize, unsigned int pageSize, unsigned int subRes);
	Resource* getres();
	void setres(Resource *res);
	MapData();
	MapData& operator=(MapData& obj) = delete;
	std::unique_ptr<Resource> res;
	void* data;
	unsigned int rowSize;
	unsigned int pageSize;
	unsigned int subRes;
	const DXMain* dx;
	GET_METHOD(rowSize, unsigned int, public);
	SET_METHOD(rowSize, unsigned int, private);
	GET_METHOD(pageSize, unsigned int, public);
	SET_METHOD(pageSize, unsigned int, private);
	SAME_GET_METHOD(data, void*, public);
	SAME_SET_METHOD(data, void*, private);
	GET_SET_METHOD(subRes, unsigned int, private);
};
#endif