#include "VideoOutput.h"
#include "VideoMode.h"
#include "DxResException.h"
#include "Device.h"
#include "DxEnums.h"
VideoOutput::VideoOutput(VideoOutputType adapter):adapter(adapter)
{
}
VideoOutput::VideoOutput(const VideoOutput &copy):adapter(copy.adapter)
{
	this->adapter->AddRef();
}
VideoOutput::~VideoOutput()
{
	this->adapter->Release();
}
std::vector<DataFormat> VideoOutput::getAllSupportedFormat() const
{
#ifdef DX_11
	int start = (int)(DataFormat::R32G32B32A32_TYPELESS);
	int end = (int)(DataFormat::BC7_UNORM_SRGB);
	std::vector<DataFormat> ret;
	UINT numModes = 0;
	while (start <= end)
	{
		HRESULT hr = this->adapter->GetDisplayModeList((DXGI_FORMAT)start, 0, &numModes, NULL);
		if (numModes != 0 && SUCCEEDED(hr))
		{
			ret.push_back((DataFormat)start);
		}
		start++;
	}
	return ret;
#else ifdef DX_9

#endif
}
std::vector<VideoMode> VideoOutput::getVideosMode(DataFormat format) const
{
#ifdef DX_11
	UINT numModes = 0;
	DXGI_MODE_DESC* displayModes = NULL;
	HRESULT hr = this->adapter->GetDisplayModeList((DXGI_FORMAT)((int)(format)), 0, &numModes, NULL);
	if (FAILED(hr))
	{
		throw DxResException(false, __LINE__, __FILE__, "error get video mode", hr);
	}
	displayModes = new DXGI_MODE_DESC[numModes];
	hr = this->adapter->GetDisplayModeList((DXGI_FORMAT)((int)(format)), 0, &numModes, displayModes);
	if (FAILED(hr))
	{
		delete[] displayModes;
		throw DxResException(false, __LINE__, __FILE__, "error get video mode", hr);
	}
	std::vector<VideoMode> ret;
	for (UINT i = 0; i < numModes; i++)
	{
		ret.push_back(VideoMode(displayModes[i]));
	}
	delete[] displayModes;
	return ret;
#else ifdef DX_9

#endif
}