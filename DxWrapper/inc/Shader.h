#ifndef _SHADER_H_
#define _SHADER_H_
#include "Engine.h"
#include "DXTypes.h"
#include <vector>
#include <memory>
class Data;
class DXMain;
class Device;
class DataManager;
//base class for shader
class Shader
{
public://public for everyone
	virtual ~Shader();
	virtual Shader* clone() const = 0;
	//create the shader
	//parameters:
	//	path:the path of the shader file
	//	startFunction:the name of the entry function in the shader
	//	shaderModel:the level of the shader
	static Data compileShader(const UTF8 *path, const char *startFunction, const char *shaderModel);
	void saveShader(const DataManager &manager, const DXMain &dx, const char *id) const;
	static Shader* loadShader(const DataManager &manager, const DXMain &dx, const char *id);
public://public for engine
	virtual void prepareForRender(const DXMain &dx) const = 0;
	virtual bool checkIsMe(const char *name) const = 0;
	virtual char* getName() const = 0;
	virtual unsigned int getNumber() const = 0;
	static byte* save(const Shader*, int *);
	static Shader* load(const byte *, int, const Device*);
protected://for engine
	Shader(std::vector<byte> &&data);
	Shader(const Shader& shader);
	static std::vector<byte> createVectorFromData(const Data& dat);
private:
	Shader();
	std::vector<byte> d;
};
#endif