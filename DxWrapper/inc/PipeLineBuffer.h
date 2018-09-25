#ifndef _PIPELINEBUFFER_H_
#define _PIPELINEBUFFER_H_
#include "ResourceView.h"
#include "Engine.h"
#include "Texture2D.h"
class PipeLineBuffer:public ResourceView
{
public:
	class PipeLineBufferBuilder
	{
	public:
		PipeLineBufferBuilder() = default;
		~PipeLineBufferBuilder() = default;
	protected:
		unsigned int width;
		unsigned int height;
		unsigned int sampleCount;
		unsigned int sampleQuality;
		BUILDER_GET_SET_METHOD(width, unsigned int, public, PipeLineBufferBuilder);
		BUILDER_GET_SET_METHOD(height, unsigned int, public, PipeLineBufferBuilder);
		BUILDER_GET_SET_METHOD(sampleCount, unsigned int, public, PipeLineBufferBuilder);
		BUILDER_GET_SET_METHOD(sampleQuality, unsigned int, public, PipeLineBufferBuilder);
	};
	PipeLineBuffer() = delete;
	~PipeLineBuffer() = default;
protected://for engine
	PipeLineBuffer(const PipeLineBuffer&) = default;
	PipeLineBuffer(ResourceViewType resView, Texture2D &&texture);
	PipeLineBuffer(ResourceViewsType resView, int viewsCount, Texture2D &&texture);
	Texture2D texture;
};
#endif