#include "DXMain.h"
#include "DxHandler.h"
#include "DXObject.h"
#include "Device.h"
#include "VideoMode.h"
#include "VideoAdapter.h"
#include "Color.h"
#include "RenderDevice.h"
#include "DxResException.h"
#include "ShortPoolMemory.h"
#include "PipeLineState.h"
#include "GpuResourceManager.h"
#include "ResourceManager.h"
DXMain::DXMain() :pipeLine(std::make_unique<PipeLineState>()), shortPool(std::make_unique<ShortPoolMemory>()), dxHandler(std::make_unique<DxHandler>(*this)), resourceManager(std::make_unique<ResourceManager>(*this)), gpuManager(std::make_unique<GpuResourceManager>(*this, *resourceManager))
{
#if RELEASEC0
	this->DX = std::unique_ptr<DXObject>(nullptr);
	this->device = std::unique_ptr<Device>(nullptr);
	this->BackgrountColor = std::unique_ptr<Color>(nullptr);
#endif
#if RELEASEC3
	this->renderDevice = std::unique_ptr<RenderDevice>(nullptr);
#endif
}
DXMain::~DXMain()
{
}
bool DXMain::initDXObject(const VideoAdapter &driverTypes, FeatureLevel features, bool isInDebug)
{
	return this->initDXObject(&driverTypes, 1, &features, 1, isInDebug);
}
bool DXMain::initDXObject(const VideoAdapter *driverTypes, int numOfTypes, FeatureLevel *features, int numOfFeatures,  bool isInDebug)
{
	this->DX.reset(new DXObject(driverTypes, numOfTypes, features, numOfFeatures, isInDebug));
	return true;
}
bool DXMain::initDevice()
{
#if RELEASEC0
	if (this->DX.get() == NULL)
	{
		return false;
	}
#endif
	this->device.reset(new Device(*this));
	return true;
}
RenderDevice DXMain::createRenderDevice(HWND hwnd, const VideoMode &videoMode)
{
	return this->createRenderDevice(hwnd, videoMode, 1, 0);
}
RenderDevice DXMain::createRenderDevice(HWND hwnd, const VideoMode &videoMode, UINT sampleCount, UINT sampleQuality)
{
#if RELEASEC0
	if (this->device.get() == NULL)
	{
		throw "not implement";
	}
#endif
	return std::move(RenderDevice(*this, hwnd, videoMode, sampleCount, sampleQuality));
}
UINT DXMain::getSampleQuality(DataFormat format, UINT sampleCount)
{
	UINT ret;
	HRESULT res = DX->getDevice()->CheckMultisampleQualityLevels((DXGI_FORMAT)((int)(format)), sampleCount, &ret);
	if (FAILED(res))
	{
		throw DxResException(false, __LINE__, __FILE__, "can not get sample quailty", res);
	}
	return ret;
}
void DXMain::applyViewport(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth) const
{
	getDevice().applyViewport(topLeftX, topLeftY, width, height, minDepth, maxDepth);
}
DXObject & DXMain::getDx() const
{
	return *DX;
}
Device& DXMain::getDevice() const
{
	return *this->device;
}
void DXMain::loadModel(const char * loc, Model* m, const ModelBuildBuilder &builder) const
{
	dxHandler->loadModel(loc, m, builder);
}
ModelData DXMain::getModelData(const char * modelLoc) const
{
	return dxHandler->getModelData(modelLoc);
}
const ResourceManager & DXMain::getResourceManager() const
{
	return *resourceManager;
}
const GpuResourceManager & DXMain::getGpuResourceManager() const
{
	return *gpuManager;
}
PipeLineState* DXMain::getPipeLineState() const
{
	return pipeLine.get();
}
ShortPoolMemory* DXMain::getShortPoolMemory() const
{
	return shortPool.get();
}