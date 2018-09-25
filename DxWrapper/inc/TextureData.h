#ifndef _TEXTUREDATA_H_
#define _TEXTUREDATA_H_
#include "Engine.h"
enum Format;
class TextureData
{
public:
public://for engine
	CONSTEXPR TextureData(unsigned int Width, unsigned int Height, unsigned int MipLevels,
		unsigned int ArraySize, Format format, unsigned int sampleCount, unsigned int sampleQuality);
private:
	unsigned int width; 
	unsigned int height;
	unsigned int mipLevels;
	unsigned int arraySize;
	Format format;
	unsigned int sampleCount;
	unsigned int sampleQuality;
	GET_METHOD(width, unsigned int, public);
	GET_METHOD(height, unsigned int, public);
	GET_METHOD(mipLevels, unsigned int, public);
	GET_METHOD(arraySize, unsigned int, public);
	GET_METHOD(format, Format, public);
	GET_METHOD(sampleCount, unsigned int, public);
	GET_METHOD(sampleQuality, unsigned int, public);
	SET_METHOD(width, unsigned int, private);
	SET_METHOD(height, unsigned int, private);
	SET_METHOD(mipLevels, unsigned int, private);
	SET_METHOD(arraySize, unsigned int, private);
	SET_METHOD(format, Format, private);
	SET_METHOD(sampleCount, unsigned int, private);
	SET_METHOD(sampleQuality, unsigned int, private);
};
#endif