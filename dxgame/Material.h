#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include <memory>
#include "Engine.h"
class Color;
class Material
{
public:
	Material(const Material &mat);
	Material(const char* name);
	~Material();
	const char* const getName() const;
	void setAmb(float r, float g, float b);
	void setDif(float r, float g, float b);
	void setEmission(float r, float g, float b);
	void setSpec(float r, float g, float b);
	void setTransmissionFilter(float r, float g, float b);
	void setRange(float range);
	void setTransparentD(float transparent);
	void setTransparentTr(float transparent);
	void setOpticalDensity(float density);
	void setIllumination(int illumination);
	void setDifMap(const char* loc);
	void setAmbMap(const char* loc);
	void setSpecMap(const char* loc);
private:
	Color* getNewColor(Color &col);
	float range;
	float transparent;
	float transparentR;
	float opticalDensity;
	int illumination;
	std::unique_ptr<Color> ambColor;
	std::unique_ptr<Color> difColor;
	std::unique_ptr<Color> emissionColor;
	std::unique_ptr<Color> specColor;
	std::unique_ptr<Color> trsFilter;
	char* name;
	char* diffLoc;
	char* ambLoc;
	char* specLoc;
	SAME_GET_SET_METHOD(name, char*, private);
	SAME_SET_METHOD(diffLoc, char*, private);
	SAME_SET_METHOD(ambLoc, char*, private);
	SAME_SET_METHOD(specLoc, char*, private);
	SAME_GET_METHOD(diffLoc, char*, public);
	SAME_GET_METHOD(ambLoc, char*, public);
	SAME_GET_METHOD(specLoc, char*, public);
	GET_SET_METHOD(range, float, private);
	GET_SET_METHOD(transparent, float, private);
	GET_SET_METHOD(transparentR, float, private);
	GET_SET_METHOD(opticalDensity, float, private);
	GET_SET_METHOD(illumination, int, private);
};
#endif