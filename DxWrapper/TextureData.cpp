#include "TextureData.h"
TextureData::TextureData(unsigned int Width, unsigned int Height, unsigned int MipLevels, unsigned int ArraySize, Format format, unsigned int sampleCount, unsigned int sampleQuality) :
	width(Width), height(Height), mipLevels(MipLevels), arraySize(ArraySize), format(format), sampleCount(sampleCount), sampleQuality(sampleQuality)
{
}
GET_SET_METHOD_IMPL(width, unsigned int, TextureData);
GET_SET_METHOD_IMPL(height, unsigned int, TextureData);
GET_SET_METHOD_IMPL(mipLevels, unsigned int, TextureData);
GET_SET_METHOD_IMPL(arraySize, unsigned int, TextureData);
GET_SET_METHOD_IMPL(format, Format, TextureData);
GET_SET_METHOD_IMPL(sampleCount, unsigned int, TextureData);
GET_SET_METHOD_IMPL(sampleQuality, unsigned int, TextureData);