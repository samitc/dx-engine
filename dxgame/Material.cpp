#include "Material.h"
#include "Color.h"
Material::Material(const Material & mat) :range(mat.range), opticalDensity(mat.opticalDensity), transparent(mat.transparent), transparentR(mat.transparentR), illumination(mat.illumination), ambColor(getNewColor(*mat.ambColor)),
difColor(getNewColor(*mat.difColor)), specColor(getNewColor(*mat.specColor)), trsFilter(getNewColor(*mat.trsFilter)), emissionColor(getNewColor(*mat.emissionColor)),
specLoc(NULL), diffLoc(NULL), ambLoc(NULL)
{
	register int l = strlen(mat.name) + 1;
	this->name = new char[l];
	strcpy(this->name, mat.name);
	if (mat.getdiffLoc() == nullptr)
	{
		this->setdiffLoc(NULL);
	}
	else
	{
		this->setDifMap(mat.getdiffLoc());
	}
	if (mat.getdiffLoc() == nullptr)
	{
		this->setdiffLoc(NULL);
	}
	else
	{
		this->setDifMap(mat.getdiffLoc());
	}
	if (mat.getambLoc() == nullptr)
	{
		this->setambLoc(NULL);
	}
	else
	{
		this->setAmbMap(mat.getambLoc());
	}
	if (mat.getspecLoc() == nullptr)
	{
		this->setspecLoc(NULL);
	}
	else
	{
		this->setSpecMap(mat.getspecLoc());
	}
}
Material::Material(const char * name) :range(0), opticalDensity(0), transparent(0), transparentR(0), illumination(0), ambColor(std::unique_ptr<Color>(nullptr)),
difColor(std::unique_ptr<Color>(nullptr)), specColor(std::unique_ptr<Color>(nullptr)), trsFilter(std::unique_ptr<Color>(nullptr)), emissionColor(std::unique_ptr<Color>(nullptr)),
specLoc(NULL), diffLoc(NULL), ambLoc(NULL)
{
	register int l = strlen(name) + 1;
	this->name = new char[l];
	strcpy(this->name, name);
}
Material::~Material()
{
}
const char * const Material::getName() const
{
	return this->name;
}
void Material::setAmb(float r, float g, float b)
{
	this->ambColor.reset(new Color(r, g, b));
}
void Material::setDif(float r, float g, float b)
{
	this->difColor.reset(new Color(r, g, b));
}
void Material::setEmission(float r, float g, float b)
{
	this->emissionColor.reset(new Color(r, g, b));
}
void Material::setSpec(float r, float g, float b)
{
	this->specColor.reset(new Color(r, g, b));
}
void Material::setTransmissionFilter(float r, float g, float b)
{
	this->trsFilter.reset(new Color(r, g, b));
}
void Material::setRange(float range)
{
	this->setrange(range);
}
void Material::setTransparentD(float transparent)
{
	this->settransparent(transparent);
}
void Material::setTransparentTr(float transparent)
{
	this->settransparentR(transparent);
}
void Material::setOpticalDensity(float density)
{
	this->setopticalDensity(density);
}
void Material::setIllumination(int illumination)
{
	this->setillumination(illumination);
}
void Material::setDifMap(const char * loc)
{
	register int l = strlen(loc) + 1;
	this->setdiffLoc(new char[l]);
	strcpy(this->getdiffLoc(), loc);
}
void Material::setAmbMap(const char * loc)
{
	register int l = strlen(loc) + 1;
	this->setambLoc(new char[l]);
	strcpy(this->getambLoc(), loc);
}
void Material::setSpecMap(const char * loc)
{
	register int l = strlen(loc) + 1;
	this->setspecLoc(new char[l]);
	strcpy(this->getspecLoc(), loc);
}
Color * Material::getNewColor(Color & col)
{
	if (&col != NULL)
	{
		return new Color(col);
	}
	return nullptr;
}
SAME_GET_SET_METHOD_IMPL(name, char*, Material);
SAME_GET_SET_METHOD_IMPL(diffLoc, char*, Material);
SAME_GET_SET_METHOD_IMPL(ambLoc, char*, Material);
SAME_GET_SET_METHOD_IMPL(specLoc, char*, Material);
GET_SET_METHOD_IMPL(range, float, Material);
GET_SET_METHOD_IMPL(transparent, float, Material);
GET_SET_METHOD_IMPL(transparentR, float, Material);
GET_SET_METHOD_IMPL(opticalDensity, float, Material);
GET_SET_METHOD_IMPL(illumination, int, Material);