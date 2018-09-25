#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_
#include "Vector.h"
#include "Engine.h"
class Vector2D :public Vector
{
public:
	bool operator==(const Vector2D &vec) const;
	bool operator>(const Vector2D &vec) const;
	bool operator<(const Vector2D &vec) const;
	float distance(const Vector2D &vec) const;
	float angleBetweenTwoVectors(const Vector2D &vec) const;
	float dot(const Vector2D &vec) const;
	float length() const;
	bool inBound(const Vector2D &vec) const;
	Vector2D cross(const Vector2D &vec) const;
	Vector2D normal();
	Vector2D reflect(const Vector2D &vec) const;
private:
	Vector2D& operator= (const Vector2D &color) = delete;
	Vector2D(DxVector &copy);
};
#endif