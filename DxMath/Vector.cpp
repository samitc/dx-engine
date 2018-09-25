#include "Vector.h"
#include "ShortPoolMemory.h"
using namespace DirectX;
Vector::Vector():vector(XMVectorSet(0,0,0,0))
{
}
Vector::Vector(const Vector& copy):vector(XMVectorSet(copy.getX(), copy.getY(), copy.getZ(), copy.getW()))
{
}
Vector::Vector(float x, float y, float z, float w):vector(XMVectorSet(x,y,z,w))
{
}
Vector::Vector(DxVector &vec) : vector(vec)
{

}
Vector::Vector(const DxVector & copy) : Vector(copy.m128_f32[0], copy.m128_f32[1], copy.m128_f32[2], copy.m128_f32[3])
{
}
Vector Vector::minVec(const Vector & vec) const
{
	return Vector(XMVectorMin(this->vector, vec.vector));
}
Vector Vector::maxVec(const Vector & vec) const
{
	return Vector(XMVectorMax(this->vector, vec.vector));
}
Vector Vector::operator+ (Vector const &vec) const
{
	return Vector(XMVectorAdd(this->vector, vec.vector));
}
Vector Vector::operator-(const Vector & vec) const
{
	return Vector(XMVectorSubtract(this->vector, vec.vector));
}
Vector Vector::operator*(const Vector & vec) const
{
	return Vector(XMVectorMultiply(this->vector, vec.vector));
}
Vector Vector::operator*(float num) const
{
	return Vector(XMVectorMultiply(this->vector, Vector(num, num, num, num).vector));
}
Vector Vector::operator/(const Vector & vec) const
{
	return Vector(XMVectorDivide(this->vector, vec.vector));
}
Vector Vector::operator-() const
{
	return Vector(-getX(), -getY(), -getZ(), -getW());
}
float Vector::getX() const
{
	return this->vector.m128_f32[0];
}
float Vector::getY() const
{
	return this->vector.m128_f32[1];
}
float Vector::getZ() const
{
	return this->vector.m128_f32[2];
}
float Vector::getW() const
{
	return this->vector.m128_f32[3];
}
DxVector & Vector::getvector()
{
	return this->vector;
}
const DxVector& Vector::getVector() const
{
	return this->vector;
}
void * Vector::operator new(size_t size)
{
	if (size == 0)
	{
		size = 1;
	}
	return _aligned_malloc(size,ALIGNE);
}
void Vector::operator delete(void * p)
{
	if (p != 0)
	{
		_aligned_free(p);
	}
}
void * Vector::operator new[](size_t size)
{
	if (size == 0)
	{
		size = 1;
	}
	return _aligned_malloc(size, ALIGNE);
}
void Vector::operator delete[](void * p)
{
	if (p != 0)
	{
		_aligned_free(p);
	}
}
Vector Vector::lerp(const Vector & v1, const Vector & v2, float percent)
{
	return Vector(XMVectorLerp(v1.getVector(), v2.getVector(), percent));
}
float * const Vector::getFloat(ShortPoolMemory& mem) const
{
	float * ret = (float*)mem.alloc(4 * sizeof(float));
	ret[0] = this->getX();
	ret[1] = this->getY();
	ret[2] = this->getZ();
	ret[3] = this->getW();
	return ret;
}