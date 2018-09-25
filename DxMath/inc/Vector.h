#ifndef _VECTOR_H_
#define _VECTOR_H_
#include "Engine.h"
#include <DirectXMath.h>
#ifdef DX_11
typedef DirectX::XMVECTOR DxVector;
#else ifdef DX_9
#endif
class ShortPoolMemory;
ALIGN(16)
class Vector
{
public://public for everyone
	Vector();
	Vector(const Vector& copy);
	Vector(float x, float y, float z, float w = 0);
	Vector operator+ (const Vector &vec) const;
	Vector operator- (const Vector &vec) const;
	Vector operator* (const Vector &vec) const;
	Vector operator* (float num) const;
	Vector operator/ (const Vector &vec) const;
	Vector operator- () const;
	Vector minVec(const Vector &vec) const;
	Vector maxVec(const Vector &vec) const;
	float getX() const;
	float getY() const;
	float getZ() const;
	float getW() const;
	void* operator new(size_t);
	void operator delete(void*);
	void* operator new[](size_t);
	void operator delete[](void*);
	static Vector lerp(const Vector& v1, const Vector& v2, float percent);
	Vector& operator= (const Vector &color) = default;
public://public for engine
	Vector(const DxVector &copy);
	const DxVector& getVector() const;
	DxVector & getvector();
	float* const getFloat(ShortPoolMemory& mem) const;
protected://for engine
	Vector(DxVector &vec);
	DxVector vector;
private:
	static const int ALIGNE = 16;
};
#endif