#include "LayoutElement.h"
#include <string>
#include "GeneralException.h"
#include "BaseLog.h"
#include "DxEnums.h"
LayoutElement::LayoutElement(const char * const semanticName, DataFormat format, int index, int bufferIndex, bool isInstanceData, int offset, int drawPerInstance)
	:format(format), index(index), instanceData(isInstanceData), offset(offset), drawPerInstance(drawPerInstance), bufferIndex(bufferIndex)
{
	register int l = strlen(semanticName) + 1;
	this->semanticName = new char[l];
	strcpy_s(this->semanticName, l, semanticName);
}
LayoutElement::LayoutElement(const LayoutElement &copy) :LayoutElement(copy.semanticName, copy.format, copy.index, copy.getBufferIndex(), copy.instanceData, copy.getOffset(), copy.getDrawPerInstance())
{
}
LayoutElement::~LayoutElement()
{
	delete []this->semanticName;
}
LayoutElement &LayoutElement::operator=(LayoutElement &ass)
{
	if (this != &ass)
	{
		this->index = ass.index;
		this->format = ass.format;
		delete[] this->semanticName;
		register unsigned char l = (unsigned char)strlen(ass.semanticName) + 1;
		this->semanticName = new char[l];
		strcpy_s(this->semanticName, l, ass.semanticName);
	}
	return *this;
}
int LayoutElement::getSize() const
{
	if (format >= DataFormat::R32G32B32A32_TYPELESS && format <= DataFormat::R32G32B32A32_SINT)
	{
		return 16;
	}
	else if (format >= DataFormat::R32G32B32_TYPELESS && format <= DataFormat::R32G32B32_SINT)
	{
		return 12;
	}
	else if (format >= DataFormat::R16G16B16A16_TYPELESS && format <= DataFormat::X32_TYPELESS_G8X24_UINT)
	{
		return 8;
	}
	else if (format >= DataFormat::R10G10B10A2_TYPELESS && format <= DataFormat::X24_TYPELESS_G8_UINT)
	{
		return 4;
	}
	else if (format >= DataFormat::R8G8_TYPELESS && format <= DataFormat::R16_SINT)
	{
		return 2;
	}
	else if (format >= DataFormat::R8_TYPELESS && format <= DataFormat::A8_UNORM)
	{
		return 1;
	}
	else
	{
		std::string logStr("unknown format:");
		logStr.append(std::to_string((int)format));
		 BaseLog::getInstance().error(logStr.c_str());
		throw GeneralException("unknown format in layout element");
	}
}
int LayoutElement::getIndex() const
{
	return this->index;
}
const char * const LayoutElement::getSemanticName() const
{
	return this->semanticName;
}
DataFormat LayoutElement::getFormat() const
{
	return this->format;
}
bool LayoutElement::getIsInstanceData() const
{
	return instanceData;
}

int LayoutElement::getOffset() const
{
	return offset;
}

int LayoutElement::getBufferIndex() const
{
	return bufferIndex;
}

int LayoutElement::getDrawPerInstance() const
{
	return drawPerInstance;
}
