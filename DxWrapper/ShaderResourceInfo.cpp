#include "ShaderResourceInfo.h"
ShaderResourceInfoType ShaderResourceInfo::getShaderResourceInfoType() const
{
	ShaderResourceInfoType ret;
	ret.Format = (DxFormat)(int)this->getformat();
	ret.ViewDimension = (DxShaderType)(int)this->gettype();
	void * data = &ret + sizeof DxFormat + sizeof DxShaderType;
	memcpy(data, &this->getdata(), sizeof ShaderData);
	return ret;
}

ShaderResourceInfoType *ShaderResourceInfo::getShaderResourceInfoType(ShaderResourceInfoType * info) const
{
	if ((int)this->getformat() == -1 || this->gettype() == -1)
	{
		return NULL;
	}
	else
	{
		info->Format = (DxFormat)(int)this->getformat();
		info->ViewDimension = (DxShaderType)(int)this->gettype();
		void * data = info + sizeof DxFormat + sizeof DxShaderType;
		memcpy(data, &this->getdata(), sizeof ShaderData);
		return info;
	}
}
GET_SET_METHOD_IMPL(format, DataFormat, ShaderResourceInfo);
GET_SET_METHOD_IMPL(type, ShaderResourceInfo::ShaderType, ShaderResourceInfo);
GET_SET_METHOD_IMPL(data, ShaderResourceInfo::ShaderData, ShaderResourceInfo);