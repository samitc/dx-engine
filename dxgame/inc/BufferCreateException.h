#ifndef _BUFFERCREATEEXCEPTION_H_
#define _BUFFERCREATEEXCEPTION_H_
#include "BufferException.h"
#include <string>
//exception that created when try to create buffer
class BufferCreateException :public BufferException
{
public:
	BufferCreateException() = default;
	BufferCreateException(const std::string &reason);
	~BufferCreateException() = default;
private:
	std::string reason;
};
#endif