#include "Vector4D.h"
using namespace DirectX;
Vector4D::Vector4D(DxVector &copy) :Vector(copy)
{

}
bool Vector4D::operator==(const Vector4D & vec) const
{
	return XMVector4Equal(this->vector, vec.vector) != 0;
}
bool Vector4D::operator>(const Vector4D & vec) const
{
	return XMVector4Greater(this->vector, vec.vector) != 0;
}
bool Vector4D::operator<(const Vector4D & vec) const
{
	return XMVector4Less(this->vector, vec.vector) != 0;
}
float Vector4D::distance(const Vector4D & vec) const
{
	return Vector(XMVector4Length(XMVectorSubtract(this->vector, vec.vector))).getX();
}
float Vector4D::angleBetweenTwoVectors(const Vector4D & vec) const
{
	return Vector(XMVector4AngleBetweenVectors(this->vector, vec.vector)).getX();
}
float Vector4D::dot(const Vector4D & vec) const
{
	return Vector(XMVector4Dot(this->vector, vec.vector)).getX();
}
float Vector4D::length() const
{
	return Vector(XMVector4Length(this->vector)).getX();
}
bool Vector4D::inBound(const Vector4D & vec) const
{
	return XMVector4InBounds(this->vector, vec.vector) != 0;
}
Vector4D Vector4D::cross(const Vector4D & vec, const Vector4D &vec1) const
{
	return Vector4D(XMVector4Cross(this->vector, vec.vector, vec1.vector));
}
Vector4D Vector4D::normal()
{
	return Vector4D(XMVector4Normalize(this->vector));
}
Vector4D Vector4D::reflect(const Vector4D & vec) const
{
	return Vector4D(XMVector4Reflect(this->vector, vec.vector));
}