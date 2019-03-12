#include "PipeLineState.h"
#include <algorithm>
#include "Device.h"
#include "DXMain.h"
#include "GraphicState.h"
#include "Shader.h"
#include "Buffer.h"
#include "inputLayout.h"
#include "Shader.h"
#include"ShaderState.h"
int getStartNumAsInt(const int startInBool)
{
	return startInBool;
}
unsigned int createUpper32(unsigned int num)
{
	int ret = num / 32;
	int mud = num % 32;
	return (ret + (mud != 0)) * 32;
}
int getNumberOfBools(const int valSize)
{
	return valSize;
}
int getSizeForArray(const int valSize)
{
	return createUpper32(valSize);
}
unsigned int* createArray(unsigned int size, unsigned int *arr, unsigned int maxSize)
{
	if (size > maxSize)
	{
		return new unsigned int[size];
	}
	else
	{
		return arr;
	}
}
int addToArray(const int valSize)
{
	return valSize + 1;
}
BoolsArray::BoolsArray() :BoolsArray(64)
{
}
BoolsArray::BoolsArray(int startNum) : actcount(0), maxCount(getSizeForArray(startNum)), val(createArray(maxCount, tempVal, localSize))
{
	setAll<false>();
}
BoolsArray::~BoolsArray()
{
	if (maxCount>localSize)
	{
		delete[] val;
	}
}
void BoolsArray::push_back(bool val)
{
	resize(actcount + 1);
	setVal(actcount - 1, val);
}
const bool BoolsArray::operator[](int index)
{
	int posInArray = index / 32;
	int posInVal = index % 32;
	return (val[posInArray] & (1 << posInVal)) != 0;
}
void BoolsArray::setVal(int index, bool val)
{
	int posInArray = index / 32;
	int posInVal = index % 32;
	setVal(posInArray, posInVal, val);
}
template<bool val>
void BoolsArray::setAll()
{
	if (val)
	{
		memset(this->val, (int)(-1), sizeof(int)*maxCount / 32);
	}
	else
	{
		memset(this->val, 0, sizeof(int)*maxCount / 32);
	}
}
void BoolsArray::setRange(int start, int end,bool val)
{
	for (; start < end; start++)
	{
		setVal(start, val);
	}
}
void BoolsArray::clear()
{
	setAll<false>();
	actcount = 0;
}
unsigned int BoolsArray::size()
{
	return actcount;
}
void BoolsArray::resize(unsigned int newSize)
{
	if (newSize > maxCount)
	{
		unsigned tempSize = createUpper32(newSize);
		if (tempSize > localSize)
		{
			if (maxCount > localSize)
			{
				delete[] val;
			}
			val = new unsigned int[tempSize / 32];
		}
		maxCount = tempSize;
	}
	actcount = newSize;
}
void BoolsArray::resize(unsigned int newSize, bool val)
{
	unsigned int oldSize = actcount;
	if (newSize > maxCount)
	{
		unsigned tempSize = createUpper32(newSize);
		if (tempSize > localSize)
		{
			if (maxCount > localSize)
			{
				delete[] this->val;
			}
			this->val = new unsigned int[tempSize / 32];
		}
		maxCount = tempSize;
	}
	actcount = newSize;
	setRange(oldSize, newSize,val);
}
void BoolsArray::setVal(int posInArray, int posInVal, bool val)
{
	int finalPos = (1 << posInVal)&(val == true ? 1 : 0);
	this->val[posInArray] = this->val[posInArray] & ~(1 << posInVal) | (val << posInVal);
	//this->val[posInArray] = this->val[posInArray] ^ (this->val[posInArray] ^ finalPos);
}
template<class T> void addItemToArray(const T *item, const T **&arr, unsigned char &count, unsigned char &max)
{
    if (count == max)
    {
        const T **newArr;
        if (max > 255 / 2)
        {
            if (max == 255)
            {
                throw "Not implement";
            }
            max = 255;
        }
        else
        {
            max *= 2;
        }
        newArr = new T const*[max];
        memcpy(newArr, arr, count * sizeof(T*));
        delete[] arr;
        arr = newArr;
    }
    arr[count] = item;
    ++count;
}
template<class T> void removeNullFromArray(const T **&arr, unsigned char &count)
{
	const T **freePos = arr;
	const T **curPos = arr;
	const T **finalPos = arr + count;
	while (curPos < finalPos)
	{
		if (*curPos != nullptr)
		{
			*freePos = *curPos;
			++freePos;
		}
		++curPos;
	}
	count = freePos - arr;
}
PipeLineState::PipeLineState() :State(), vBuffer(nullptr), iBuffer(nullptr),
input(), topology((PrimitiveTopology)0), shaders(new Shader const*[3]), shaderMax(3), shaderCount(0), states(new GraphicState const*[3]), stateMax(3), stateCount(0), hasShaderChange(), hasStateChange(), needToResetState(false)
{
}
PipeLineState::~PipeLineState()
{
}
void PipeLineState::setGraphicState(const GraphicState * state)
{
	if (!this->getChange(WAS_JUST_RESET_STATE))
	{
        this->needToResetState = false;
		unsigned char i = 0;
		for (; i < stateCount; i++)
		{
			if (states[i] == state)
			{
				break;
			}
		}
		if (i == stateCount)
		{
			addItemToArray(state, states, stateCount, stateMax);
			hasStateChange.push_back(true);
		}
		else
		{
			if (states[i] != state)
			{
				states[i] = state;
				this->hasShaderChange.setVal(i, true);
			}
		}
	}
	else
	{
		this->clearChange(WAS_JUST_RESET_STATE);
	}
}
void PipeLineState::addShader(const Shader *shader)
{
	if (!this->getChange(WAS_JUST_RESET_SHADER))
	{
		unsigned char i = 0;
		for (; i < shaderCount; i++)
		{
			if (shaders[i]->getName()[0] == shader->getName()[0])
			{
				break;
			}
		}
		if (i==shaderCount)
		{
			addItemToArray(shader, shaders, shaderCount, shaderMax);
			hasShaderChange.push_back(true);
		}
		else if(*shader!=*shaders[i])
		{
			shaders[i] = shader;
			hasShaderChange.setVal(i, true);
		}
	}
}
void PipeLineState::setVertexBuffer(const Buffer * buffer)
{
	if (this->vBuffer->getResource() != buffer->getResource())
	{
		this->setChange(VERTEX_POS);
		this->vBuffer = buffer;
	}
}
void PipeLineState::setIndexBuffer(const Buffer * buffer)
{
	if (this->iBuffer->getResource() != buffer->getResource())
	{
		this->setChange(INDEX_POS);
		this->iBuffer = buffer;
	}
}
void PipeLineState::setInputLayout(const InputLayout input)
{
	if (this->getChange(WAS_JUST_RESET_SHADER))
	{
		this->clearChange(WAS_JUST_RESET_SHADER);
	}
	if (this->input.getLayout() != input.getLayout())
	{
		this->setChange(INPUT_POS);
		this->input = input;
	}
}
void PipeLineState::setPrimitiveTopology(PrimitiveTopology topology)
{
	if (this->topology != topology)
	{
		this->setChange(TOPOLOGY_POS);
		this->topology = topology;
	}
}
void PipeLineState::apply(const DXMain & dx)
{
	auto &device = dx.getDevice();
	for (size_t i = 0, j = hasShaderChange.size(); i < j; i++)
	{
		if (hasShaderChange[i])
		{
			shaders[i]->prepareForRender(dx);
			hasShaderChange.setVal(i, false);
		}
	}
	if (this->getChange(INPUT_POS))
	{
		device.setInputLayout(input.getLayout());
	}
	if (hasStateChange.size()==0)
	{
        if (needToResetState)
        {
            device.setRenderState(nullptr);
            device.setBlendState(nullptr, Color(0.0f, 0.0f, 0.0f));
        }
	}
	else
	{
		for (size_t i = 0, j = hasStateChange.size(); i < j; i++)
		{
			if (hasStateChange[i])
			{
				if (states[i] != nullptr)
				{
					hasStateChange.setVal(i, false);
					states[i]->apply(dx);
				}
			}
		}
	}
	if (this->getChange(TOPOLOGY_POS))
	{
		device.setPrimitiveTopology(this->topology);
	}
	if (this->getChange(VERTEX_POS))
	{
		unsigned int sizeOfBuffers;
		BufferType buffers;
		buffers = this->vBuffer->getBufferType();
		sizeOfBuffers = this->vBuffer->getSizeOfElement();
		Device::BufferData dat(&buffers, &sizeOfBuffers, 1);
		device.setVertexBuffer(0, dat, 0);
	}
	if (this->getChange(INDEX_POS))
	{
		if (this->iBuffer != nullptr)
		{
			device.setIndexBuffer(iBuffer->getBufferType(), 0, iBuffer->getSizeOfElement());
		}
	}
	this->clearChange();
}
bool PipeLineState::hasChanged() const
{
	return this->hasChange != 0;
}
void PipeLineState::reset(const GraphicState* state, const std::vector<Shader*> &shaders)
{
    this->setChange(WAS_JUST_RESET_SHADER);
    this->setChange(WAS_JUST_RESET_STATE);
    if (state == nullptr)
    {
        if (stateCount > 0)
        {
            needToResetState = true;
            stateCount = 0;
            this->hasStateChange.clear();
        }
    }
    else
    {
        bool wasAdded = false;
        for (unsigned char i = 0; i < stateCount; i++)
        {
            if (states[i] != state)
            {
                states[i] = nullptr;
            }
        }
        removeNullFromArray(states, stateCount);
        this->hasStateChange.clear();
        if (stateCount == 0)
        {
            addItemToArray(state, states, stateCount, stateMax);
            hasStateChange.push_back(true);
        }
        else
        {
            hasStateChange.push_back(false);
        }
    }
    if (shaders.size() == 0)
    {
        shaderCount = 0;
        this->hasShaderChange.clear();
    }
    else
    {
        BoolsArray boolsVal;
        boolsVal.resize(shaders.size());
        for (unsigned char i = 0; i < shaderCount; i++)
        {
            bool wasAdded = false;
            int index = 0;
            for (const auto &shad : shaders)
            {
                if (shad->getName()[0] == this->shaders[i]->getName()[0])
                {
                    if (*shad == *this->shaders[i])
                    {
                        boolsVal.setVal(index, true);
                        wasAdded = true;
                    }
                    break;
                }
                ++index;
            }
            if (!wasAdded)
            {
                this->shaders[i] = nullptr;
            }
        }
        removeNullFromArray(this->shaders, shaderCount);
        this->hasShaderChange.resize(shaderCount, false);
        auto &conShad = shaders.cbegin();
        {
            for (size_t i = 0; i < boolsVal.size(); i++)
            {
                auto wasAdd = boolsVal[i];
                if (!wasAdd)
                {
                    addItemToArray(*conShad, this->shaders, shaderCount, shaderMax);
                    this->hasShaderChange.push_back(true);
                }
                ++conShad;
            }
        }
    }
}