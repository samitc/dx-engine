#ifndef _PIPELINERENDERTARGET_H_
#define _PIPELINERENDERTARGET_H_
#include "PipeLineBuffer.h"
#include "DXTypes.h"
class PipeLineRenderTarget :public PipeLineBuffer
{
public:
	class PipeLineRenderTargetBuilder :public PipeLineBufferBuilder
	{
	public:
		PipeLineRenderTargetBuilder() = default;
		~PipeLineRenderTargetBuilder() = default;
	private:
		DataFormat dataFormat;
		BUILDER_GET_SET_METHOD(dataFormat, DataFormat, public, PipeLineRenderTargetBuilder);
	};
	PipeLineRenderTarget() = delete;
	~PipeLineRenderTarget() = default;
public://for engine
	PipeLineRenderTarget(const PipeLineRenderTarget&);
	PipeLineRenderTarget(const DXMain&, Texture2D&&);
	const RenderTargetViewType getRenderTarget() const;
private:
};
#endif