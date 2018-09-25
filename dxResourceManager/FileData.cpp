#include "FileData.h"
#include <fstream>
#include "ResourceData.h"
bool FileData::saveData(const char * fileName, const byte * data, unsigned int l)
{
	int bufL = ResourceData::getSizeForCompressBuffer(l);
	byte* buf;
	if (bufL<FileData::MAX_FOR_ALLOCA)
	{
		buf = (byte*)_alloca(bufL*sizeof(byte));
	}
	else
	{
		buf = new byte[bufL];
	}
	int numOfBytes = ResourceData::saveData(buf, bufL, data, l);
	std::ofstream file{ fileName ,std::ios_base::binary };
	file.write((const char*)buf, numOfBytes);
	file.close();
	if (!(bufL<FileData::MAX_FOR_ALLOCA))
	{
		delete[] buf;
	}
	return true;
}
byte * FileData::loadData(const char * fileName,unsigned int &size)
{
	std::ifstream file{ fileName ,std::ios_base::binary};
	if (!file.is_open())
	{
		throw "not implement";
	}
	file.seekg(0, file.end);
	unsigned int length = (unsigned int)file.tellg();
	file.seekg(0, file.beg);
	byte* buf;
	if (length < FileData::MAX_FOR_ALLOCA)
	{
		buf = (byte*)_alloca(length*sizeof(byte));
	}
	else
	{
		buf = new byte[length];
	}
	file.read((char*)buf, length);
	file.close();
	size = ResourceData::getSizeForDecompressBuffer(buf);
	byte* retData = new byte[size];
	ResourceData::loadData(retData, size, buf, length);
	return retData;
}