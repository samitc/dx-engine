#include "Effect.h"
#include "Data.h"
#include "DXObject.h"
#include "Device.h"
#include "DXMain.h"
#include "Matrix.h"
#include "Vector.h"
#include "Buffer.h"
#include "ShaderResource.h"
#include "ShaderState.h"
char* copyStr(const char* src)
{
	int size = strlen(src) + 1;
	char *dst = new char[size];
	strcpy(dst, src);
	return dst;
}
template<class T> T* findVar(const char* varName, const std::map<char*, T*>& vars)
{
	for (auto var : vars)
	{
		if (strcmp(varName, var.first) == 0)
		{
			return var.second;
		}
	}
	return nullptr;
}
template<class T> void copyMap(const std::map<char*, T*> &s, std::map<char*, T*> &d)
{
	for (auto m : s)
	{
		d.insert(d.cend(), std::pair<char*, T*>(copyStr(m.first), m.second));
	}
}
Effect::EffectBuilder::EffectBuilder(const UTF8 * loc, FsModel model)
{
	Data d = Shader::compileShader(loc, "", this->convertFsModelToString(model));
	this->setl(d.getSize());
	this->setdata(new char[this->getl()]);
	memcpy(this->getdata(), d.getPointer(), this->getl());
	effect = 0;
}
Effect::EffectBuilder::EffectBuilder(const Data &data)
{
int l=data.getSize();
void *effect=data.getPointer();
	void * e = new char[l];
	memcpy(e, effect, l);
	this->setdata(e);
	this->setl(l);
	this->effect = 0;
}
Effect::EffectBuilder::EffectBuilder(const EffectBuilder & copy)
{
	delete[] this->getdata();
	this->setl(copy.getl());
	this->setdata(new char[this->getl()]);
	memcpy(this->getdata(), copy.getdata(), this->getl());
	effect = 0;
}
Effect::EffectBuilder::~EffectBuilder()
{
	delete[] this->getdata();
}
Effect Effect::EffectBuilder::build(DXMain & dx,const char *t)
{
	if (effect == nullptr)
	{
		effect = new DxEffect();
		*effect = dx.getDevice().createEffect(this->getdata(), this->getl());
	}
	std::vector<byte> dat;
	dat.resize(getl());
	memcpy(&dat[0], getdata(), getl());
	return Effect(std::move(dat), *effect, t);
}
const char * Effect::EffectBuilder::convertFsModelToString(FsModel model)
{
	static const char* const models[] =
	{
		"fx_2_0",
		"fx_3_0",
		"fx_4_0",
		"fx_4_1",
		"fx_5_0"
	};
	return models[(int)model];
}
Effect::Effect(std::vector<byte> &&data, DxEffect effect, const char *t) :Shader(std::move(data)), effect(effect)
{
	tSize = strlen(t) + 1;
	this->t = new char[tSize];
	strcpy(this->t, t);
	updateVariables();
	updateTechniques();
	this->effect->Optimize();
}
Effect::Effect(const Effect & copy) : Shader(copy), effect(copy.effect)
{
	copyMap(copy.variables, variables);
	copyMap(copy.techniques, techniques);
	tSize = strlen(copy.t) + 1;
	this->t = new char[tSize];
	strcpy(this->t, copy.t);
	this->geteffect()->AddRef();
}
Effect::~Effect()
{
	this->geteffect()->Release();
	for (auto var : variables)
	{
		delete[] var.first;
	}
	for (auto teq : techniques)
	{
		delete[] teq.first;
	}
	delete[] t;
}
void Effect::prepareForRender(const DXMain & dx) const
{
	findVar(this->gett(), techniques)->GetPassByIndex(0)->Apply(0, dx.getDx().getContext());
}
Shader * Effect::clone() const
{
	return new Effect(*this);
}
void Effect::setMatrixVar(const char * varName, const Matrix & matrix)
{
	findVar(varName,variables)->AsMatrix()->SetMatrix((float*)&matrix);
}
void Effect::setMatrixArrayVar(const char * varName, const Matrix * matrix,int startIndex, int numOfMatrix)
{
	resValid(findVar(varName, variables)->AsMatrix()->SetMatrixArray((float*)matrix, 0, numOfMatrix));
}
void Effect::setVectorVar(const char * varName, const Vector & vector)
{
	findVar(varName, variables)->AsVector()->SetFloatVector((float*)&vector);
}
void Effect::setBufferVar(const char * varName, const Buffer & buffer)
{
	findVar(varName, variables)->AsConstantBuffer()->SetConstantBuffer(buffer.getBufferType());
}
void Effect::setScalarVar(const char * varName, const float scalar)
{
	findVar(varName, variables)->AsScalar()->SetFloat(scalar);
}
void Effect::setShaderResourceVar(const char * varName, const ShaderResource & res)
{
	findVar(varName, variables)->AsShaderResource()->SetResource(res.getResView());
}
void Effect::setDataVar(const char* varName, const void *data, unsigned int count, unsigned int offset)
{
	findVar(varName, variables)->SetRawValue(data, offset, count);
}
void Effect::setTechnique(const char * tec)
{
	unsigned int nSize = strlen(tec) + 1;
	if (nSize>tSize)
	{
		delete[] t;
		t = new char[nSize];
		tSize = nSize;
	}
	strcpy(t, tec);
}
BYTE * Effect::getInputSignature(SIZE_T & size) const
{
	EffectPassDesc d;
	findVar(this->gett(), techniques)->GetPassByIndex(0)->GetDesc(&d);
	size = d.IAInputSignatureSize;
	return d.pIAInputSignature;
}
char * Effect::getName() const
{
	return "effect";
}
unsigned int Effect::getNumber() const
{
	return 7;
}
const byte * Effect::getTechData(unsigned int * size) const
{
	*size = strlen(t) + 1;
	return (byte*)t;
}
void Effect::updateVariables()
{
	D3DX11_EFFECT_DESC efDesc;
	D3DX11_EFFECT_VARIABLE_DESC varDesc;
	effect->GetDesc(&efDesc);
	int l;
	char *str;
	for (size_t i = 0; i < efDesc.GlobalVariables; i++)
	{
		auto var = effect->GetVariableByIndex(i);
		var->GetDesc(&varDesc);
		l = strlen(varDesc.Name) + 1;
		str = new char[l];
		strcpy(str, varDesc.Name);
		variables.insert(std::pair<char*, ID3DX11EffectVariable*>(str, var));
	}
}
void Effect::updateTechniques()
{
	D3DX11_EFFECT_DESC efDesc;
	D3DX11_TECHNIQUE_DESC varDesc;
	effect->GetDesc(&efDesc);
	int l;
	char *str;
	for (size_t i = 0; i < efDesc.Techniques; i++)
	{
		auto var = effect->GetTechniqueByIndex(i);
		var->GetDesc(&varDesc);
		l = strlen(varDesc.Name) + 1;
		str = new char[l];
		strcpy(str, varDesc.Name);
		techniques.insert(std::pair<char*, ID3DX11EffectTechnique*>(str, var));
	}
}
bool Effect::checkIsMe(const char *name) const
{
 return name[0] == 'e' || name[0] == 'E';
}
SAME_GET_SET_METHOD_IMPL(data, void*, Effect::EffectBuilder);
GET_SET_METHOD_IMPL(l, SIZE_T, Effect::EffectBuilder);
GET_SET_METHOD_IMPL(effect, DxEffect, Effect);
SAME_GET_SET_METHOD_IMPL(t, char*, Effect);