#include "PixelShader.h"
#include <iterator>
#include "DXMain.h"
#include "Device.h"
#include "Data.h"
#include "ShaderResource.h"
#include "Sampler.h"
#include "ShaderState.h"
PixelShader::PixelShader(std::vector<byte> &&dat,PixelShaderType shader) :Shader(std::move(dat)), pixelShader(shader)
{
#if CLDLEVEL >= 4
	static int number = 0;
	char name[] = "Pixel Shader:   ";
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
PixelShader::PixelShader(const PixelShader &copy) :Shader(copy), pixelShader(copy.pixelShader)
{
	this->pixelShader->AddRef();
}
PixelShader PixelShader::createPixelShader(const DXMain &dx,const Data &data)
{
	return PixelShader(Shader::createVectorFromData(data), dx.getDevice().createPixelShader(data.getPointer(), data.getSize()));
}
PixelShader PixelShader::createPixelShader(const DXMain &dx, const UTF8 *path, const char *startFunction, const PsModel shaderModel)
{
	PixelShader ret = PixelShader::createPixelShader(dx, Shader::compileShader(path, startFunction, convertPsModelToString(shaderModel)));
	return ret;
}
PixelShader::~PixelShader()
{
	this->pixelShader->Release();
}
PixelShader* PixelShader::clone() const
{
	return new PixelShader(*this);
}
bool PixelShader::operator==(const Shader &shad) const
{
    return getNumber() == shad.getNumber() && getPixelShaderType() == ((PixelShader&)shad).getPixelShaderType();
}
PixelShader &PixelShader::operator=(PixelShader& ass)
{
	if (this != &ass)
	{
		this->pixelShader->Release();
		this->pixelShader = ass.pixelShader;
		this->pixelShader->AddRef();
	}
	return *this;
}
void PixelShader::prepareForRender(const DXMain &dx) const
{
	auto &device = dx.getDevice();
	device.setPixelShader(this->getPixelShaderType());
}
PixelShaderType PixelShader::getPixelShaderType() const
{
	return this->pixelShader;
}
const char* PixelShader::convertPsModelToString(PsModel model)
{
	static const char* const models[] = 
	{ 
		"ps_2_0",
		"ps_3_0",
		"ps_4_0",
		"ps_4_1",
		"ps_5_0"
	};
	return models[(int)model];
}
bool PixelShader::checkIsMe(const char *name) const
{
 return name[0] == 'p' || name[0] == 'P';
}
char * PixelShader::getName() const
{
	return "pixel";
}

unsigned int PixelShader::getNumber() const
{
	return 5;
}
