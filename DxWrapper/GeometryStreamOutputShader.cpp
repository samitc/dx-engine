#include "GeometryStreamOutputShader.h"
#include "Device.h"
#include "DXMain.h"
GeometryStreamOutputShader::GeometryStreamOutputShaderBuilder & GeometryStreamOutputShader::GeometryStreamOutputShaderBuilder::setShader(const DXMain & dx, const UTF8 * path, const char * startFunction, GsModel shaderModel)
{
	this->shader = Shader::compileShader(path, startFunction, GeometryShader::convertGsModelToString(shaderModel));
	return *this;
}
GeometryStreamOutputShader::GeometryStreamOutputShaderBuilder & GeometryStreamOutputShader::GeometryStreamOutputShaderBuilder::setShader(const DXMain & dx, const Data & data)
{
	this->shader = data;
	return *this;
}
GeometryStreamOutputShader::GeometryStreamOutputShaderBuilder & GeometryStreamOutputShader::GeometryStreamOutputShaderBuilder::addOutputEntry(const OutputEntry &entry)
{
	entries.push_back(entry);
	return *this;
}
GeometryStreamOutputShader GeometryStreamOutputShader::GeometryStreamOutputShaderBuilder::build(const DXMain & dx) const
{
	unsigned int count = entries.size();
	std::vector<D3D11_SO_DECLARATION_ENTRY> ent{ count };
	int i = 0;
	for (const auto& outputEntry : entries)
	{
		memcpy(&ent[i], &outputEntry, sizeof(D3D11_SO_DECLARATION_ENTRY));
	}
	return GeometryStreamOutputShader(Shader::createVectorFromData(this->shader), dx.getDevice().createGeometryStreamOutputShader(this->shader.getPointer(), this->shader.getSize(), &ent[0], count), std::move(ent));
}
GeometryStreamOutputShader * GeometryStreamOutputShader::clone() const
{
	return new GeometryStreamOutputShader(*this);
}
void GeometryStreamOutputShader::prepareForRender(const DXMain & dx) const
{
	GeometryShader::prepareForRender(dx);
	BufferType buf = outputBuffer.getBufferType();
	dx.getDevice().soSetBuffer(1, &buf);
}
bool GeometryStreamOutputShader::checkIsMe(const char * name) const
{
	return name[0] == 's' || name[0] == 'S';
}
char * GeometryStreamOutputShader::getName() const
{
	return "streamOutput";
}
unsigned int GeometryStreamOutputShader::getNumber() const
{
	return 4;
}
void GeometryStreamOutputShader::setOutputBuffer(const Buffer & buffer)
{
	outputBuffer = buffer;
}
GeometryStreamOutputShader::GeometryStreamOutputShader(std::vector<byte> &&dat, GeometryShaderType shader) :GeometryShader(std::move(dat), shader)
{
}
const byte * GeometryStreamOutputShader::getSoData(unsigned int *size) const
{
	*size = dec.size()*sizeof(D3D11_SO_DECLARATION_ENTRY);
	return (const byte*)&dec[0];
}
GeometryStreamOutputShader::GeometryStreamOutputShader(std::vector<byte>&&dat, GeometryShaderType s, std::vector<D3D11_SO_DECLARATION_ENTRY>&&dec) : GeometryStreamOutputShader(std::move(dat), s)
{
	dec = std::move(dec);
}