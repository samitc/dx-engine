#ifndef _BUFFERSIZENOTMATCH_H_
#define _BUFFERSIZENOTMATCH_H_
#include "BufferException.h"
//exception when try to create combine two buffers and their size does not match 
class BufferSizeNotMatch :public BufferException
{
public:
	BufferSizeNotMatch() = delete;
	BufferSizeNotMatch(int sizeBufA, int sizeBufB);
	~BufferSizeNotMatch() = default;
private:
	int sizeA;
	int sizeB;
};
#endif