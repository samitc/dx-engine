#ifndef _GPUEXCEPTION_H_
#define _GPUEXCEPTION_H_
#include "DXException.h"
//general gpu exception
class GPUException :public DXException
{
public:
	GPUException()=default;
	~GPUException() = default;
private:

};
#endif