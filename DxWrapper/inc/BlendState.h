#ifndef _BLENDSTATE_H_
#define _BLENDSTATE_H_
#include "GraphicState.h"
#include <memory>
#include <list>
#include <vector>
#include "Engine.h"
#include "DXTypes.h"
#include "Color.h"
enum Blend
{
	ZERO = 1,
	ONE = 2,
	SRC_COLOR = 3,
	INV_SRC_COLOR = 4,
	SRC_ALPHA = 5,
	INV_SRC_ALPHA = 6,
	DEST_ALPHA = 7,
	INV_DEST_ALPHA = 8,
	DEST_COLOR = 9,
	INV_DEST_COLOR = 10,
	SRC_ALPHA_SAT = 11,
	BLEND_FACTOR = 14,
	INV_BLEND_FACTOR = 15,
	SRC1_COLOR = 16,
	INV_SRC1_COLOR = 17,
	SRC1_ALPHA = 18,
	INV_SRC1_ALPHA = 19
};
enum BlendOp
{
	OP_ADD = 1,
	OP_SUBTRACT = 2,
	OP_REV_SUBTRACT = 3,
	OP_MIN = 4,
	OP_MAX = 5
};
enum ColorWrite
{
	ENABLE_RED = 1,
	ENABLE_GREEN = 2,
	ENABLE_BLUE = 4,
	ENABLE_ALPHA = 8,
	ENABLE_ALL = ENABLE_RED | ENABLE_GREEN | ENABLE_BLUE | ENABLE_ALPHA
};
//this class represent blending state for the hardware
class BlendState :public GraphicState
{
public:
	//builder class for blend state
	class BlendStateBuilder
	{
	public:
		//struct for spesific blend state target
		struct BlendStateTarget
		{
			bool enable = false;
			Blend srcBlend = Blend::ONE;
			Blend destBlend = Blend::ZERO;
			BlendOp blendOp = BlendOp::OP_ADD;
			Blend srcBlendAlpha = Blend::ONE;
			Blend destBlendAlpha = Blend::ZERO;
			BlendOp blendOpAlpha = BlendOp::OP_ADD;
			UINT8 renderTargetWriteMask = ColorWrite::ENABLE_ALL;
			BlendStateTarget();
			BlendStateTarget(const BlendStateTarget& copy);
			//for engine
			D3D11_RENDER_TARGET_BLEND_DESC getStateTarget() const;
		};
		//create the blend state
		BlendState build(DXMain &dx) const;
		BlendStateBuilder();
		BlendStateBuilder(bool alphaToCoverageEnable, bool independentBlendEnable, Color &col);
		BlendStateBuilder(bool alphaToCoverageEnable = false, bool independentBlendEnable = false, int numOfBlendStateTarget = 0, const BlendStateTarget* const blendState = NULL, Color &col = Color((unsigned char)255, 255, 255));
		BlendStateBuilder(bool alphaToCoverageEnable, bool independentBlendEnable, std::vector<BlendStateTarget> blendState, Color &col);
		BlendStateBuilder(const BlendStateBuilder &copy);
		~BlendStateBuilder();
		//add another blend state(up to 8 states)
		//parameters:
		//	blendStateTarget: the blend state data
		BlendStateBuilder& addBlendStateTarget(const BlendStateTarget &blendStateTarget);
		Color getcolor() const;
		//set the color for the blend state factor
		BlendStateBuilder& setcolor(const Color& col);
		GET_METHOD(alphaToCoverageEnable, bool, public);
		BUILDER_SET_METHOD(alphaToCoverageEnable, bool, public, BlendStateBuilder);
		GET_METHOD(independentBlendEnable, bool, public);
		BUILDER_SET_METHOD(independentBlendEnable, bool, public, BlendStateBuilder);
		BlendStateBuilder& operator=(BlendStateBuilder& obj);
	private:
		std::list<BlendStateTarget> blendState;
		Color color;
		bool alphaToCoverageEnable;
		bool independentBlendEnable;
		const std::list<BlendStateTarget> &getBlendState() const;
		std::list<BlendStateTarget> &getblendState();
	};
public://public for everyone
	BlendState();
	BlendState(const BlendState &copy);
	virtual ~BlendState();
	virtual BlendState *clone() const;
	virtual void apply(const DXMain &dx) const;
		//set the color for the blend state factor
	void setColor(const Color &color);
	Color getColor() const;
	BlendState& operator=(BlendState& obj);
public://public for engine
	BlendStateType getBlendState() const;
	int getId() const override;
protected://for engine
	void setBlendState(const BlendStateType state);
private:
	BlendState(const BlendStateType state,const Color &color);
	std::unique_ptr<Color> color;
	BlendStateType blendType;
};
#endif