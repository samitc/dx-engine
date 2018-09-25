#include "ComputeShader.h"
#include "Device.h"
#include "DXMain.h"
#include "ShaderState.h"
#include "Buffer.h"
#include "Data.h"
ComputeShader::ComputeShader(const ComputeShader & copy) :Shader(copy), computeShader(copy.computeShader)
{
	this->computeShader->AddRef();
}
ComputeShader ComputeShader::createComputeShader(const DXMain & dx, const Data & data)
{
	return ComputeShader(Shader::createVectorFromData(data), dx.getDevice().createComputeShader(data.getPointer(), data.getSize()));
}
ComputeShader ComputeShader::createComputeShader(const DXMain & dx, const UTF8 * path, const char * startFunction, CsModel shaderModel)
{
	ComputeShader ret = ComputeShader::createComputeShader(dx, Shader::compileShader(path, startFunction, ComputeShader::convertCsModelToString(shaderModel)));
	return ret;
}
ComputeShader::~ComputeShader()
{
	this->computeShader->Release();
}
ComputeShader * ComputeShader::clone() const
{
	return new ComputeShader(*this);
}
ComputeShader & ComputeShader::operator=(ComputeShader & ass)
{
	if (this != &ass)
	{
		this->computeShader->Release();
		this->computeShader = ass.computeShader;
		this->computeShader->AddRef();
	}
	return *this;
}
void ComputeShader::prepareForRender(const DXMain & dx) const
{
#ifdef RELEASEC0
	if (this->getComputeShaderType() != NULL)
	{
#endif
		auto &device = dx.getDevice();
		device.setComputeShader(this->getComputeShaderType());
#ifdef RELEASEC0
	}
#endif
}
const char * ComputeShader::convertCsModelToString(CsModel model)
{
	static const char* const models[] =
	{
		"cs_2_0",
		"cs_3_0",
		"cs_4_0",
		"cs_4_1",
		"cs_5_0"
	};
	return models[(int)model];
}
bool ComputeShader::checkIsMe(const char * name) const
{
	return name[0] == 'c' || name[0] == 'C';
}
ComputeShaderType ComputeShader::getComputeShaderType() const
{
	return computeShader;
}
char * ComputeShader::getName() const
{
	return "compute";
}
unsigned int ComputeShader::getNumber() const
{
	return 6;
}
ComputeShader::ComputeShader(std::vector<byte> &&data, ComputeShaderType shader) :Shader(std::move(data)), computeShader(shader)
{
#ifdef DEBUGC
	static int number = 0;
	char name[] = "Vertex shader:   ";
	char *p = name + strlen(name) - 1;
	int num = number++;
	while (num > 0)
	{
		*p = (num % 10) + 48;
		num /= 10;
		p--;
	}
	shader->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
#endif
}