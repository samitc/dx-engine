#include "Texture.h"
#include "Device.h"
#include "DXMain.h"
#include "DataManager.h"
#include "Data.h"
#include "Texture2D.h"
Texture::Texture(ResourceType texture) :Resource(texture)
{
}
Texture::Texture()
{
}
Texture::Texture(const Texture & copy) :Resource(copy)
{
}
Texture::~Texture()
{
}
bool Texture::isCurrectResource(const IID resId) const
{
	ResourceType ret;
	HRESULT res = this->getResource()->QueryInterface(resId, (void **)&ret);
	register bool retu = !(res == E_NOINTERFACE || (res == E_POINTER&&ret == NULL));
	if (retu)
	{
		ret->Release();
		return true;
	}
	return false;
}