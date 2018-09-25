#ifndef _GENERALEXCEPTION_H_
#define _GENERALEXCEPTION_H_
#include "DXException.h"
#include <string>
//general engine exception
class GeneralException :public DXException
{
public:
	GeneralException() = default;
	GeneralException(const std::string &errorMessage);
	~GeneralException() = default;
private:
	std::string error;
};
#endif