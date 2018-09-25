#ifndef _DXMAIN_H_
#define _DXMAIN_H_
#include "Engine.h"
#include <vector>
#include <memory>
#include <Windows.h>
#include "DxEnums.h"
#include "RenderDevice.h"
class VideoAdapter;
class DXObject;
class Device;
class RenderDevice;
class VideoMode;
class Color;
class PipeLineRenderTarget;
class PipeLineDepthStencil;
class Resource;
class ShaderResource;
class ShaderResourceInfo;
class TextureInfo;
class UnorderedView;
class UnorderedViewInfo;
class Model;
class ModelBuildBuilder;
class ModelData;
class PipeLineState;
class ShortPoolMemory;
class DxHandler;
class ResourceManager;
class GpuResourceManager;
enum DXDriverType;
enum FeatureLevel;
//this class represent the main data of the engine that communicate with the gpu
class DXMain
{
public:
	DXMain();
	~DXMain();
	//initialized the gpu interface
	//parameters:
	//	driverTypes:how to use the hardware
	//	FeatureLevel:the feature the needed to the game
	//	isInDebug:create the device in debug mode
	bool initDXObject(const VideoAdapter &driverTypes, FeatureLevel features, bool isInDebug);
	//initialized the gpu interface
	//parameters:
	//	driverTypes:how to use the hardware
	//	numOfTypes:the number of elements in driverTypes 
	//	FeatureLevel:the feature the needed to the game
	//	numOfFeatures:the number of elements in FeatureLevel
	//	isInDebug:create the device in debug mode
	bool initDXObject(const VideoAdapter *driverTypes, int numOfTypes, FeatureLevel *features, int numOfFeatures, bool isInDebug);
	//initialized the device of the engine
	bool initDevice();
	//initialized the engien windows data
	//parameters:
	//	HWND:the handle to the window of the game
	//	videoMode:the video mode to create the window data with
	//	sampleCount:
	//	sampleQuality:
	RenderDevice createRenderDevice(HWND hwnd, const VideoMode &videoMode, UINT sampleCount, UINT sampleQuality);
	//initialized the engien windows data
	//parameters:
	//	HWND:the handle to the window of the game
	//	videoMode:the video mode to create the window data with
	RenderDevice createRenderDevice(HWND hwnd, const VideoMode &videoMode);
	//change the size of the gpu window buffers
	//get the maximum available sample quality for specific format and sample count
	UINT getSampleQuality(DataFormat format, UINT sampleCount);
	//send the view port to the gpu
	void applyViewport(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0, float maxDepth = 1.0) const;
	void loadModel(const char* loc, Model *m, const ModelBuildBuilder &builder) const;
	ModelData getModelData(const char* modelLoc) const;
	const ResourceManager& getResourceManager() const;
	const GpuResourceManager& getGpuResourceManager() const;
public://public for engine
	DXObject& getDx() const;
	Device &getDevice() const;
	PipeLineState* getPipeLineState() const;
	ShortPoolMemory* getShortPoolMemory() const;
private:
	std::unique_ptr<DXObject> DX;
	std::unique_ptr<Device> device;
	std::unique_ptr<PipeLineState> pipeLine;
	std::unique_ptr<ShortPoolMemory> shortPool;
	std::unique_ptr<DxHandler> dxHandler;
	std::unique_ptr<ResourceManager> resourceManager;
	std::unique_ptr<GpuResourceManager> gpuManager;
};
#endif