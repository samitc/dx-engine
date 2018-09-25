#include "DXObject.h"
#include <string>
#include "DxResException.h"
#include "BaseLog.h"
#include "Device.h"
#include "VideoAdapter.h"
DXObject::DXObject(const VideoAdapter *driverTypes, int numOfTypes, FeatureLevel *features, int numOfFeatures, bool isInDebug)
{
#ifdef DX_11
	D3D_FEATURE_LEVEL f;
	HRESULT r;
	int flags = 0;
	if (isInDebug)
	{
		flags |= D3D11_CREATE_DEVICE_DEBUG;
	}
	int i = 0;
	for (; i < numOfTypes; i++)
	{
		r = D3D11CreateDevice(driverTypes[i].getAdapter(), D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN, 0, flags, (D3D_FEATURE_LEVEL*)features, numOfFeatures, D3D11_SDK_VERSION, &this->dev, &f, &this->context);
		if (SUCCEEDED(r))
		{
			this->level = (FeatureLevel)((int)f);
			break;
		}
	}
	if (FAILED(r))
	{
		BaseLog::getInstance().fatal("can not create device");
		throw DxResException(true, __LINE__, __FILE__, "can not create device", r);
	}
	std::string logStr("create device with:level=");
	logStr.append(std::to_string(level));
	logStr.append(",drive=");
	WCHAR* des = driverTypes[i].getDescription();
	int size = wcslen(des);
	char* str = allocStack(char, size);
	wcstombs(str, des, size);
	delete[] des;
	logStr.append(str);
	BaseLog::getInstance().debug(logStr.c_str());
	#ifdef DEBUGC
	char * name = "DX device";
	this->dev->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
	name = "Context";
 this->context->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
	#endif
#else
#ifdef DX_9

#endif
#endif
}
DXObject::~DXObject()
{
#ifdef RELEASEC0
	if (this->context != 0)
	{
#endif
		this->context->Release();
#ifdef RELEASEC0
	}
#endif
#ifdef CLDEBUG
	ID3D11Debug   *m_d3dDebug;
	this->dev->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_d3dDebug));
	m_d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	m_d3dDebug->Release();
	#endif
#ifdef CLRELEASE0
	if (this->dev != 0)
	{
#endif
		ULONG num = this->dev->Release();
		#ifdef DEBUGC
		char strText[100];
		wchar_t strText1[100];
		if (num > 0)
		{
			itoa(num, strText, 10);
			mbstowcs(strText1, strText, 100);
			MessageBox(0, strText1, L"error in clean all objects", MB_OK);
		}
		#endif
#ifdef CLRELEASE0
	}
#endif
}
FormatSupport DXObject::checkFormatSupport(DataFormat format) const
{
#if defined(DX_11)
	unsigned int sup;
	dev->CheckFormatSupport((DXGI_FORMAT)(int)(format), &sup);
	return (FormatSupport)sup;
#elif defined(DX_9)
#endif
}
FeatureLevel DXObject::checkFeatureSupport()
{
#if defined(DX_11)

	HRESULT hr = E_FAIL;
	D3D_FEATURE_LEVEL MaxSupportedFeatureLevel = D3D_FEATURE_LEVEL_9_1;
	D3D_FEATURE_LEVEL FeatureLevels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
};

	hr = D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		FeatureLevels,
		ARRAYSIZE(FeatureLevels),
		D3D11_SDK_VERSION,
		NULL,
		&MaxSupportedFeatureLevel,
		NULL
		);
	return (FeatureLevel)MaxSupportedFeatureLevel;
#elif defined(DX_9)
#endif
}
DxDevice DXObject::getDevice() const
{
	return this->dev;
}
DxContext DXObject::getContext() const
{
	return this->context;
}