#ifndef _GEOMETRYSTREAMOUTPUTSHADER_H_
#define _GEOMETRYSTREAMOUTPUTSHADER_H_
#include "GeometryShader.h"
#include <list>
#include "Buffer.h"
#include "Data.h"
class DXMain;
class GeometryStreamOutputShader:public GeometryShader
{
public:
    class GeometryStreamOutputShaderBuilder
    {
    public:
        struct OutputEntry
        {
            unsigned int stream;
            char *semanticName;
            unsigned int semanticIndex;
            unsigned char startComponent;
            unsigned char componentCount;
            unsigned char outputSlot;
        };
        GeometryStreamOutputShaderBuilder() = default;
        ~GeometryStreamOutputShaderBuilder() = default;
        GeometryStreamOutputShaderBuilder& setShader(const DXMain &dx, const UTF8 *path, const char *startFunction, GsModel shaderModel);
        GeometryStreamOutputShaderBuilder& setShader(const DXMain &dx, const Data &data);
        GeometryStreamOutputShaderBuilder& addOutputEntry(const OutputEntry&);
        GeometryStreamOutputShader build(const DXMain& dx) const;
    private:
        Data shader;
        std::list<OutputEntry> entries;
    };
	GeometryStreamOutputShader()=delete;
	~GeometryStreamOutputShader()=default;
	GeometryStreamOutputShader* clone() const override;
	void prepareForRender(const DXMain &dx) const override;
	bool checkIsMe(const char *name) const override;
	char* getName() const override;
	unsigned int getNumber() const override;
	void setOutputBuffer(const Buffer& buffer);
	GeometryStreamOutputShader(std::vector<byte> &&, GeometryShaderType);
	const byte* getSoData(unsigned int *size) const;
    virtual bool operator==(const Shader&) const;
private:
	GeometryStreamOutputShader(std::vector<byte> &&, GeometryShaderType, std::vector<D3D11_SO_DECLARATION_ENTRY>&&);
	std::vector<D3D11_SO_DECLARATION_ENTRY> dec;
	Buffer outputBuffer;
};
#endif