#include "HullShader.h"
#include "Device.h"
#include "DXMain.h"
#include "Data.h"
HullShader::HullShader(const HullShader &copy) :Shader(copy), hullShader(copy.hullShader)
{
	hullShader->AddRef();
}
HullShader::~HullShader()
{
	hullShader->Release();
}
HullShader * HullShader::clone() const
{
	return new HullShader(*this);
}
void HullShader::prepareForRender(const DXMain & dx) const
{
	dx.getDevice().setHullShader(hullShader);
}
HullShader HullShader::createHullShader(const DXMain & dx, const UTF8 * path, const char * startFunction, HsModel shaderModel)
{
	HullShader ret = createHullShader(dx, Shader::compileShader(path, startFunction, convertHsModelToString(shaderModel)));
	return ret;
}
HullShader HullShader::createHullShader(const DXMain & dx, const Data & data)
{
	return HullShader(Shader::createVectorFromData(data), dx.getDevice().createHullShader(data.getPointer(), data.getSize()));
}
const char * HullShader::convertHsModelToString(HsModel model)
{
	CONSTEXPRN char* models[] =
	{
		"hs_2_0",
		"hs_3_0",
		"hs_4_0",
		"hs_4_1",
		"hs_5_0"
	};
	return models[(int)model];
}
bool HullShader::checkIsMe(const char * name) const
{
	return name[0] == 'h' || name[0] == 'H';
}
char * HullShader::getName() const
{
	return "hull";
}
unsigned int HullShader::getNumber() const
{
	return 1;
}
HullShader::HullShader(std::vector<byte> &&dat, HullShaderType hs) :Shader(std::move(dat)), hullShader(hs)
{
}