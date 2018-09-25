#ifndef _VECTOR3D_H_
#define _VECTOR3D_H_
#include "Vector.h"
#include "Engine.h"
class Vector3D :public Vector
{
public:
	bool operator==(const Vector3D &vec) const;
	bool operator>(const Vector3D &vec) const;
	bool operator<(const Vector3D &vec) const;
	float distance(const Vector3D &vec) const;
	float angleBetweenTwoVectors(const Vector3D &vec) const;
	float dot(const Vector3D &vec) const;
	float length() const;
	bool inBound(const Vector3D &vec) const;
	Vector3D cross(const Vector3D &vec) const;
	Vector3D normal();
	Vector3D reflect(const Vector3D &vec) const;
private:
	Vector3D& operator= (const Vector3D &color) = delete;
	Vector3D(DxVector &copy);
};
#endif