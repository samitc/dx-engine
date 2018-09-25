#ifndef _PIPESTATE_H_
#define _PIPESTATE_H_
#include "Engine.h"
#include <vector>
class Shader;
class GraphicState;
class DXMain;
class PipeState
{
public:
	class PipeStateBuilder
	{
	public:
		PipeStateBuilder() = default;
		~PipeStateBuilder();
		PipeStateBuilder(const PipeStateBuilder&);
		PipeStateBuilder& operator=(const PipeStateBuilder&);
		PipeStateBuilder& addShader(const Shader&);
		PipeStateBuilder& addState(const GraphicState&);
		PipeState build() const;
	private:
		std::vector<Shader*> shaders;
		std::vector<GraphicState*> states;
	};
	~PipeState();
	PipeState(const PipeState&);
	PipeState(PipeState&&);
	PipeState& operator=(PipeState&&);
	bool operator==(const PipeState&) const;
private:
	PipeState(const std::vector<Shader*> &shaders, const std::vector<GraphicState*> &states);
	std::vector<Shader*> shaders;
	std::vector<GraphicState*> states;
	GET_METHOD(shaders, std::vector<Shader*>, public);
	GET_METHOD(states, std::vector<GraphicState*>, public);
};
#endif