#include "Texture2D.h"
#include "ShaderResource.h"
#include "DXMain.h"
#include "Device.h"
#include "TextureData.h"
#include "DataManager.h"
Texture2D::Texture2D() :Texture()
{
	
}
Texture2D::Texture2D(const Texture2D &copy) : Texture(copy)
{
	
}
Texture2D::~Texture2D()
{
}
TextureDesc Texture2D::getDesc() const
{
	TextureDesc d;
	this->getTexture()->GetDesc(&d);
	return d;
}
Texture2D::Texture2D(ResourceType texture) :Texture(texture)
{
}
Texture2D & Texture2D::operator=(Texture2D & obj)
{
	if (this != &obj)
	{
		if (this->getResource() != nullptr)
		{
			this->getResource()->Release();
		}
		this->setResource(obj.getResource());
		if (this->getResource() != nullptr)
		{
			this->getResource()->AddRef();
		}
	}
	return *this;
}
CONSTEXPR TextureData Texture2D::getTextureData() const
{
	TextureDesc d;
	this->getTexture()->GetDesc(&d);
	return TextureData(d.Width, d.Height, d.MipLevels, d.ArraySize, (Format)d.Format, d.SampleDesc.Count, d.SampleDesc.Quality);
}
Texture2DType Texture2D::getTexture() const
{
	return (Texture2DType)(this->getResource());
}
Texture2D::Texture2DBuilder::Texture2DBuilder(UINT width, UINT height, DataFormat format)
	:Texture2DBuilder(width, height, format, ResourceBuilder::ACCESS_DEFAULT)
{
}
Texture2D::Texture2DBuilder::Texture2DBuilder(UINT width, UINT height, DataFormat format, Access access) :
	Texture2DBuilder(access, ResourceBuilder::FORMAT_DEFAULT, width, height, MIP_LEVEL_DEFAULT, ARRAY_SIZE_DEFAULT, format, SAMPLE_COUNT_DEFAULT, SAMPLE_QUALITY_DEFAULT, MISC_FLAGS_DEFAULT)
{
}
Texture2D::Texture2DBuilder::Texture2DBuilder(Access access, ResourceFormat resourceFormat, UINT width, UINT height, UINT mipLevel, UINT arraySize, DataFormat format, UINT sampleCount, UINT sampleQuality, UINT miscFlags) :ResourceBuilder(access, resourceFormat),
width(width), height(height), mipLevel(mipLevel), arraySize(arraySize), dFormat(format), sampleCount(sampleCount), sampleQuality(sampleQuality), miscFlags(miscFlags)
{
}
BUILDER_GET_SET_METHOD_IMPL(width, UINT, Texture2D::Texture2DBuilder);
BUILDER_GET_SET_METHOD_IMPL(height, UINT, Texture2D::Texture2DBuilder);
BUILDER_GET_SET_METHOD_IMPL(mipLevel, UINT, Texture2D::Texture2DBuilder);
BUILDER_GET_SET_METHOD_IMPL(arraySize, UINT, Texture2D::Texture2DBuilder);
BUILDER_GET_SET_METHOD_IMPL(dFormat, DataFormat, Texture2D::Texture2DBuilder);
BUILDER_GET_SET_METHOD_IMPL(sampleCount, UINT, Texture2D::Texture2DBuilder);
BUILDER_GET_SET_METHOD_IMPL(sampleQuality, UINT, Texture2D::Texture2DBuilder);
BUILDER_GET_SET_METHOD_IMPL(miscFlags, UINT, Texture2D::Texture2DBuilder);