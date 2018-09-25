#ifndef _FILEDATA_H_
#define _FILEDATA_H_
class FileData
{
public:
	using byte = unsigned char;
	FileData() = delete;
	~FileData() = delete;
	static bool saveData(const char* fileName, const byte *data, unsigned int l);
	static byte * loadData(const char* fileName,unsigned int &size);
private:
	static constexpr int MAX_FOR_ALLOCA = 1000000;
};
#endif