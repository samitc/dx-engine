#include "Vector3D.h"
using namespace DirectX;
Vector3D::Vector3D(DxVector &copy) :Vector(copy)
{

}
bool Vector3D::operator==(const Vector3D & vec) const
{
	return XMVector3Equal(this->vector, vec.vector) != 0;
}
bool Vector3D::operator>(const Vector3D & vec) const
{
	return XMVector3Greater(this->vector, vec.vector) != 0;
}
bool Vector3D::operator<(const Vector3D & vec) const
{
	return XMVector3Less(this->vector, vec.vector) != 0;
}
float Vector3D::distance(const Vector3D & vec) const
{
	return Vector(XMVector3Length(XMVectorSubtract(this->vector, vec.vector))).getX();
}
float Vector3D::angleBetweenTwoVectors(const Vector3D & vec) const
{
	return Vector(XMVector3AngleBetweenVectors(this->vector, vec.vector)).getX();
}
float Vector3D::dot(const Vector3D & vec) const
{
	return Vector(XMVector3Dot(this->vector, vec.vector)).getX();
}
float Vector3D::length() const
{
	return Vector(XMVector3Length(this->vector)).getX();
}
bool Vector3D::inBound(const Vector3D & vec) const
{
	return XMVector3InBounds(this->vector, vec.vector) != 0;
}
Vector3D Vector3D::cross(const Vector3D & vec) const
{
	return Vector3D(XMVector3Cross(this->vector, vec.vector));
}
Vector3D Vector3D::normal()
{
	return Vector3D(XMVector3Normalize(this->vector));
}
Vector3D Vector3D::reflect(const Vector3D & vec) const
{
	return Vector3D(XMVector3Reflect(this->vector, vec.vector));
}