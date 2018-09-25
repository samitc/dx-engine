#include "DrawCommand.h"
GET_METHOD_IMPL(pipe, const PipeState&, DrawCommand);
GET_METHOD_IMPL(topology, PrimitiveTopology, DrawCommand);
GET_METHOD_IMPL(layout, const InputLayout&, DrawCommand);
DrawCommand::DrawCommand(PrimitiveTopology topo, const PipeState &pipeState, const InputLayout &input) :topology(topo), pipe(pipeState), layout(input)
{
}

bool DrawCommand::operator==(const DrawCommand &dr) const
{
	return topology == dr.topology&&pipe == dr.pipe&&layout == dr.layout;
}
