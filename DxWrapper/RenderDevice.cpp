#include "RenderDevice.h"
#include <DXGI.h>
#include "DXObject.h"
#include "VideoMode.h"
#include "DXMain.h"
#include "Device.h"
#include "Texture2D.h"
#include "PipeLineRenderTarget.h"
#include "PipeLineDepthStencil.h"
#include "Color.h"
#include "ShortPoolMemory.h"
#include "DxResException.h"
#include "BaseLog.h"
#include "GpuResourceManager.h"
RenderDevice::RenderDevice(DXMain &dx, HWND hwnd, const VideoMode videoMode, UINT sampleCount, UINT sampleQuality) :hwnd(hwnd), dx(dx), video(new VideoMode(videoMode)), sampleCount(sampleCount), sampleQuality(sampleQuality), texRenderTarget(nullptr), texDepthStencil(nullptr), backgroundColor((unsigned char)0, (unsigned char)0, (unsigned char)0), boolsvalue(3)
{
	auto &device = dx.getDevice();
#ifdef DX_11
	LONG w, h;
	if (videoMode.getWidth() != -1)
	{
		w = videoMode.getWidth();
		h = videoMode.getHeight();
	}
	else
	{
		RECT clientSize;
		GetClientRect(hwnd, &clientSize);
		w = clientSize.right - clientSize.left;
		h = clientSize.bottom - clientSize.top;
	}
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = w;
	swapChainDesc.BufferDesc.Height = h;
	swapChainDesc.BufferDesc.Format = (DXGI_FORMAT)((int)(videoMode.getFormat()));
	swapChainDesc.BufferDesc.RefreshRate.Numerator = videoMode.getNumerator();
	swapChainDesc.BufferDesc.RefreshRate.Denominator = videoMode.getDenominator();
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;
	swapChainDesc.SampleDesc.Count = sampleCount;
	swapChainDesc.SampleDesc.Quality = sampleQuality;
	IDXGIDevice* dxgiDevice;
	device.getDX()->getDevice()->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	IDXGIAdapter* adapter;
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&adapter);
	IDXGIFactory* factory;
	adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory);
	HRESULT res = factory->CreateSwapChain(device.getDX()->getDevice(), &swapChainDesc, &renderDevice);
	factory->Release();
	adapter->Release();
	dxgiDevice->Release();
	if (FAILED(res))
	{
		throw DxResException(true, __LINE__, __FILE__, "can not create swap chain", res);
	}
	BaseLog::getInstance().debug("created swap chain");
	this->init(w, h);
#if CLDLEVEL >= 4
	static int number = 0;
	char name[] = "Swap chain:   ";
	char *p = name + strlen(name) - 1;
	int num = number++;
	while (num > 0)
	{
		*p = (num % 10) + 48;
		num /= 10;
	 p--;
	}
	renderDevice->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
#endif
#else ifdef DX_9

