#include "DomainShader.h"
#include "Device.h"
#include "DXMain.h"
#include "Data.h"
DomainShader::DomainShader(std::vector<byte> &&dat, DomainShaderType ds) :Shader(std::move(dat)), domainShader(ds)
{
}
DomainShader::DomainShader(const DomainShader &copy) : Shader(copy), domainShader(copy.domainShader)
{
	domainShader->AddRef();
}
DomainShader::~DomainShader()
{
	domainShader->Release();
}
DomainShader * DomainShader::clone() const
{
	return new DomainShader(*this);
}
void DomainShader::prepareForRender(const DXMain & dx) const
{
	dx.getDevice().setDomainShader(domainShader);
}
DomainShader DomainShader::createDomainShader(const DXMain & dx, const UTF8 * path, const char * startFunction, DsModel shaderModel)
{
	DomainShader ret = createDomainShader(dx, Shader::compileShader(path, startFunction, convertDsModelToString(shaderModel)));
	return ret;
}
DomainShader DomainShader::createDomainShader(const DXMain & dx, const Data & data)
{
	return DomainShader(Shader::createVectorFromData(data), dx.getDevice().createDomainShader(data.getPointer(), data.getSize()));
}
const char * DomainShader::convertDsModelToString(DsModel model)
{
	CONSTEXPRN char* models[] =
	{
		"ds_2_0",
		"ds_3_0",
		"ds_4_0",
		"ds_4_1",
		"ds_5_0"
	};
	return models[(int)model];
}
bool DomainShader::checkIsMe(const char * name) const
{
	return name[0] == 'd' || name[0] == 'D';
}
char * DomainShader::getName() const
{
	return "domain";
}

unsigned int DomainShader::getNumber() const
{
	return 2;
}
