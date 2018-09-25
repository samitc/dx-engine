#include "PipeState.h"
#include "Shader.h"
#include "GraphicState.h"
GET_METHOD_IMPL(shaders, std::vector<Shader*>, PipeState);
GET_METHOD_IMPL(states, std::vector<GraphicState*>, PipeState);
template <class T> void copyVector(std::vector<T> &vecT,const std::vector<T> &vecF)
{
	vecT.reserve(vecF.size());
	for (const T& val : vecF)
	{
		vecT.push_back(val->clone());
	}
}
template <class T> void deleteVector(const std::vector<T> &vec)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		delete vec[i];
	}
}
PipeState::PipeStateBuilder::~PipeStateBuilder()
{
	deleteVector(states);
	deleteVector(shaders);
}
PipeState::PipeStateBuilder::PipeStateBuilder(const PipeStateBuilder &pipe)
{
	copyVector(shaders, pipe.shaders);
	copyVector(states, pipe.states);
}
PipeState::PipeStateBuilder & PipeState::PipeStateBuilder::operator=(const PipeStateBuilder &pipe)
{
	if (this!=&pipe)
	{
		copyVector(shaders, pipe.shaders);
		copyVector(states, pipe.states);
	}
	return *this;
}
PipeState::PipeStateBuilder& PipeState::PipeStateBuilder::addShader(const Shader &s)
{
	shaders.push_back(s.clone());
	return *this;
}
PipeState::PipeStateBuilder& PipeState::PipeStateBuilder::addState(const GraphicState &g)
{
	states.push_back(g.clone());
	return *this;
}
PipeState PipeState::PipeStateBuilder::build() const
{
	return PipeState(shaders, states);
}
PipeState::~PipeState()
{
	deleteVector(states);
	deleteVector(shaders);
}
PipeState::PipeState(const PipeState &pipe) :PipeState(pipe.shaders, pipe.states)
{
}
PipeState::PipeState(PipeState &&pipe) : states(std::move(pipe.states)), shaders(std::move(pipe.shaders))
{
}
PipeState & PipeState::operator=(PipeState &&pipe)
{
	if (this!=&pipe)
	{
		states = std::move(states);
		shaders = std::move(shaders);
	}
	return *this;
}
bool PipeState::operator==(const PipeState &pipe) const
{
	return states == pipe.states&&shaders == pipe.shaders;
}
PipeState::PipeState(const std::vector<Shader*>& shaders, const std::vector<GraphicState*>& states) :shaders(), states()
{
	copyVector(this->states, states);
	copyVector(this->shaders, shaders);
}