#ifndef _RENDERDEVICE_H_
#define _RENDERDEVICE_H_
#include "Engine.h"
#include <memory>
#include "DXTypes.h"
#include "Color.h"
class DXObject;
class VideoMode;
class DXMain;
class RenderResource;
class PipeLineRenderTarget;
class PipeLineDepthStencil;
class Color;
enum class DataFormat;
typedef unsigned char ColorNum;
class RenderDevice//public for engine
{
public://public for everyone
	//constructor for render device
	//parameters:
	//	HWND:the handle to the window of the game
	//	videoMode:the video mode to create the window data with
	//	sampleCount:
	//	sampleQuality:
	RenderDevice(DXMain &dx, HWND hwnd, const VideoMode videoMode, UINT sampleCount = 1, UINT sampleQuality = 0);
	RenderDevice(RenderDevice&&);
	~RenderDevice();
	//this not changing the size itself but should be called after such change
	void resizeWindow(UINT width, UINT height, const VideoMode *videoMode);
	//clear the screen buffer
	void clearBackBuffer() const;
	//clear the DepthStencil buffer
	void clearDepthStencil() const;
	//set if press on alt+enter change the window to full screen
	void setAllowAltEnterFullScreen(bool allow);
	bool getFullScreenMode() const;
	//change the full screen mode
	void setFullScreenMode(bool isFullScreen);
	//change the target to render to texture
	void renderToTexture(const PipeLineRenderTarget *renderTarget, const PipeLineDepthStencil *depthStencil);
	//change the target to render the window
	void renderToBackBuffer();
	//set the window background color
	void setBackgroundColor(ColorNum R, ColorNum G, ColorNum B);
	//this method must be called before any draw statement(each frame)
	void beginRender() const;
	//this method must be called after any draw statement(each frame)
	void endRender() const;
	//set if each frame the screen will be clear by the background color
	void setShouldClearBuf(bool clear);
	bool getShouldClearBuf() const;
	//set if each frame the DepthStencil buffer will be clear
	void setShouldClearDepthStencil(bool clear);
	bool getShouldClearDepthStencil() const;
public://public for engine
	void present() const;
	void ClearColor(const Color &col) const;
	void ClearDepthStencil(bool needClearDepth, float clearDepth, bool needClearStencil, UINT8 clearStencil) const;
protected:
	void init(UINT width, UINT height);
	void setVideoMode(const VideoMode &mode);
	VideoMode getVideoMode() const;
	void setSampleCount(UINT sample);
	void setSampleQuality(UINT sample);
	UINT getSampleCount()const;
	UINT getSampleQuality() const;
private:
	void applyRenderTargets() const;
	RenderDevice& operator=(RenderDevice& obj) = delete;
	std::unique_ptr<VideoMode> video;
	const DXMain& dx;
	HWND hwnd;
	std::unique_ptr<PipeLineRenderTarget> renderTarget;
	std::unique_ptr<PipeLineDepthStencil> depthStencil;
	const PipeLineRenderTarget* texRenderTarget;
	const PipeLineDepthStencil* texDepthStencil;
	RenderDeviceType renderDevice;
	UINT sampleCount;
	UINT sampleQuality;
	Color backgroundColor;
	//bit 1 - clear back buffer bit 2 - Clear Depth Stencil
	unsigned char boolsvalue;
	GET_SET_METHOD(hwnd, HWND, private);
	GET_SET_METHOD(renderDevice, RenderDeviceType, private);
	GET_METHOD(dx, const DXMain&, public);
};
#endif