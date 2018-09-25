#ifndef _BUFFEREXCEPTION_H_
#define _BUFFEREXCEPTION_H_
#include "GPUException.h"
//general exception for buffer
class BufferException :public GPUException
{
public:
	BufferException() = default;
	~BufferException() = default;
private:
};
#endif