#include "VertexShader.h"
#include <iterator>
#include "DXMain.h"
#include "Device.h"
#include "Data.h"
#include "Buffer.h"
#include "ShaderState.h"
VertexShader::VertexShader(std::vector<byte>&& dat, VertexShaderType shader) :Shader(std::move(dat)), vertexShader(shader)
{
#if CLDLEVEL >= 4
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
VertexShader::VertexShader(const VertexShader &copy) :Shader(copy), vertexShader(copy.vertexShader)
{
	this->vertexShader->AddRef();
}
VertexShader VertexShader::createVertexShader(const DXMain &dx, const Data &data)
{
	return VertexShader(Shader::createVectorFromData(data), dx.getDevice().createVertexShader(data.getPointer(), data.getSize()));
}
VertexShader VertexShader::createVertexShader(const DXMain &dx, const UTF8 *path, const char *startFunction, VsModel shaderModel)
{
	VertexShader ret = VertexShader::createVertexShader(dx, Shader::compileShader(path, startFunction, VertexShader::convertVsModelToString(shaderModel)));
	return ret;
}
VertexShader::~VertexShader()
{
	this->vertexShader->Release();
}
VertexShader* VertexShader::clone() const
{
	return new VertexShader(*this);
}
VertexShader &VertexShader::operator=(VertexShader& ass)
{
	if (this != &ass)
	{
		this->vertexShader->Release();
		this->vertexShader = ass.vertexShader;
		this->vertexShader->AddRef();
	}
	return *this;
}
void VertexShader::prepareForRender(const DXMain &dx) const
{
#if CLDLEVEL >= 0
		if (this->getVertexShaderType() != NULL)
		{
#endif
			auto &device = dx.getDevice();
			device.setVertexShader(this->getVertexShaderType());
#if CLDLEVEL >= 0
		}
#endif
}
VertexShaderType VertexShader::getVertexShaderType() const
{
	return this->vertexShader;
}
char * VertexShader::getName() const
{
	return "vertex";
}
unsigned int VertexShader::getNumber() const
{
	return 0;
}
const char * VertexShader::convertVsModelToString(VsModel model)
{
	static const char* const models[] =
	{
		"vs_2_0",
		"vs_3_0",
		"vs_4_0",
		"vs_4_1",
		"vs_5_0"
	};
	return models[(int)model];
}
bool VertexShader::checkIsMe(const char *name) const
{
 return name[0] == 'v' || name[0] == 'V';
}