#pragma once
#include "GraphicState.h"
#include <random>
class GraphicStateMock :public GraphicState
{
public:
	int num;
	GraphicStateMock() { num = rand(); }
	GraphicState *clone() const override { return new GraphicStateMock(*this); }
	void apply(const DXMain &dx) const override {}
	int getId() const override
	{
		return 0x7fffffff;
	}
};