#ifndef _PIPELINEDEPTHSTENCIL_H_
#define _PIPELINEDEPTHSTENCIL_H_
#include "PipeLineBuffer.h"
#include "DXTypes.h"
class PipeLineDepthStencil :public PipeLineBuffer
{
public:
	class PipeLineDepthStencilBuilder :public PipeLineBufferBuilder
	{
	public:
		PipeLineDepthStencilBuilder() = default;
		~PipeLineDepthStencilBuilder() = default;
	private:
		DataFormat dataFormat;
		BUILDER_GET_SET_METHOD(dataFormat, DataFormat, public, PipeLineDepthStencilBuilder);
	};
	PipeLineDepthStencil() = delete;
	~PipeLineDepthStencil() = default;
public://for engine
	PipeLineDepthStencil(const DXMain&, Texture2D&&);
	PipeLineDepthStencil(const PipeLineDepthStencil&);
	const DepthStencilViewType getDepthStencil() const;
private:
};
#endif