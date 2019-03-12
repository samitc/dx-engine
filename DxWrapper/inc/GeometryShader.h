#ifndef _GEOMETRYSHADER_H_
#define _GEOMETRYSHADER_H_
#include "Shader.h"
#include "Engine.h"
#include "DXTypes.h"
enum class GsModel
{
	GS2_0,
	GS3_0,
	GS4_0,
	GS4_1,
	GS5_0,
};
//this class represent geomtry shader
class GeometryShader:public Shader
{
public://public for everyone
	GeometryShader() = delete;;
	GeometryShader(const GeometryShader&);
	virtual ~GeometryShader();
	GeometryShader* clone() const override;
	GeometryShader &operator=(GeometryShader&);
	void prepareForRender(const DXMain &dx) const override;
	//create the geometry shader
	//parameters:
	//	dx:
	//	path:the path of the shader file
	//	startFunction:the name of the entry function in the shader
	//	shaderModel:the level of the shader
	static GeometryShader createGeometryShader(const DXMain &dx, const UTF8 *path, const char *startFunction, GsModel shaderModel);
	//create the geometry shader
	//parameters:
	//	dx:
	//	data:precompile shader
	static GeometryShader createGeometryShader(const DXMain &dx, const Data &data);
	//convert the shader model to string that represent the level
	static const char* convertGsModelToString(GsModel model);
    virtual bool operator==(const Shader&) const;
public://public for engine
	bool checkIsMe(const char *name) const override;
	char* getName() const override;
	unsigned int getNumber() const override;
	GeometryShader(std::vector<byte> &&, GeometryShaderType);
protected:
private:
	GeometryShaderType geometryShader;
};
#endif