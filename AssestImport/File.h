#ifndef _FILE_H_
#define _FILE_H_
#include <memory>
#include <fstream>
#include "Engine.h"
typedef bool(*tokenFunc)(void *data, const char* text, void **nextFunc);
typedef void(*endFunc)(void *data, bool forceStop);
class File
{
public:
	File(const File &copy);
	static File create(const char* loc);
	~File();
	bool isOpen();
	File& operator=(const File& file);
public://for engine
	void proccessTokens(void *data, tokenFunc func, endFunc end);
private:
	File(std::ifstream *file, const char* loc);
	File();
	std::unique_ptr<std::ifstream> file;
	char* loc;
	GET_METHOD(loc, char*, public);
};
#endif