#include "Matrix.h"
#include "Vector.h"
#include "GeneralException.h"
#include "BaseLog.h"
using namespace DirectX;
#define MatrixMultiFunc(func)	return *this*Matrix((func));
Matrix::Matrix():matrix(XMMatrixIdentity())
{
}
Matrix::Matrix(const float vals[4][4]):matrix(vals[0][0], vals[0][1], vals[0][2], vals[0][3], 
	vals[1][0], vals[1][1], vals[1][2], vals[1][3], 
	vals[2][0], vals[2][1], vals[2][2], vals[2][3], 
	vals[3][0], vals[3][1], vals[3][2], vals[3][3])
{
}
Matrix::Matrix(const Matrix &copy) :matrix(copy.matrix)
{
}
Matrix::Matrix(const XMMATRIX &copy) :matrix(copy)
{
}
XMMATRIX Matrix::getMatrix() const
{
	return matrix;
}
Matrix Matrix::affineTransformation(const Vector & translation, const Vector & rotationQuat, const Vector & rotationOrigin, const Vector & s)
{
	return Matrix(XMMatrixAffineTransformation(s.getVector(), rotationOrigin.getVector(), rotationQuat.getVector(), translation.getVector()));
}
Matrix Matrix::inverse() const
{
	Vector v;
	return Matrix(XMMatrixInverse(&v.getvector(), matrix));
}
Matrix Matrix::operator+(const Matrix & matrix) const
{
	Matrix ret;
	for (int i = 0; i < 4; i++)
	{
		ret.matrix.r[i] = this->matrix.r[i] + matrix.matrix.r[i];
	}
	return ret;
}
Matrix Matrix::operator* (const Matrix &matrix) const
{
	return Matrix(XMMatrixMultiply(this->matrix, matrix.matrix));
}
Matrix Matrix::operator/ (const Matrix &matrix) const
{
	Vector v;
	Matrix inv = Matrix(XMMatrixInverse(&(v.getvector()), matrix.matrix));
	if (v.getX() == 0)
	{
		BaseLog::getInstance().warn("can not divide matrix that can not being inverse");
		throw GeneralException("can not divide matrix that can not being inverse");
	}
	return Matrix(*this*inv);
}
Matrix Matrix::rotationXYZ(const Vector &angles)
{
	MatrixMultiFunc(XMMatrixRotationRollPitchYawFromVector(angles.getVector()));
}
Matrix Matrix::scale(const float x, const float y, const float z)
{
	MatrixMultiFunc(XMMatrixScaling(x, y, z));
}
Matrix Matrix::scale(const Vector & scale)
{
	MatrixMultiFunc(XMMatrixScalingFromVector(scale.getVector()));
}
void Matrix::getComp(Vector & trans, Vector & scale, Vector & rot) const
{
	XMMatrixDecompose(&scale.getvector(), &rot.getvector(), &trans.getvector(), matrix);
}
Matrix Matrix::rotationXYZ(const float angleX, const float angleY, const float angleZ)
{
	MatrixMultiFunc(XMMatrixRotationRollPitchYaw(angleX, angleY, angleZ));
}
Matrix Matrix::translation(const float X, const float Y, const float Z)
{
	MatrixMultiFunc(XMMatrixTranslation(X, Y, Z));
}
Matrix Matrix::translation(const Vector &vector)
{
	MatrixMultiFunc(XMMatrixTranslationFromVector(vector.getVector()));
}
Matrix Matrix::transpose()
{
	return Matrix(XMMatrixTranspose(this->matrix));
}
Matrix Matrix::rotationX(const float angle)
{
	MatrixMultiFunc(XMMatrixRotationX(angle));
}
Matrix Matrix::rotationY(const float angle)
{
	MatrixMultiFunc(XMMatrixRotationY(angle));
}
Matrix Matrix::rotationZ(const float angle)
{
	MatrixMultiFunc(XMMatrixRotationZ(angle));
}
Vector Matrix::getVector(int i) const
{
	if (i > 3 || i < 0)
	{
		BaseLog::getInstance().warn("out of range(4x4 matrix)");
		throw GeneralException("out of range(4x4 matrix)");
	}
	return Vector(this->matrix.r[i]);
}
Matrix Matrix::LookAtLH( const Vector &camPos, const Vector &camLookAt, const Vector &camUpVector)
{
	return Matrix(XMMatrixLookAtLH(camPos.getVector(), camLookAt.getVector(), camUpVector.getVector()));
}
Matrix Matrix::PerspectiveFovLH(float angle, float ratio, float minRange, float maxRange)
{
	return Matrix(XMMatrixPerspectiveFovLH(angle, ratio, minRange, maxRange));
}
Matrix Matrix::LookAtRH(const Vector & camPos, const Vector & camLookAt, const Vector & camUpVector)
{
	return Matrix(XMMatrixLookAtRH(camPos.getVector(), camLookAt.getVector(), camUpVector.getVector()));
}
Matrix Matrix::PerspectiveFovRH(float angle, float ratio, float minRange, float maxRange)
{
	return Matrix(XMMatrixPerspectiveFovRH(angle, ratio, minRange, maxRange));
}
void * Matrix::operator new(size_t size)
{
	if (size == 0)
	{
		size = 1;
	}
return _aligned_malloc(size,ALIGNE);
}
void Matrix::operator delete(void * p)
{
	if (p != 0)
	{
		_aligned_free(p);
	}
}
void * Matrix::operator new[](size_t size)
{
	if (size == 0)
	{
		size = 1;
	}
	return _aligned_malloc(size, ALIGNE);
}
void Matrix::operator delete[](void * p)
{
	if (p != 0)
	{
		_aligned_free(p);
	}
}