#ifndef _SAMPLER_H_
#define _SAMPLER_H_
#include "Engine.h"
#include "DXTypes.h"
enum TextureAddressMode
{
	WRAP = 1, MIRROR, CLAMP, BORDER, MIRROR_ONCE
};
enum Filter
{
	MIN_MAG_MIP_POINT = 0,
	MIN_MAG_POINT_MIP_LINEAR = 0x1,
	MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
	MIN_POINT_MAG_MIP_LINEAR = 0x5,
	MIN_LINEAR_MAG_MIP_POINT = 0x10,
	MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
	MIN_MAG_LINEAR_MIP_POINT = 0x14,
	MIN_MAG_MIP_LINEAR = 0x15,
	ANISOTROPIC = 0x55,
	COMPARISON_MIN_MAG_MIP_POINT = 0x80,
	COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
	COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
	COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
	COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
	COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
	COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
	COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
	COMPARISON_ANISOTROPIC = 0xd5
};
enum Comparison
{
	NEVER = 1,
	LESS = 2,
	EQUAL = 3,
	LESS_EQUAL = 4,
	GREATER = 5,
	NOT_EQUAL = 6,
	GREATER_EQUAL = 7,
	ALWAYS = 8
};
class DXMain;
class Sampler
{
public:
	//builder class for sampler
	class SamplerBuilder
	{
	public:
		SamplerBuilder();
		SamplerBuilder(TextureAddressMode u, TextureAddressMode v, TextureAddressMode w, Filter filter, Comparison comparison);
		Sampler build(const DXMain& dx) const;
	private:
		TextureAddressMode u, v, w;
		Filter filter;
		Comparison comparison;
		BUILDER_GET_SET_METHOD(u, TextureAddressMode, public, SamplerBuilder);
		BUILDER_GET_SET_METHOD(v, TextureAddressMode, public, SamplerBuilder);
		BUILDER_GET_SET_METHOD(w, TextureAddressMode, public, SamplerBuilder);
		BUILDER_GET_SET_METHOD(filter, Filter, public, SamplerBuilder);
		BUILDER_GET_SET_METHOD(comparison, Comparison, public, SamplerBuilder);
	};
public://public for everyone
	Sampler(const Sampler &copy);
	~Sampler();
	Sampler& operator=(const Sampler&);
public://public for engine
protected:
private:
	Sampler(SamplerStateType sampler);
	SamplerStateType samplerState;
	SET_METHOD(samplerState, SamplerStateType, private);
	//for engine
	GET_METHOD(samplerState, SamplerStateType, public);
};
#endif