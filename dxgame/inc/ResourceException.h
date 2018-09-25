#ifndef _RESOURCEEXCEPTION_H_
#define _RESOURCEEXCEPTION_H_
#include "DXException.h"
#include <string>
//exception in gpu resource
class ResourceException :public DXException
{
public:
	ResourceException() = default;
	ResourceException(const std::string &errorMessage);
	~ResourceException() = default;
private:
	std::string error;
};
#endif