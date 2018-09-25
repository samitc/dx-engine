#ifndef _DXRESEXCEPTION_H_
#define _DXRESEXCEPTION_H_
#include <Windows.h>
#include "DXException.h"
//engine general resource exception
class DxResException:public DXException
{
public:
	DxResException() = delete;
	DxResException(bool isFatal, int line, const char* file, const char* error,HRESULT res);
	~DxResException() = default;
private:
	int line;
	const char *file;
	const char *error;
	HRESULT res;
};
#endif