#ifndef _HULLSHADER_H_
#define _HULLSHADER_H_
#include "Shader.h"
#include "DXTypes.h"
enum class HsModel
{
	HS2_0,
	HS3_0,
	HS4_0,
	HS4_1,
	HS5_0,
};
//this class represent hull shader
class HullShader:public Shader
{
public:
	HullShader() = delete;
	HullShader(const HullShader&);
	~HullShader();
	HullShader* clone() const override;
	void prepareForRender(const DXMain &dx) const override;
	//create the hull shader
	//parameters:
	//	dx:
	//	path:the path of the shader file
	//	startFunction:the name of the entry function in the shader
	//	shaderModel:the level of the shader
	static HullShader createHullShader(const DXMain &dx, const UTF8 *path, const char *startFunction, HsModel shaderModel);
	//create the hull shader
	//parameters:
	//	dx:
	//	data:precompile shader
	static HullShader createHullShader(const DXMain &dx, const Data &data);
	//convert the shader model to string that represent the level
	static const char* convertHsModelToString(HsModel model);
public://public for engine
	bool checkIsMe(const char *name) const override;
	char* getName() const override;
	unsigned int getNumber() const override;
	HullShader(std::vector<byte> &&, HullShaderType);
private:
	HullShaderType hullShader;
};
#endif