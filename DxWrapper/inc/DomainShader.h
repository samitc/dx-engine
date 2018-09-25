#ifndef _DOMAINSHADER_H_
#define _DOMAINSHADER_H_
#include "Shader.h"
#include "DXTypes.h"
enum class DsModel
{
	DS2_0,
	DS3_0,
	DS4_0,
	DS4_1,
	DS5_0,
};
//this class represent domain shader
class DomainShader :public Shader
{
public:
	DomainShader()=delete;
	DomainShader(const DomainShader&);
	~DomainShader();
	DomainShader* clone() const override;
	void prepareForRender(const DXMain &dx) const override;
	//create the domain shader
	//parameters:
	//	dx:
	//	path:the path of the shader file
	//	startFunction:the name of the entry function in the shader
	//	shaderModel:the level of the shader
	static DomainShader createDomainShader(const DXMain &dx, const UTF8 *path, const char *startFunction, DsModel shaderModel);
	//create the domain shader
	//parameters:
	//	dx:
	//	data:precompile shader
	static DomainShader createDomainShader(const DXMain &dx, const Data &data);
	//convert the shader model to string that represent the level
	static const char* convertDsModelToString(DsModel model);
public://public for engine
	bool checkIsMe(const char *name) const override;
	char* getName() const override;
	unsigned int getNumber() const override;
	DomainShader(std::vector<byte> &&, DomainShaderType);
private:
	DomainShaderType domainShader;
};
#endif