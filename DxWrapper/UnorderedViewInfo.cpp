#include "UnorderedViewInfo.h"
UnorderedResourceInfoType UnorderedViewInfo::getUnorderedInfoType() const
{
	UnorderedResourceInfoType ret;
	ret.Format = (DxFormat)(int)this->getformat();
	ret.ViewDimension = (DxUnoederedType)(int)this->gettype();
	void *data = &ret + sizeof DxFormat + sizeof DxUnoederedType;
	memcpy(data, &this->getdata(), sizeof UnorderedData);
	return ret;
}
UnorderedResourceInfoType * UnorderedViewInfo::getUnorderedInfoType(UnorderedResourceInfoType * info) const
{
	if ((int)this->getformat() == -1 || this->gettype() == -1)
	{
		return NULL;
	}
	else
	{
		info->Format = (DxFormat)(int)this->getformat();
		info->ViewDimension = (DxUnoederedType)(int)this->gettype();
		void * data = info + sizeof DxFormat + sizeof DxShaderType;
		memcpy(data, &this->getdata(), sizeof UnorderedData);
		return info;
	}
}
GET_SET_METHOD_IMPL(format, DataFormat, UnorderedViewInfo);
GET_SET_METHOD_IMPL(type, UnorderedViewInfo::UnorderedType, UnorderedViewInfo);
GET_SET_METHOD_IMPL(data, UnorderedViewInfo::UnorderedData, UnorderedViewInfo);