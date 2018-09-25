#ifndef _SHADERRESOURCEINFO_H_
#define _SHADERRESOURCEINFO_H_
#include "Engine.h"
#include "DXTypes.h"
enum class DataFormat;
//this class help to build shader resource
class ShaderResourceInfo
{
public:
	struct ShaderBuffer
	{
		unsigned int firstElement;
		union
		{
			unsigned int numOfElements;
			unsigned int elementWidth;
		};
	};
	struct ShaderText
	{
		unsigned int mostDetailedMip;
		unsigned int mipLevels;
	};
	struct ShaderTextArr :public ShaderText
	{
		unsigned int firstArraySlice;
		unsigned int arraySize;
	};
	struct ShaderTextCubeArr :public ShaderText
	{
		unsigned int first2DTexture;
		unsigned int numOfCubeTexture;
	};
	struct ShaderArray
	{
		unsigned int firstArraySlice;
		unsigned int arraySize;
	};
	struct ShaderBufferEx
	{
		unsigned int firstElement;
		unsigned int numOfElements;
		unsigned int flags;
	};
	union ShaderData {
		ShaderBuffer        Buffer;
		ShaderText         Texture1D;
		ShaderTextArr   Texture1DArray;
		ShaderText         Texture2D;
		ShaderTextArr   Texture2DArray;
		//D3D11_TEX2DMS_SRV       Texture2DMS;empty struct
		ShaderArray Texture2DMSArray;
		ShaderText         Texture3D;
		ShaderText       TextureCube;
		ShaderTextCubeArr TextureCubeArray;
		ShaderBufferEx      BufferEx;
	};
	enum ShaderType {
		UNKNOWN = 0,
		BUFFER = 1,
		TEXTURE1D = 2,
		TEXTURE1DARRAY = 3,
		TEXTURE2D = 4,
		TEXTURE2DARRAY = 5,
		TEXTURE2DMS = 6,
		TEXTURE2DMSARRAY = 7,
		TEXTURE3D = 8,
		TEXTURECUBE = 9,
		TEXTURECUBEARRAY = 10,
		BUFFEREX = 11
	};
public://for engine
	ShaderResourceInfoType getShaderResourceInfoType() const;
	ShaderResourceInfoType* getShaderResourceInfoType(ShaderResourceInfoType *info) const;
private:
	DataFormat format=(DataFormat)-1;
	ShaderType type=(ShaderType)-1;
	ShaderData data;
	GET_SET_METHOD(format, DataFormat, public);
	GET_SET_METHOD(type, ShaderType, public);
	GET_SET_METHOD(data, ShaderData, public);
};
#endif