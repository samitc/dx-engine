#ifndef _RESOURCEDATA_H_
#define _RESOURCEDATA_H_
#include <lz4.h>
using byte = unsigned char;
class ResourceData
{
public:
	ResourceData() = delete;
	~ResourceData() = delete;
	constexpr static unsigned int getSizeForCompressBuffer(unsigned int l)
	{
		return LZ4_COMPRESSBOUND(l);
	}
	static unsigned int getSizeForDecompressBuffer(const byte *data);
	static int saveData(byte * comBuf, unsigned int comL, const byte *data, unsigned int l);
	static void loadData(byte * deComBuf, int deComL, byte *data, int l);
private:
	static void xorData(byte* data, int cmpByts);
	static void retXorData(byte* data, unsigned int cmpByts);
	static constexpr unsigned int VERSION = 1;
	static constexpr unsigned int SIZE_OF_VERSION = 4;
};
#endif