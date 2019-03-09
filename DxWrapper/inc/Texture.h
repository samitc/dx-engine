#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include "Resource.h"
#include "DXTypes.h"
#include "Engine.h"
class Resource;
class DataManager;
class Texture :public Resource
{
public://public for everyone
	Texture();
	Texture(const Texture&);
	~Texture();
public://public for engine
	Texture(ResourceType texture);
	bool isCurrectResource(const IID resId) const;
	using dxType = Texture2DTypeS;
protected:
private:
};
#endif