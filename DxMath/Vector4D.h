#ifndef _VECTOR4D_H_
#define _VECTOR4D_H_
#include "Vector.h"
#include "Engine.h"
class Vector4D :public Vector
{
public:
	bool operator==(const Vector4D &vec) const;
	bool operator>(const Vector4D &vec) const;
	bool operator<(const Vector4D &vec) const;
	float distance(const Vector4D &vec) const;
	float angleBetweenTwoVectors(const Vector4D &vec) const;
	float dot(const Vector4D &vec) const;
	float length() const;
	bool inBound(const Vector4D &vec) const;
	Vector4D cross(const Vector4D &vec, const Vector4D &vec1) const;
	Vector4D normal();
	Vector4D reflect(const Vector4D &vec) const;
private:
	Vector4D& operator= (const Vector4D &color) = delete;
	Vector4D(DxVector &copy);
};
#endif