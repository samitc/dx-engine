#ifndef _MATHB_H_
#define _MATHB_H_
#include "Engine.h"
#include <limits>
#include <DirectXCollision.h>
#include "Vector.h"
#include "Matrix.h"
struct Float2S
{
	union
	{
		struct
		{
			float x;
			float y;
		};
		struct
		{
			float a;
			float b;
		};
		struct
		{
			float u;
			float v;
		};
		struct
		{
			float c;
			float x;
		};
	};
};
struct Float3S
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};
		struct
		{
			float a;
			float b;
			float c;
		};
	};
	Float3S operator+(Float3S &o) const
	{
		Float3S ret = *this;
		ret.x += o.x;
		ret.y += o.y;
		ret.z += o.z;
		return ret;
	}
	Float3S operator-(Float3S &o) const
	{
		Float3S ret = *this;
		ret.x -= o.x;
		ret.y -= o.y;
		ret.z -= o.z;
		return ret;
	}
	Float3S operator*(float m) const
	{
		Float3S ret = *this;
		ret.x *= m;
		ret.y *= m;
		ret.z *= m;
		return ret;
	}
};
class FrustMatrix;
bool intersectBox(Float3S &objectCenter, Float3S &objectExtend, FrustMatrix &frustMatrix, Matrix &viewMatrixToObjectSpace);
class FrustMatrix
{
private:
	DirectX::BoundingFrustum projFrust;
public:
	static FrustMatrix createFrustMatrix(Matrix &projMat)
	{
		FrustMatrix ret;
        DirectX::BoundingFrustum::CreateFromMatrix(ret.projFrust, projMat.getMatrix());
		return ret;
	}
	friend bool intersectBox(Float3S &objectCenter, Float3S &objectExtend, FrustMatrix &frustMatrix, Matrix &viewMatrixToObjectSpace);
};
#define maxVar(res,a,b)	(res)=(a)>(b)?(a):(b);
#define minVar(res,a,b)	(res)=(a)>(b)?(b):(a);
void max3(Float3S &a, const Float3S &b)
{
	maxVar(a.x, a.x, b.x);
	maxVar(a.y, a.y, b.y);
	maxVar(a.z, a.z, b.z);
}
void min3(Float3S &a, const Float3S &b)
{
	minVar(a.x, a.x, b.x);
	minVar(a.y, a.y, b.y);
	minVar(a.z, a.z, b.z);
}
template <class T> void computeBoundingBox(Float3S &center, Float3S &extend, T* points, unsigned int lenthToNextPoint, unsigned int pointsCount)
{
	Float3S max;
	Float3S min;
	max.z = max.y = max.x = -FLT_MAX;
	min.z = min.y = min.x = FLT_MAX;
	Float3S* curPoint = (Float3S*)points;
	for (size_t i = 0; i < pointsCount; i++)
	{
		max3(max, *curPoint);
		min3(min, *curPoint);
		curPoint = (Float3S*)(((char*)curPoint) + lenthToNextPoint);
	}
	center = (max + min)*0.5;
	extend = (max - min)*0.5;
}
bool intersectBox(Float3S &objectCenter, Float3S &objectExtend, FrustMatrix &frustMatrix, Matrix &viewMatrixToObjectSpace)
{
    Vector translate;
    Vector scale;
    Vector rotate;
    viewMatrixToObjectSpace.getComp(translate, scale, rotate);
    DirectX::BoundingFrustum localSpaceFrus;
    frustMatrix.projFrust.Transform(localSpaceFrus, viewMatrixToObjectSpace.getMatrix());
    DirectX::BoundingBox object((DirectX::XMFLOAT3&)objectCenter, (DirectX::XMFLOAT3&)objectExtend);
    return frustMatrix.projFrust.Intersects(object);
}
#endif