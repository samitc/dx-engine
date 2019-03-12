#ifndef _COMPUTESHADER_H_
#define _COMPUTESHADER_H_
#include "Shader.h"
#include <vector>
#include <list>
class Buffer;
enum CsModel
{
	CS2_0,
	CS3_0,
	CS4_0,
	CS4_1,
	CS5_0,
};
class ComputeShader :public Shader
{
public:
    ComputeShader(const ComputeShader &copy);
    static ComputeShader createComputeShader(const DXMain &dx, const Data &data);
    static ComputeShader createComputeShader(const DXMain &dx, const UTF8 *path, const char *startFunction, CsModel shaderModel);
    virtual ~ComputeShader();
    virtual ComputeShader* clone() const;
    ComputeShader &operator=(ComputeShader& ass);
    void prepareForRender(const DXMain &dx) const;
    static const char* convertCsModelToString(CsModel model);
    virtual bool operator==(const Shader&) const;
public://for engine
    virtual bool checkIsMe(const char *name) const;
    ComputeShaderType getComputeShaderType() const;
    virtual char* getName() const override;
    unsigned int getNumber() const override;
    ComputeShader(std::vector<byte> &&, ComputeShaderType shader);
private:
    ComputeShaderType computeShader;
};
#endif