#endif
}
RenderDevice::RenderDevice(RenderDevice &&obj) :hwnd(obj.hwnd), dx(obj.dx), video(std::move(obj.video)), sampleCount(obj.sampleCount), sampleQuality(obj.sampleQuality), texRenderTarget(obj.texRenderTarget), texDepthStencil(obj.texDepthStencil), backgroundColor(obj.backgroundColor), boolsvalue(obj.boolsvalue), renderTarget(std::move(obj.renderTarget)), depthStencil(std::move(obj.depthStencil)), renderDevice(obj.renderDevice)
{
	obj.renderDevice = nullptr;
}
RenderDevice::~RenderDevice()
{
	if (this->renderDevice != NULL)
	{
		if (this->getFullScreenMode())
		{
			this->setFullScreenMode(false);
		}
		this->renderDevice->Release();
	}
}
void RenderDevice::init(UINT width, UINT height)
{
	ID3D11Texture2D* backBufferTexture;
	HRESULT res = this->renderDevice->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferTexture);
	if (FAILED(res))
	{
		throw DxResException(true, __LINE__, __FILE__, "can not get the render buffers", res);
	}
	renderTarget = std::make_unique<PipeLineRenderTarget>(dx, backBufferTexture);
	PipeLineDepthStencil::PipeLineDepthStencilBuilder builder;
	builder.setwidth(width);
	builder.setheight(height);
	builder.setsampleCount(getSampleCount());
	builder.setsampleQuality(getSampleQuality());
	depthStencil = std::make_unique<PipeLineDepthStencil>(dx.getGpuResourceManager().createDepthStencil(builder));
	applyRenderTargets();
	dx.applyViewport(0, 0, (float)width, (float)height);
	BaseLog::getInstance().debug("create pipe line buffer");
}
void RenderDevice::resizeWindow(UINT width, UINT height,const VideoMode *videoMode)
{
	BaseLog::getInstance().debug("resize window");
	if (videoMode != NULL)
	{
		this->setVideoMode(*videoMode);
	}
	this->renderDevice->ResizeBuffers(1, width, height, (DXGI_FORMAT)(int)this->video->getFormat(), 0);
	this->init(width, height);
}
void RenderDevice::present() const
{
	this->renderDevice->Present(0, 0);
}
void RenderDevice::beginRender() const
{
	if (boolsvalue & 1)
	{
		ClearColor(backgroundColor);
	}
	if (boolsvalue & 2)
	{
		ClearDepthStencil(true, 1, true, 0);
	}
}
void RenderDevice::endRender() const
{
	present();
}
void RenderDevice::applyRenderTargets() const
{
	RenderTargetViewType renderTargets;
	DepthStencilViewType depthTargets = nullptr;
#if CLDLEVEL >= 4
	if (this->depthStencil.get() != NULL)
	{
#endif
		if (texRenderTarget != nullptr)
		{
			renderTargets = (RenderTargetViewType)texRenderTarget->getRenderTarget();
			if (texDepthStencil != nullptr)
			{
				depthTargets = texDepthStencil->getDepthStencil();
			}
		}
		else
		{
			renderTargets = (RenderTargetViewType)renderTarget->getRenderTarget();
			depthTargets = depthStencil->getDepthStencil();
		}
#if CLDLEVEL >= 4
	}
#endif
	dx.getDx().getContext()->OMSetRenderTargets(1, &renderTargets, depthTargets);
}
void RenderDevice::clearBackBuffer() const
{
	ClearColor(backgroundColor);
}
void RenderDevice::setBackgroundColor(ColorNum R, ColorNum G, ColorNum B)
{
	backgroundColor = Color(R, G, B);
}
void RenderDevice::clearDepthStencil() const
{
	ClearDepthStencil(true, 1, true, 0);
}
void RenderDevice::ClearColor(const Color &col) const
{
#if CLDLEVEL >= 4
	if (this->renderTarget.get() != NULL)
	{
#endif
		dx.getDx().getContext()->ClearRenderTargetView(this->renderTarget->getRenderTarget(), col.getFloat(*dx.getShortPoolMemory()));
#if CLDLEVEL >= 4
	}
#endif
}
void RenderDevice::ClearDepthStencil(bool needClearDepth, float clearDepth, bool needClearStencil, UINT8 clearStencil) const
{
#if CLDLEVEL >= 4
	if (this->depthStencil.get() != NULL)
	{
#endif
		register char flags = 0;
		if (needClearDepth)
		{
			flags |= D3D11_CLEAR_DEPTH;
		}
		if (needClearStencil)
		{
			flags |= D3D11_CLEAR_STENCIL;
		}
		dx.getDx().getContext()->ClearDepthStencilView(this->depthStencil->getDepthStencil(), flags, clearDepth, clearStencil);
#if CLDLEVEL >= 4
	}
#endif
}
void RenderDevice::setVideoMode(const VideoMode &mode)
{
	this->video.reset(new VideoMode(mode));
}
void RenderDevice::setShouldClearBuf(bool clear)
{
	if (clear)
		boolsvalue |= 1;
	else
	{
		boolsvalue &= 254;
	}
}
bool RenderDevice::getShouldClearBuf() const
{
	return boolsvalue & 1;
}
void RenderDevice::setShouldClearDepthStencil(bool clear)
{
	if (clear)
	{
		boolsvalue |= 2;
	}
	else
	{
		boolsvalue &= 253;
	}
}
bool RenderDevice::getShouldClearDepthStencil() const
{
	return (boolsvalue & 2) != 0;
}
void RenderDevice::setSampleCount(UINT sample)
{
	this->sampleCount = sample;
}
void RenderDevice::setSampleQuality(UINT sample)
{
	this->sampleQuality = sample;
}
UINT RenderDevice::getSampleCount() const
{
	return this->sampleCount;
}
UINT RenderDevice::getSampleQuality() const
{
	return this->sampleQuality;
}
void RenderDevice::setAllowAltEnterFullScreen(bool allow)
{
	if (!allow)
	{
		IDXGIDevice * pDXGIDevice;
		dx.getDx().getDevice()->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
		IDXGIAdapter * pDXGIAdapter;
		pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
		IDXGIFactory * pIDXGIFactory;
		pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);
		pIDXGIFactory->MakeWindowAssociation(this->gethwnd(), DXGI_MWA_NO_ALT_ENTER);
		pIDXGIFactory->Release();
		pDXGIAdapter->Release();
		pDXGIDevice->Release();
	}
	else
	{
		BaseLog::getInstance().info("can not reallow alt enter full screen.to imulate this do it manual");
		throw "not implement";
	}
}
bool RenderDevice::getFullScreenMode() const
{
	BOOL ret;
	resValid(this->getrenderDevice()->GetFullscreenState(&ret, NULL));
	return ret != 0;
}
void RenderDevice::setFullScreenMode(bool isFullScreen)
{
	resValid(this->getrenderDevice()->SetFullscreenState((BOOL)isFullScreen, NULL));
}
void RenderDevice::renderToTexture(const PipeLineRenderTarget *renderTarget, const PipeLineDepthStencil *depthStencil)
{
	BaseLog::getInstance().trace("swap to render to texture");
	texRenderTarget = renderTarget;
	texDepthStencil = depthStencil;
	applyRenderTargets();
}
void RenderDevice::renderToBackBuffer()
{
	if (this->texRenderTarget != nullptr)
	{
		BaseLog::getInstance().trace("swap to render to back buffer");
		texRenderTarget = nullptr;
		texDepthStencil = nullptr;
		applyRenderTargets();
}
}
VideoMode RenderDevice::getVideoMode() const
{
	return *this->video;
}
GET_SET_METHOD_IMPL(hwnd, HWND, RenderDevice);
GET_SET_METHOD_IMPL(renderDevice, RenderDeviceType, RenderDevice);
GET_METHOD_IMPL(dx, const DXMain&, RenderDevice);