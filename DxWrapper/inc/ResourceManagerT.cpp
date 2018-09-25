#ifndef _RESOURCMANAGERT_CPP_
#define _RESOURCMANAGERT_CPP_
#include "ResourceManager.h"
#include "Device.h"
template <class T> T ResourceManager::loadTexture(const UTF8 * loc, const TextureInfo & info) const
{
	ResourceType t = dx.getDevice().getTextureType(loc, info);
	T ret(t);
	if (!ret.isCurrectResource(__uuidof(T::dxType)))
	{
		throw "Not implement";
	}
	return ret;
}
#endif