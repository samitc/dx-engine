#ifndef _DRAWCOMMAND_H_
#define _DRAWCOMMAND_H_
#include "Engine.h"
#include "PipeState.h"
#include "inputLayout.h"
enum PrimitiveTopology;
class DrawCommand
{
public:
	DrawCommand() = delete;
	DrawCommand(const DrawCommand&) = default;
	DrawCommand(DrawCommand&&) = default;
	DrawCommand(PrimitiveTopology, const PipeState&, const InputLayout&);
	~DrawCommand() = default;
	DrawCommand& operator=(DrawCommand&&) = default;
	bool operator==(const DrawCommand&) const;
private:
	PipeState pipe;
	InputLayout layout;
	PrimitiveTopology topology;
	GET_METHOD(pipe, const PipeState&, public);
	GET_METHOD(topology, PrimitiveTopology, public);
	GET_METHOD(layout, const InputLayout&, public);
};
#endif