#ifndef _PIXELSHADER_H_
#define _PIXELSHADER_H_
#include "Shader.h"
#include <memory>
#include <list>
#include <vector>
#include "Engine.h"
#include "DXTypes.h"
class DXMain;
class ShaderResource;
class Sampler;
enum PsModel
{
	PS2_0,
	PS3_0,
	PS4_0,
	PS4_1,
	PS5_0,
};
//this class represent domain shader
class PixelShader:public Shader
{
public://public for everyone
	PixelShader(const PixelShader &copy);
	//create the domain shader
	//parameters:
	//	dx:
	//	path:the path of the shader file
	//	startFunction:the name of the entry function in the shader
	//	shaderModel:the level of the shader
	static PixelShader createPixelShader(const DXMain &dx, const UTF8 *path, const char *startFunction, PsModel shaderModel);
	//create the domain shader
	//parameters:
	//	dx:
	//	data:precompile shader
	static PixelShader createPixelShader(const DXMain &dx,const Data &data);
	virtual ~PixelShader();
	virtual PixelShader* clone() const;
	PixelShader &operator=(PixelShader& ass);
	void prepareForRender(const DXMain &dx) const;
	//convert the shader model to string that represent the level
	static const char* convertPsModelToString(PsModel model);
public://public for engine
 virtual bool checkIsMe(const char *name) const;
 virtual char* getName() const override;
 unsigned int getNumber() const override;
 PixelShader(std::vector<byte> &&, PixelShaderType shader);
private:
	PixelShaderType getPixelShaderType() const;
	PixelShaderType pixelShader;
};
#endif