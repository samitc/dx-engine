#include "Vector2D.h"
using namespace DirectX;
Vector2D::Vector2D(DxVector &copy):Vector(copy)
{

}
bool Vector2D::operator==(const Vector2D & vec) const
{
	return XMVector2Equal(this->vector, vec.vector) != 0;
}
bool Vector2D::operator>(const Vector2D & vec) const
{
	return XMVector2Greater(this->vector, vec.vector) != 0;
}
bool Vector2D::operator<(const Vector2D & vec) const
{
	return XMVector2Less(this->vector, vec.vector) != 0;
}
float Vector2D::distance(const Vector2D & vec) const
{
	return Vector(XMVector2Length(XMVectorSubtract(this->vector, vec.vector))).getX();
}
float Vector2D::angleBetweenTwoVectors(const Vector2D & vec) const
{
	return Vector(XMVector2AngleBetweenVectors(this->vector, vec.vector)).getX();
}
float Vector2D::dot(const Vector2D & vec) const
{
	return Vector(XMVector2Dot(this->vector, vec.vector)).getX();
}
float Vector2D::length() const
{
	return Vector(XMVector2Length(this->vector)).getX();
}
bool Vector2D::inBound(const Vector2D & vec) const
{
	return XMVector2InBounds(this->vector, vec.vector) != 0;
}
Vector2D Vector2D::cross(const Vector2D & vec) const
{
	return Vector2D(XMVector2Cross(this->vector, vec.vector));
}
Vector2D Vector2D::normal()
{
	return Vector2D(XMVector2Normalize(this->vector));
}
Vector2D Vector2D::reflect(const Vector2D & vec) const
{
	return Vector2D(XMVector2Reflect(this->vector, vec.vector));
}