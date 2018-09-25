// this is file that all the setting will be in

#ifndef _DXGAME_H_
#define _DXGAME_H_
#include "Engine.h"
#include <Windows.h>
#include "DxResException.h"
#define resValid(hr)	\
	HRESULT resdfdgfdgd = hr;	\
if (!SUCCEEDED(resdfdgfdgd)){		\
	throw DxResException(false,__LINE__,__FILE__,"",resdfdgfdgd);\
	}
typedef WCHAR	UTF8;
typedef unsigned int UINT;
#endif