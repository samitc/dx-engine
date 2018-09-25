#ifndef _INPUTLAYOUT_H_
#define	_INPUTLAYOUT_H_
#include <list>
#include "Engine.h"
#include "DXTypes.h"
class LayoutElement;
class DXMain;
class Effect;
class Data;
enum VsModel;
//this class represent the shader expected input(the structer of the vertex)
class InputLayout
{
public:
	//builder for input layout
	class InputLayoutBuilder
	{
	public:
		InputLayoutBuilder();
		InputLayoutBuilder(const InputLayoutBuilder &builder);
		InputLayoutBuilder& addLayoutElement(const LayoutElement &layoutElement);
		//build input layout from already compiled shader
		InputLayout build(const DXMain & dx, const Data & shader) const;
		//build input layout from shader vertex shader parameters
		//parameters:
		//	dx:
		//	path:the path of the shader file
		//	startFunction:the name of the entry function in the shader
		//	shaderModel:the level of the shader
		InputLayout build(const DXMain &dx, const UTF8 *path, const char *startFunction, VsModel shaderModel) const;
		//build input layout from effect
		InputLayout build(const DXMain & dx, const Effect & effect) const;
	private:
		D3D11_INPUT_ELEMENT_DESC *getElements() const;
		std::list<LayoutElement> elements;
		GET_SET_METHOD(elements, std::list<LayoutElement>, private);
	};
public://public for everyone
	InputLayout(const InputLayout &copy);
	~InputLayout();
	InputLayout &operator=(const InputLayout &ass);
	InputLayout(InputLayout&&);
	InputLayout& operator=(InputLayout&&);
	bool operator==(const InputLayout&) const;
public://public for engine
	InputLayout();
	InputLayoutType getLayout() const;
private:
	InputLayout(InputLayoutType inputLayout);
	InputLayoutType inputLayout;
};
#endif