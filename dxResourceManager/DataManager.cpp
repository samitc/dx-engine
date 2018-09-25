#include "DataManager.h"
#include <memory>
#include <string>
#include <algorithm>
template<class T,class Pr> bool checkIsSorted(std::vector<T> &vec,Pr &pr)
{
	auto s = vec.size() - 1;
	for (size_t i = 0; i < s; i++)
	{
		if (!pr(vec[i],vec[i+1]))
		{
			return false;
		}
	}
	return true;
}
template<class T, class Pr> int findBinary(const std::vector<T> &vec, const Pr &pr,const T &item)
{
	int min = 0;
	int max = vec.size() - 1;
	unsigned int curPos;
	while (min <= max)
	{
		curPos = (min + max) / 2;
		int prRes = pr(vec[curPos], item);
		if (prRes == 0)
		{
			return curPos;
		}
		else if (prRes < 0)
		{
			min = curPos + 1;
		}
		else
		{
			max = curPos - 1;
		}
	}
	return -1;
}
DataManager::DataManager() :nextId(0), resources()
{
	readManagerData();
}
DataManager::~DataManager()
{
	size_t size = resources.size()*sizeof(unsigned int);
	for (auto &res : resources)
	{
		size += res.name.size() + 1;
	}
	auto data = std::make_unique<byte[]>(size);
	byte *p = data.get();
	for (auto &res : resources)
	{
		strcpy((char*)p, res.name.c_str());
		p += res.name.size();
		*p = 0;
		++p;
		*((unsigned int*)(p)) = res.ID;
		p += sizeof(unsigned int);
	}
	FileData::saveData("engMag.dat", data.get(), size);
}
void DataManager::addResource(ResourceData & res) const
{
	int index = findBinary(resources, [](const auto &res, const auto &toFind)
	{
		return strcmp(res.name.c_str(), toFind.name.c_str());
	}, res);
	if (index==-1)
	{
		this->resources.push_back(res);
	}
	else
	{
		this->resources[index].ID = res.ID;
	}
}
void DataManager::readManagerData()
{
	unsigned int size;
	unsigned int maxId = 0;
	std::unique_ptr<byte> data;
	try
	{
		data.reset(FileData::loadData("engMag.dat", size));
	}
	catch (const char*)
	{
		return;
	}
	byte *end = data.get() + size;
	byte *p = data.get();
	while (p < end)
	{
		ResourceData res;
		res.name = (char*)p;
		p += res.name.size() + 1;
		res.ID = (unsigned int)*p;
		p += sizeof(unsigned int);
		if (res.ID >= maxId)
		{
			maxId = res.ID + 1;
		}
		resources.push_back(res);
	}
	nextId = maxId;
	auto &comp = [](auto &resA, auto &resB)
	{
		return strcmp(resA.name.c_str(), resB.name.c_str()) < 0;
	};
	if (!checkIsSorted(resources, comp))
	{
		std::sort(resources.begin(), resources.end(), comp);
	}
}
unsigned int DataManager::findResId(const std::vector<DataManager::ResourceData> &vec, const char* name)
{
	return (*std::find_if(vec.cbegin(), vec.cend(), [name](const auto &res)
	{
		return strcmp(name, res.name.c_str()) == 0;
	})).ID;
}
DataManager::ResourceData::ResourceData(std::string name, unsigned int id) :name(std::move(name)), ID(id)
{
}