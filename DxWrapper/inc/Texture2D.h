#ifndef _TEXTURE2D_H_
#define _TEXTURE2D_H_
#include "Texture.h"
#include "DXTypes.h"
#include "Engine.h"
class Resource;
class ShaderResource;
class DXMain;
class TextureData;
enum class DataFormat;
//this class represent 2 diminution texture 
class Texture2D:public Texture
{
public://public for everyone
	//builder class for texture2D
	class Texture2DBuilder:public ResourceBuilder
	{
	public:
		Texture2DBuilder() = delete;
		Texture2DBuilder(UINT width, UINT height, DataFormat format);
		Texture2DBuilder(UINT width, UINT height, DataFormat format, Access access);
		Texture2DBuilder(Access access,ResourceFormat resourceFormat, UINT width, UINT height, UINT mipLevel,
			UINT arraySize,
			DataFormat format,
			UINT sampleCount,
			UINT sampleQuality,
			UINT miscFlags);
	private:
		UINT width;
		UINT height;
		UINT mipLevel;
		UINT arraySize;
		DataFormat dFormat;
		UINT sampleCount;
		UINT sampleQuality;
		UINT miscFlags;
		static CONSTEXPRN UINT MIP_LEVEL_DEFAULT = 1;
		static CONSTEXPRN UINT ARRAY_SIZE_DEFAULT = 1;
		static CONSTEXPRN UINT SAMPLE_COUNT_DEFAULT = 1;
		static CONSTEXPRN UINT SAMPLE_QUALITY_DEFAULT = 0;
		static CONSTEXPRN UINT MISC_FLAGS_DEFAULT = 0;
		BUILDER_GET_SET_METHOD(width, UINT, public, Texture2DBuilder);
		BUILDER_GET_SET_METHOD(height, UINT, public, Texture2DBuilder);
		BUILDER_GET_SET_METHOD(mipLevel, UINT, public, Texture2DBuilder);
		BUILDER_GET_SET_METHOD(arraySize, UINT, public, Texture2DBuilder);
		BUILDER_GET_SET_METHOD(dFormat, DataFormat, public, Texture2DBuilder);
		BUILDER_GET_SET_METHOD(sampleCount, UINT, public, Texture2DBuilder);
		BUILDER_GET_SET_METHOD(sampleQuality, UINT, public, Texture2DBuilder);
		BUILDER_GET_SET_METHOD(miscFlags, UINT, public, Texture2DBuilder);
	};
	Texture2D();
	Texture2D(const Texture2D &copy);
	~Texture2D();
	Texture2D& operator=(Texture2D& obj);
	CONSTEXPR TextureData getTextureData() const;
public://public for engine
	TextureDesc getDesc() const;
	Texture2D(ResourceType texture);
	Texture2DType getTexture() const;
	using dxType = Texture2DTypeS;
private:
};
#endif