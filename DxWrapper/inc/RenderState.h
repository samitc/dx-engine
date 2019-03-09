#ifndef _RENDERSTATE_H_
#define _RENDERSTATE_H_
#include "GraphicState.h"
#include "Engine.h"
#include "DXTypes.h"
class DXMain;
//this class represent how the gpu will draw vertex
class RenderState:public GraphicState
{
public:
	//builder class for renderstate
	class RenderStateBuilder
	{
	public:
		enum FillMode
		{
			WIREFRAME = 2,
			SOLID = 3
		};
		enum CullMode
		{
			NONE = 1,
			FRONT = 2,
			BACK = 3
		};
		RenderState build(const DXMain &dx) const;
		RenderStateBuilder();
		RenderStateBuilder(FillMode fill);
		RenderStateBuilder(FillMode fill, CullMode cull);
		RenderStateBuilder(FillMode fill, CullMode cull, bool frontCounterClockwise);
		RenderStateBuilder(FillMode fill, CullMode cull, bool frontCounterClockwise, int depthBias, float depthBiasClamp = 0, float slopeScaledDepthBias = 0, bool depthClipEnable = true, bool scissorEnable = false, bool multisampleEnable = false, bool antialiasedLineEnable = false);
	private:
		float depthBiasClamp;
		float slopeScaledDepthBias;
		int depthBias;
		FillMode fill;
		CullMode cull;
		bool frontCounterClockwise;
		bool depthClipEnable;
		bool scissorEnable;
		bool multisampleEnable;
		bool antialiasedLineEnable;
		BUILDER_GET_SET_METHOD(fill, FillMode, public, RenderStateBuilder)
		BUILDER_GET_SET_METHOD(cull, CullMode, public, RenderStateBuilder)
		BUILDER_GET_SET_METHOD(frontCounterClockwise, bool, public, RenderStateBuilder)
		BUILDER_GET_SET_METHOD(depthBias, int, public, RenderStateBuilder)
		BUILDER_GET_SET_METHOD(depthBiasClamp, float, public, RenderStateBuilder)
		BUILDER_GET_SET_METHOD(slopeScaledDepthBias, float, public, RenderStateBuilder)
		BUILDER_GET_SET_METHOD(depthClipEnable, bool, public, RenderStateBuilder)
		BUILDER_GET_SET_METHOD(scissorEnable, bool, public, RenderStateBuilder)
		BUILDER_GET_SET_METHOD(multisampleEnable, bool, public, RenderStateBuilder)
		BUILDER_GET_SET_METHOD(antialiasedLineEnable, bool, public, RenderStateBuilder)
	};
public://public for everyone
	RenderState();
	RenderState(const RenderState &copy);
	virtual RenderState *clone() const;
	virtual void apply(const DXMain &dx) const;
	RenderState& operator=(RenderState& obj);
	virtual ~RenderState();
public://public for engine
	RenderStateType getRenderState() const;
	int getId() const override;
protected:
	void setRenderState(RenderStateType state);
private:
	explicit RenderState(RenderStateType renderState);
	RenderStateType renderState;
};
#endif