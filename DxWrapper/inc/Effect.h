#ifndef _EFFECT_H_
#define _EFFECT_H_
#include "Shader.h"
#include <d3dx11effect.h>
#include <map>
#include "DXTypes.h"
#include "DXGame.h"
class Matrix;
class Vector;
class Buffer;
class Data;
class ShaderResource;
enum FsModel
{
	FS2_0,
	FS3_0,
	FS4_0,
	FS4_1,
	FS5_0,
};
//this class represent an effect
class Effect:public Shader
{
public:
	//builder class for effect
	class EffectBuilder
	{
	public:
		EffectBuilder(const UTF8 *loc,FsModel model);
		EffectBuilder(const Data &data);
		EffectBuilder(const EffectBuilder& copy);
		~EffectBuilder();
		//build the effect
		//parameters:
		//	dx:
		//	t:the technique from the effect to use
		Effect build(DXMain &dx,const char *t);
	private:
		SIZE_T l;
		void *data;
		DxEffect *effect;
		std::map<std::string, ID3DX11EffectVariable*> variables;
		SAME_GET_SET_METHOD(data, void*, private);
		GET_SET_METHOD(l, SIZE_T, private);
		static const char* convertFsModelToString(FsModel model);
	};
public:
	Effect(const Effect& copy);
	~Effect();
	virtual void prepareForRender(const DXMain &dx) const;
	virtual Shader* clone() const;
	//set effect parameter by name
	void setMatrixVar(const char* varName, const Matrix &matrix);
	//set effect parameter by name
	void setMatrixArrayVar(const char* varName, const Matrix *matrix, int startIndex, int numOfMatrix);
	//set effect parameter by name
	void setVectorVar(const char* varName, const Vector& vector);
	//set effect parameter by name
	void setBufferVar(const char* varName, const Buffer& buffer);
	//set effect parameter by name
	void setScalarVar(const char* varName, const float scalar);
	//set effect parameter by name
	void setShaderResourceVar(const char* varName, const ShaderResource &res);
	//set general (any type) effect parameter by name
	//parameters:
	//	varName:the name of the shader variable
	//	data:the data to set the variable to 
	//	count:the size of the data in bytes
	//	offset:the offset from the begging of the data to start reading
	void setDataVar(const char* varName, const void *data, unsigned int count, unsigned int offset = 0);
	void setTechnique(const char* tec);
    virtual bool operator==(const Shader&) const;
public://for engine
	Effect(std::vector<byte> &&dat, DxEffect effect, const char *t);
	virtual bool checkIsMe(const char *name) const;
	BYTE* getInputSignature(SIZE_T &size) const;
	virtual char* getName() const override;
	unsigned int getNumber() const override;
	const byte* getTechData(unsigned int *size) const;
private:
	void updateVariables();
	void updateTechniques();
	DxEffect effect;
	char* t;
	unsigned int tSize;
	std::map<char*, ID3DX11EffectVariable*> variables;
	std::map<char*, ID3DX11EffectTechnique*> techniques;
	GET_SET_METHOD(effect, DxEffect, private);
	SAME_GET_SET_METHOD(t, char*, private);
};
#endif