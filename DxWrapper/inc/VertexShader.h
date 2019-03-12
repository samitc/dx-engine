#ifndef _VERTEXSHADER_H_
#define _VERTEXSHADER_H_
#include "Shader.h"
#include <memory>
#include <list>
#include <vector>
class Shader;
class Buffer;
class DXMain;
enum VsModel
{
	VS2_0,
	VS3_0,
	VS4_0,
	VS4_1,
	VS5_0,
};
//this class represent vertex shader
class VertexShader:public Shader
{
public://public for everyone
	VertexShader(const VertexShader &copy);
	//create the vertex shader
	//parameters:
	//	dx:
	//	path:the path of the shader file
	//	startFunction:the name of the entry function in the shader
	//	shaderModel:the level of the shader
	static VertexShader createVertexShader(const DXMain &dx, const UTF8 *path, const char *startFunction, VsModel shaderModel);
	//create the vertex shader
	//parameters:
	//	dx:
	//	data:precompile shader
	static VertexShader createVertexShader(const DXMain &dx, const Data &data);
	virtual ~VertexShader();
	virtual VertexShader* clone() const;
    virtual bool operator==(const Shader&) const;
	VertexShader &operator=(VertexShader& ass);
	void prepareForRender(const DXMain &dx) const;
	//convert the shader model to string that represent the level
	static const char* convertVsModelToString(VsModel model);
public://public for engine
 virtual bool checkIsMe(const char *name) const;
	VertexShaderType getVertexShaderType() const;
	virtual char* getName() const override;
	unsigned int getNumber() const override;
	VertexShader(std::vector<byte>&&, VertexShaderType shader);
private:
	VertexShaderType vertexShader;
};
#endif