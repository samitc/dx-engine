#ifndef _DXOBJECT_H_
#define _DXOBJECT_H_
#include "Engine.h"
#include "DXTypes.h"
enum DXDriverType;
enum FeatureLevel;
class VideoAdapter;
enum class FormatSupport
{
	BUFFER = 0x1,
	VERTEX_BUFFER = 0x2,
	INDEX_BUFFER = 0x4,
	TEXTURE1D = 0x10,
	TEXTURE2D = 0x20,
	TEXTURE3D = 0x40,
	RENDER_TARGET = 0x4000,
	DEPTH_STENCIL = 0x10000,
	MULTISAMPLE_RENDERTARGET = 0x200000,
};
class RenderDevice;
class Device;
enum class DataFormat;
//this class represent the interface with the gpu
class DXObject//public for engine
{
public://public for everyone
	~DXObject();
	FormatSupport checkFormatSupport(DataFormat format) const;
	static FeatureLevel checkFeatureSupport();
public://public for engine
	DxDevice getDevice() const;
	DxContext getContext() const;
	DXObject(const VideoAdapter *driverTypes, int numOfTypes, FeatureLevel *feature, int numOfFeatures, bool isInDebug);
protected:
private:
	DXObject& operator=(DXObject& obj) = delete;
	DxDevice dev;
	DxContext context;
	FeatureLevel level;
};
#endif