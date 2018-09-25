#ifndef _PIPELINESTATE_H_
#define _PIPELINESTATE_H_
#include "State.h"
#include <vector>
#include <list>
#include <memory>
#include "Engine.h"
#include "inputLayout.h"
class GraphicState;
class Shader;
class Buffer;
class DXMain;
enum PrimitiveTopology;
class BoolsArray
{
public:
	BoolsArray();
	BoolsArray(int startNum);
	~BoolsArray();
	void push_back(bool val);
	const bool operator[](int index);
	void setVal(int index, bool val);
	void clear();
	unsigned int size();
	void resize(unsigned int newSize);
	void resize(unsigned int newSize,bool val);
	template<bool val> void setAll();
	void setRange(int start,int end,bool val);
private:
	void setVal(int posInArray, int posInVal, bool val);
	static constexpr int localSize = 4 * 32;
	unsigned int actcount;
	unsigned int maxCount;
	unsigned int *val;
	unsigned int tempVal[localSize];
};
class PipeLineState :public State//for engine
{
public:
	PipeLineState();
	~PipeLineState();
	void setGraphicState(const GraphicState *state);
	void addShader(const Shader* shader);
	void setVertexBuffer(const Buffer* buffer);
	void setIndexBuffer(const Buffer* buffer);
	void setInputLayout(const InputLayout input);
	void setPrimitiveTopology(PrimitiveTopology topology);
	void apply(const DXMain &dx) override;
	bool hasChanged() const override;
	void reset(const GraphicState* state, const std::vector<Shader*> &shaders);
private:
	const Shader** shaders;
	unsigned char shaderMax;
	unsigned char shaderCount;
	const GraphicState** states;
	unsigned char stateMax;
	unsigned char stateCount;
	BoolsArray hasShaderChange;
	BoolsArray hasStateChange;
	const Buffer* vBuffer;
	const Buffer* iBuffer;
	InputLayout input;
	PrimitiveTopology topology;
	static CONSTEXPRN int INPUT_POS = 4;
	static CONSTEXPRN int INDEX_POS = 3;
	static CONSTEXPRN int VERTEX_POS = 2;
	static CONSTEXPRN int TOPOLOGY_POS = 5;
	static CONSTEXPRN int WAS_JUST_RESET_STATE = 0;
	static CONSTEXPRN int WAS_JUST_RESET_SHADER = 1;
};
#endif