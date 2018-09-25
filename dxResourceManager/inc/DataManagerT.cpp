#ifndef _DATAMANAGER_CPP_
#define _DATAMANAGER_CPP_
#include "DataManager.h"
#include <math.h>
#include <algorithm>
#define ADD_RESOURCE(name,outputName,idCount)\
	unsigned int ID = nextId++;n2s(outputName, ID);addResource(ResourceData(name, ID));
#define GET_RESOURCE(name,resources,outputName)\
	unsigned int ID = findResId(resources, name);n2s(outputName, ID);
inline void n2s(char* s, unsigned int n)
{
	if (n==0)
	{
		s[0] = '0';
		s[1] = 0;
	}
	else
	{
		int num = (int)log10(n);
		s[num + 1] = 0;
		while (n>0)
		{
			s[num--] = (n % 10) + '0';
			n /= 10;
		}
	}
}
template<class DataSer, class Obj>
inline Obj * DataManager::load(const DataSer& ser, const char* name) const
{
	char file[10];
	GET_RESOURCE(name, resources, file);
	int size;
	byte * data = FileData::loadData(file, size);
	Obj* ret = ser.load(data, &size);
	delete[]data;
	return ret;
}
template<class DataSer, class Obj>
void DataManager::save(const DataSer& ser, const char* name,const Obj *obj) const
{
	char file[10];
	ADD_RESOURCE(name, file, nextId);
	int size;
	byte *data = ser.save(obj, &size);
	FileData::saveData(file, data, size);
	delete[]data;
}
template<class DataSer, class Obj>
Obj * DataManager::loadStatic(const char* name) const
{
	char file[10];
	GET_RESOURCE(name, resources, file);
	unsigned int size;
	byte * data = FileData::loadData(file, size);
	Obj* ret = DataSer::load(data, size);
	delete[]data;
	return ret;
}
template<class DataSer, class Obj>
void DataManager::saveStatic(const char* name,const Obj *obj) const
{
	char file[10];
	ADD_RESOURCE(name, file, nextId);
	int size;
	byte *data = DataSer::save(obj, &size);
	FileData::saveData(file, data, size);
	delete[] data;
}
template<class DataSer, class Param, class Obj>
inline Obj * DataManager::loadParams(const DataSer& ser, const char* name, Param *param) const
{
	char file[10];
	GET_RESOURCE(name, resources, file);
	int size;
	byte * data = FileData::loadData(file, size);
	Obj* ret = ser.load(data, &size, param);
	delete[]data;
	return ret;
}
template<class DataSer, class Param, class Obj>
void DataManager::saveParams(const DataSer& ser, const char* name, const Obj *obj, Param *param) const
{
	char file[10];
	ADD_RESOURCE(name, file, nextId);
	int size;
	byte *data = ser.save(obj, &size, param);
	FileData::saveData(file, data, size);
	delete[]data;
}
template<class DataSer, class Param, class Obj>
Obj * DataManager::loadStaticParams(const char* name, Param *param) const
{
	char file[10];
	GET_RESOURCE(name, resources, file);
	unsigned int size;
	byte * data = FileData::loadData(file, size);
	Obj* ret = DataSer::load(data, size, param);
	delete[]data;
	return ret;
}
template<class DataSer, class Param, class Obj>
void DataManager::saveStaticParams(const char* name, const Obj *obj, Param *param) const
{
	char file[10];
	ADD_RESOURCE(name, file, nextId);
	int size;
	byte *data = DataSer::save(obj, &size, param);
	FileData::saveData(file, data, size);
	delete[] data;
}
#endif