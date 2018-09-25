#include "Shader.h"
#include <D3Dcompiler.h>
#include "Data.h"
#include "ShaderState.h"
#include "DxResException.h"
#include "DataManager.h"
#include "BaseLog.h"
#include "DXMain.h"
#include "Device.h"
#include "VertexShader.h"
#include "HullShader.h"
#include "DomainShader.h"
#include "GeometryShader.h"
#include "GeometryStreamOutputShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "Effect.h"
using SHADER_CHOSER = unsigned int;
namespace
{
	void loadSize(const std::vector<byte>& dat, SHADER_CHOSER* sSize, SHADER_CHOSER* dataSize)
	{
		*sSize = *((SHADER_CHOSER*)(&dat[0]));
		*dataSize = *((SHADER_CHOSER*)(&dat[0] + *sSize + sizeof(SHADER_CHOSER)));
	}
	template <class T> void loadData(std::vector<byte>& dat, T* data, SHADER_CHOSER sSize, SHADER_CHOSER dataSize)
	{
		memcpy(data, &dat[0] + sSize + sizeof(SHADER_CHOSER) * 2, dataSize);
		dat.erase(dat.cbegin() + sSize + sizeof(SHADER_CHOSER), dat.cend());
		dat.erase(dat.cbegin(), dat.cbegin() + sizeof(SHADER_CHOSER));
	}
	Shader *createVertexShader(const Device& device, std::vector<byte> &&dat)
	{
		return new VertexShader(std::move(dat), device.createVertexShader(&dat[0], dat.size()));
	}
	Shader *createHullShader(const Device& device, std::vector<byte> &&dat)
	{
		return new HullShader(std::move(dat), device.createHullShader(&dat[0], dat.size()));
	}
	Shader *createDomainShader(const Device& device, std::vector<byte> &&dat)
	{
		return new DomainShader(std::move(dat), device.createDomainShader(&dat[0], dat.size()));
	}
	Shader *createGeometryShader(const Device& device, std::vector<byte> &&dat)
	{
		return new GeometryShader(std::move(dat), device.createGeometryShader(&dat[0], dat.size()));
	}
	Shader *createGeometryShaderWithSO(const Device& device, std::vector<byte> &&dat)
	{
		SHADER_CHOSER sSize;
		SHADER_CHOSER soSize;
		loadSize(dat, &sSize, &soSize);
		D3D11_SO_DECLARATION_ENTRY * dec = (D3D11_SO_DECLARATION_ENTRY*)alloca(soSize);
		loadData(dat, dec, sSize, soSize);
		return new GeometryStreamOutputShader(std::move(dat), device.createGeometryStreamOutputShader(&dat[0], dat.size(), dec, soSize / sizeof(D3D11_SO_DECLARATION_ENTRY)));
	}
	Shader *createPixelShader(const Device& device, std::vector<byte>&&dat)
	{
		return new PixelShader(std::move(dat), device.createPixelShader(&dat[0], dat.size()));
	}
	Shader *createComputeShader(const Device& device, std::vector<byte> &&dat)
	{
		return new ComputeShader(std::move(dat), device.createComputeShader(&dat[0], dat.size()));
	}
	Shader *createEffect(const Device& device, std::vector<byte> &&dat)
	{
		SHADER_CHOSER sSize;
		SHADER_CHOSER soSize;
		loadSize(dat, &sSize, &soSize);
		char * dec = (char*)alloca(soSize);
		loadData(dat, dec, sSize, soSize);
		return new Effect(std::move(dat), device.createEffect(&dat[0], dat.size()), dec);
	}
	typedef Shader* (*createShader)(const Device& device, std::vector<byte>&&);
	createShader shadersCreator[] = { createVertexShader, createHullShader,createDomainShader,createGeometryShader,createGeometryShaderWithSO,createPixelShader,createComputeShader,createEffect };
}
Shader::~Shader()
{
}
Data Shader::compileShader(const UTF8 *path, const char *startFunction, const char *shaderModel)
{
	ID3DBlob* errorBuffer = 0, *shader;
	HRESULT res = D3DCompileFromFile(path, nullptr, nullptr, startFunction, shaderModel, D3DCOMPILE_DEBUG, 0, &shader, &errorBuffer);
	if (FAILED(res))
	{
		std::string errorStr("error compile shader:");
		int size = wcslen(path) + 1;
		char *t = (char*)_alloca(size);
		wcstombs(t, path, size);
		errorStr.append(t);
		errorStr.append(";");
        if (res == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
        {
            errorStr.append("the shader can not be found.");
        }
		else
		{
			errorStr.append((char*)errorBuffer->GetBufferPointer());
			errorBuffer->Release();
		}
		throw DxResException(false, __LINE__, __FILE__, errorStr.c_str(), res);
	}
	if (errorBuffer != 0)
	{
		std::string warnLog("warning while compile shader:");
		int size = wcslen(path) + 1;
		char *t = (char*)_alloca(size);
		wcstombs(t, path, size);
		warnLog.append(t);
		warnLog.append(",start function name:");
		warnLog.append(startFunction);
		warnLog.append(",the warning is:");
		warnLog.append((char*)errorBuffer->GetBufferPointer());
		BaseLog::getInstance().warn(warnLog.c_str());
		errorBuffer->Release();
	}
	return Data(shader);
}
void Shader::saveShader(const DataManager & manager, const DXMain & dx, const char *id) const
{
	manager.saveStatic<Shader>(id, this);
}
Shader * Shader::loadShader(const DataManager & manager, const DXMain & dx, const char *id)
{
	return manager.loadStaticParams<Shader, const Device>(id, &dx.getDevice());
}
template <class T> void combineDat(void *dst, const void *src, int *dstS, unsigned int srcS)
{
	if (src!=nullptr)
	{
		*((T*)((char*)dst + *dstS)) = srcS;
		memcpy((char*)dst + *dstS + sizeof(T), src, srcS - sizeof(T));
		*dstS += srcS;
	}
}
byte * Shader::save(const Shader *shader, int *size)
{
	int ssize = shader->d.size();
	*size = ssize + sizeof(SHADER_CHOSER) * 2;
	void *data;
	const void *aData = nullptr;
	unsigned int aSize
#ifdef _DEBUG
		= 0
#endif
		;
	switch (shader->getNumber())
	{
	case 4://createGeometryShaderWithSO
		aData = ((GeometryStreamOutputShader*)shader)->getSoData(&aSize);
		aSize += sizeof(SHADER_CHOSER);
		data = new byte[*size + aSize];
		break;
	case 7://effect
		aData = ((Effect*)shader)->getTechData(&aSize);
		aSize += sizeof(SHADER_CHOSER);
		data = new byte[*size + aSize];
		break;
	default:
		data = new byte[*size];
		break;
	}
	*((SHADER_CHOSER*)data) = (SHADER_CHOSER)shader->getNumber();
	*(((SHADER_CHOSER*)data) + 1) = ssize;
	memcpy((char*)data + sizeof(SHADER_CHOSER) * 2, &shader->d[0], ssize);
	combineDat<SHADER_CHOSER>(data, aData, size, aSize);
	return (byte*)data;
}
Shader * Shader::load(const byte *data, int size,const Device *device)
{
	SHADER_CHOSER shaderType = *((SHADER_CHOSER*)data);
	data += sizeof(SHADER_CHOSER);
	SHADER_CHOSER ssize = *((SHADER_CHOSER*)data);
	std::vector<byte> bytes;
	switch (shaderType)
	{
	case 4://createGeometryShaderWithSO
	case 7://effect
		bytes.resize(size - sizeof(SHADER_CHOSER));
		memcpy(&bytes[0], data, size - sizeof(SHADER_CHOSER));
		break;
	default:
		bytes.resize(ssize);
		memcpy(&bytes[0], data + sizeof(SHADER_CHOSER), ssize);
		break;
	}
	return shadersCreator[shaderType](*device, std::move(bytes));
}
std::vector<byte> Shader::createVectorFromData(const Data & dat)
{
	std::vector<byte> ret;
	ret.resize(dat.getSize());
	memcpy(&ret[0], dat.getPointer(), dat.getSize());
	return ret;
}
Shader::Shader(std::vector<byte> && data) : d(std::move(data))
{
}
Shader::Shader()
{
}
Shader::Shader(const Shader & shader) :  d(shader.d)
{
}