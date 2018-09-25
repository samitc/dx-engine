#include "ResourceData.h"
#include <malloc.h>
unsigned int ResourceData::getSizeForDecompressBuffer(const byte * data)
{
	int version = (int)(data[0]);
	if (version != VERSION)
	{
		throw "Not implement";
	}
	return ((int*)(data))[1];
}
int ResourceData::saveData(byte * comBuf, unsigned int comL, const byte * data, unsigned int l)
{
	if (comL<getSizeForCompressBuffer(l))
	{
		throw "Not implement";
	}
	constexpr int BOUND = 1;
	static_assert(getSizeForCompressBuffer(1) % BOUND == 0, "bound must be multiple of 1");
	unsigned int cmpByts = LZ4_compress_default((const char*)data, (char*)comBuf, l, comL);
	if (cmpByts == 0)
	{
		throw "Not implement";
	}
	if (cmpByts + SIZE_OF_VERSION + SIZE_OF_VERSION <= comL)
	{
		xorData(comBuf, cmpByts);
	}
	else
	{
		throw "Not implement";
	}
	static_assert(SIZE_OF_VERSION == sizeof(int), "SIZE_OF_VERSION must be equel to int");
	((int*)(comBuf))[0] = (int)VERSION;
	((int*)(comBuf))[1] = (int)l;
	return cmpByts + SIZE_OF_VERSION + SIZE_OF_VERSION;
}
void ResourceData::loadData(byte * deComBuf, int deComL, byte * data, int l)
{
	int lC = getSizeForDecompressBuffer(data);
	if (deComL<lC)
	{
		throw "not implement";
	}
	int version = (int)(data[0]);
	if (version != VERSION)
	{
		throw "Not implement";
	}
	byte *cmpData = data + SIZE_OF_VERSION + SIZE_OF_VERSION;
	retXorData(cmpData, l - (SIZE_OF_VERSION + SIZE_OF_VERSION));
	int cmpBytes = LZ4_decompress_safe((const char*)cmpData, (char*)deComBuf, l - SIZE_OF_VERSION - SIZE_OF_VERSION, deComL);
}
void ResourceData::xorData(byte* data, int cmpByts)
{
	using wordMac = int;
	constexpr wordMac XOR_NUM_LONG = -1;
	constexpr byte XOR_NUM_BYTE = -1;
	int curPos = cmpByts;
	while (curPos >= sizeof(byte))
	{
		curPos -= sizeof(byte);
		data[curPos + ResourceData::SIZE_OF_VERSION + ResourceData::SIZE_OF_VERSION] = data[curPos] ^ XOR_NUM_BYTE;
	}
}
void ResourceData::retXorData(byte* data, unsigned int cmpByts)
{
	constexpr byte XOR_NUM_BYTE = -1;
	for (size_t i = 0; i < cmpByts; i++)
	{
		data[i] ^= XOR_NUM_BYTE;
	}
}