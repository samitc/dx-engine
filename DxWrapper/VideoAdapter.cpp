#include "VideoAdapter.h"
#include "VideoOutput.h"
#include "DxResException.h"
VideoAdapter::VideoAdapter(VideoAdapterType adapter):adapter(adapter)
{
}
VideoAdapter::VideoAdapter(const VideoAdapter &copy):adapter(copy.adapter)
{
	this->adapter->AddRef();
}
VideoAdapter::~VideoAdapter()
{
	this->adapter->Release();
}
WCHAR* VideoAdapter::getDescription() const
{
#ifdef DX_11
	DXGI_ADAPTER_DESC des;
	this->adapter->GetDesc(&des);
	register int l = wcslen(des.Description) + 1;
	WCHAR *ret = new WCHAR[l];
	wcsncpy(ret, des.Description, l);
	return ret;
#else ifdef DX_9

#endif
}
std::vector<VideoOutput> VideoAdapter::getAllOutput() const
{
#ifdef DX_11
	UINT i = 0;
	IDXGIOutput *pOutput;
	std::vector<VideoOutput> vOutputs;
	while(this->adapter->EnumOutputs(i, (&pOutput)) != DXGI_ERROR_NOT_FOUND)
	{
		vOutputs.push_back(VideoOutput(pOutput));
		++i;
	}
	return vOutputs;
#else ifdef DX_9

#endif
}
std::vector<VideoAdapter> VideoAdapter::getAdapters()
{
	IDXGIAdapter * pAdapter;
	IDXGIFactory* pFactory;
	std::vector<VideoAdapter> adapters;
	HRESULT res = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (FAILED(res))
	{
		throw DxResException(false, __LINE__, __FILE__, "error getting adapters", res);
	}
	for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		adapters.push_back(VideoAdapter(pAdapter));
	}
	pFactory->Release();
	return adapters;
}
const VideoAdapterType VideoAdapter::getAdapter() const
{
	return adapter;
}