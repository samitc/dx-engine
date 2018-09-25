#ifndef _UNORDEREDVIEWINFO_H_
#define _UNORDEREDVIEWINFO_H_
#include "Engine.h"
#include "DXTypes.h"
enum class DataFormat;
class UnorderedViewInfo
{
public:
	struct UnorderedBuffer
	{
		unsigned int firstElement;
		unsigned int numOfElements;
		unsigned int flags;
	};
	struct UnorderedText
	{
		unsigned int mipSlice;
	};
	struct UnorderedTextArr :public UnorderedText
	{
		unsigned int firstArraySlice;
		unsigned int arraySize;
	};
	union UnorderedData
	{
		UnorderedBuffer        Buffer;
		UnorderedText         Texture1D;
		UnorderedTextArr   Texture1DArray;
		UnorderedText         Texture2D;
		UnorderedTextArr   Texture2DArray;
		UnorderedText         Texture3D;
	};
	enum UnorderedType 
	{
		UNKNOWN = 0,
		BUFFER = 1,
		TEXTURE1D = 2,
		TEXTURE1DARRAY = 3,
		TEXTURE2D = 4,
		TEXTURE2DARRAY = 5,
		TEXTURE3D = 8
	};
public://for engine
	UnorderedResourceInfoType getUnorderedInfoType() const;
	UnorderedResourceInfoType* getUnorderedInfoType(UnorderedResourceInfoType*info) const;
private:
	DataFormat format = (DataFormat)-1;
	UnorderedType type = (UnorderedType)-1;
	UnorderedData data;
	GET_SET_METHOD(format, DataFormat, public);
	GET_SET_METHOD(type, UnorderedType, public);
	GET_SET_METHOD(data, UnorderedData, public);
};
#endif