#include "PipeLineBuffer.h"
#include <utility>
PipeLineBuffer::PipeLineBuffer(ResourceViewType resView, Texture2D && texture) :ResourceView(resView), texture(std::move(texture))
{
}
PipeLineBuffer::PipeLineBuffer(ResourceViewsType resView, int viewsCount,Texture2D && texture) :ResourceView(resView, viewsCount),texture(std::move(texture))
{
}
BUILDER_GET_SET_METHOD_IMPL(width, unsigned int, PipeLineBuffer::PipeLineBufferBuilder);
BUILDER_GET_SET_METHOD_IMPL(height, unsigned int, PipeLineBuffer::PipeLineBufferBuilder);
BUILDER_GET_SET_METHOD_IMPL(sampleCount, unsigned int, PipeLineBuffer::PipeLineBufferBuilder);
BUILDER_GET_SET_METHOD_IMPL(sampleQuality, unsigned int, PipeLineBuffer::PipeLineBufferBuilder);