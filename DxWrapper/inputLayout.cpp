#include "inputLayout.h"
#include "LayoutElement.h"
#include "Device.h"
#include "DXMain.h"
#include "Shader.h"
#include "Data.h"
#include "VertexShader.h"
#include "Effect.h"
InputLayout::InputLayout(const InputLayout &copy) : inputLayout(copy.inputLayout)
{
	this->inputLayout->AddRef();
	}
InputLayout::~InputLayout()
{
	if (inputLayout!=nullptr)
	{
		inputLayout->Release();
	}
}
InputLayoutType InputLayout::getLayout() const
	{
	return this->inputLayout;
	}
InputLayout &InputLayout::operator=(const InputLayout &ass)
{
	if (this != &ass)
	{
		if (this->getLayout() !=nullptr)
		{
			this->getLayout()->Release();
		}
		this->inputLayout = ass.inputLayout;
		this->inputLayout->AddRef();
	}
	return *this;
}
InputLayout::InputLayout(InputLayout &&in) :inputLayout(in.inputLayout)
{
	in.inputLayout = nullptr;
}
InputLayout & InputLayout::operator=(InputLayout &&in)
{
	if (this != &in)
	{
		inputLayout = in.inputLayout;
		in.inputLayout = nullptr;
	}
	return *this;
}
bool InputLayout::operator==(const InputLayout &in) const
{
	return inputLayout == in.inputLayout;
}
InputLayout::InputLayoutBuilder::InputLayoutBuilder()
{
}
InputLayout::InputLayout() :inputLayout(nullptr)
{
}
InputLayout::InputLayout(InputLayoutType inputLayout) :inputLayout(inputLayout)
{
#if CLDLEVEL >= 4
	static int number = 0;
	char name[] = "Input layout:   ";
	char *p = name + strlen(name) - 1;
	int num = number++;
	while (num>0)
	{
		*p = (num % 10) + 48;
		num /= 10;
		p--;
	}
	inputLayout->SetPrivateData(WKPDID_D3DDebugObjectName, strlen(name), name);
#endif
}
InputLayout::InputLayoutBuilder::InputLayoutBuilder(const InputLayoutBuilder & builder) :elements(builder.elements)
	{
	}
InputLayout::InputLayoutBuilder& InputLayout::InputLayoutBuilder::addLayoutElement(const LayoutElement & layoutElement)
{
	this->elements.push_back(layoutElement);
	return *this;
}
D3D11_INPUT_ELEMENT_DESC * InputLayout::InputLayoutBuilder::getElements() const
{
	int size = this->elements.size();
	D3D11_INPUT_ELEMENT_DESC *elements = new D3D11_INPUT_ELEMENT_DESC[size];
	std::list<LayoutElement>::const_iterator s = this->elements.begin();
	for (int i = 0; i < size; i++)
	{
		elements[i].SemanticName = (*s).getSemanticName();
		elements[i].SemanticIndex = (*s).getIndex();
		elements[i].Format = (DXGI_FORMAT)(int)(*s).getFormat();
		elements[i].InputSlot = (*s).getBufferIndex();
		int offset = (*s).getOffset();
		if (offset==-1)
		{
			offset = D3D11_APPEND_ALIGNED_ELEMENT;
		}
		elements[i].AlignedByteOffset = offset;
		elements[i].InputSlotClass = (D3D11_INPUT_CLASSIFICATION)((int)(*s).getIsInstanceData());
		elements[i].InstanceDataStepRate = (*s).getIsInstanceData() ? (*s).getDrawPerInstance() : 0;
		s++;
	}
	return elements;
}
InputLayout InputLayout::InputLayoutBuilder::build(const DXMain & dx, const UTF8 * path, const char * startFunction, VsModel shaderModel) const
{
	InputLayout ret(this->build(dx, Shader::compileShader(path, startFunction, VertexShader::convertVsModelToString(shaderModel))));
	return ret;
}
InputLayout InputLayout::InputLayoutBuilder::build(const DXMain & dx, const Data & shader) const
{
	int size = this->elements.size();
	D3D11_INPUT_ELEMENT_DESC *elements = this->getElements();
	InputLayout ret(dx.getDevice().createInputLayout(elements, size, (BYTE*)shader.getPointer(), shader.getSize()));
	delete[] elements;
	return ret;
}
InputLayout InputLayout::InputLayoutBuilder::build(const DXMain & dx, const Effect & effect) const
{
	D3D11_INPUT_ELEMENT_DESC * elements = this->getElements();
	SIZE_T l;
	BYTE* data = effect.getInputSignature(l);
	InputLayout ret(dx.getDevice().createInputLayout(elements, this->getelements().size(), data, l));
	delete[] elements;
	return ret;
}
GET_SET_METHOD_IMPL(elements, std::list<LayoutElement>, InputLayout::InputLayoutBuilder);