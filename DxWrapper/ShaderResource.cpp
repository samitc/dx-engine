#include "ShaderResource.h"
#include "Device.h"
#include "Resource.h"
#include "ShaderResourceInfo.h"
ShaderResource::ShaderResource(const ShaderResource &copy) :ResourceView(copy)
{
}
ShaderResource & ShaderResource::operator=(const ShaderResource & obj)
{
	this->ResourceView::operator=(obj);
	return *this;
}
ShaderResource::ShaderResource(const ShaderResourceType res)
{
    this->setResView(res);
#if CLDLEVEL >= 4
    static int number = 0;
    char name[] = "Shader resource:   ";
    char *p = name + strlen(name) - 1;
    int num = number++;
    while (num > 0)
    {
        *p = (num % 10) + 48;
        num /= 10;
        p--;
    }
    res->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
#endif
}
ShaderResourceType ShaderResource::getResView() const
{
	return (ShaderResourceType)this->getResourceView();
}