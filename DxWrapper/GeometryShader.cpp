#include "GeometryShader.h"
#include "Device.h"
#include "DXMain.h"
#include "Data.h"
GeometryShader::GeometryShader(std::vector<byte> &&data, GeometryShaderType gs) :Shader(std::move(data)), geometryShader(gs)
{
}
GeometryShader GeometryShader::createGeometryShader(const DXMain & dx, const UTF8 * path, const char * startFunction, GsModel shaderModel)
{
	GeometryShader ret = createGeometryShader(dx, Shader::compileShader(path, startFunction, convertGsModelToString(shaderModel)));
	return ret;
}
GeometryShader GeometryShader::createGeometryShader(const DXMain & dx, const Data & data)
{
	return GeometryShader(Shader::createVectorFromData(data), dx.getDevice().createGeometryShader(data.getPointer(), data.getSize()));
}
const char * GeometryShader::convertGsModelToString(GsModel model)
{
	CONSTEXPRN char* models[]=
	{
		"gs_2_0",
		"gs_3_0",
		"gs_4_0",
		"gs_4_1",
		"gs_5_0"
	};
	return models[(int)model];
}
bool GeometryShader::checkIsMe(const char * name) const
{
	return name[0] == 'g' || name[0] == 'G';
}
char * GeometryShader::getName() const
{
	return "geometry";
}
unsigned int GeometryShader::getNumber() const
{
	return 3;
}
GeometryShader::GeometryShader(const GeometryShader &copy) :Shader(copy), geometryShader(copy.geometryShader)
{
	geometryShader->AddRef();
}
GeometryShader::~GeometryShader()
{
	geometryShader->Release();
}
GeometryShader * GeometryShader::clone() const
{
	return new GeometryShader(*this);
}
GeometryShader & GeometryShader::operator=(GeometryShader &copy)
{
	if (this != &copy)
	{
		this->geometryShader->Release();
		this->geometryShader = copy.geometryShader;
		this->geometryShader->AddRef();
	}
	return *this;
}
void GeometryShader::prepareForRender(const DXMain & dx) const
{
	dx.getDevice().setGeometryShader(geometryShader);
}