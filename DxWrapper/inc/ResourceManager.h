#ifndef _RESOURCMANAGER_H_
#define _RESOURCMANAGER_H_
#include "Texture.h"
#include "Texture2D.h"
#include "Buffer.h"
class DXMain;
class ResourceManager
{
public:
	ResourceManager(const DXMain&);
	~ResourceManager() = default;
	Texture2D createTexture2D(const Texture2D::Texture2DBuilder&) const;
	Buffer createBuffer(unsigned int numberOfElements, unsigned int sizeOfElement, ResourceFormat bufferFormat, Access access = Access::GpuRead, const void * data = nullptr) const;
private:
	const DXMain& dx;
};
#endif