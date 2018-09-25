#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_
#include "Engine.h"
#include <vector>
#include "FileData.h"
using byte = unsigned char;
class DataManager
{
public:
	DataManager();
	~DataManager();
	template <class DataSer, class Obj=DataSer> Obj* load(const DataSer& ser, const char* name) const;
	template <class DataSer, class Obj=DataSer> void save(const DataSer& ser, const char* name, const Obj*) const;
	template <class DataSer, class Obj=DataSer> Obj* loadStatic(const char* name) const;
	template <class DataSer, class Obj=DataSer> void saveStatic(const char* name, const Obj*) const;
	template <class DataSer, class Param = void, class Obj = DataSer> Obj* loadParams(const DataSer& ser, const char* name, Param *param) const;
	template <class DataSer, class Param = void, class Obj = DataSer> void saveParams(const DataSer& ser, const char* name, const Obj*, Param *param) const;
	template <class DataSer, class Param = void, class Obj = DataSer> Obj* loadStaticParams(const char* name, Param *param) const;
	template <class DataSer, class Param = void, class Obj = DataSer> void saveStaticParams(const char* name, const Obj*, Param *param) const;
private:
	struct ResourceData
	{
		ResourceData() = default;
		ResourceData(std::string name, unsigned int id);
		std::string name;
		unsigned int ID;
	};
	void addResource(ResourceData &res) const;
	void readManagerData();
	static unsigned int findResId(const std::vector<ResourceData> &vec, const char* name);
	mutable unsigned int nextId;
	mutable std::vector<ResourceData> resources;
};
#include "DataManagerT.cpp"
#endif