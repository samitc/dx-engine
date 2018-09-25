#ifndef _MATRIX_H_
#define _MATRIX_H_
#include "Engine.h"
#include <DirectXMath.h>
typedef DirectX::XMMATRIX DxMatrix;
#define MATRIX_ALIGN	16
class Vector;
ALIGN(MATRIX_ALIGN)
//this class represent 4X4 matrix
class Matrix
{
public://public for everyone
	Matrix();
	Matrix(const float vals[4][4]);
	Matrix(const Matrix &copy);
	~Matrix() = default;
	Matrix operator+ (const Matrix &matrix) const;
	Matrix operator* (const Matrix &matrix) const;
	Matrix operator/ (const Matrix &matrix) const;
	Matrix translation(const float X, const float Y, const float Z);
	Matrix translation(const Vector &vector);
	Matrix transpose();
	Matrix rotationX(const float angle);
	Matrix rotationY(const float angle);
	Matrix rotationZ(const float angle);
	Matrix rotationXYZ(const float angleX, const float angleY, const float angleZ);
	Matrix rotationXYZ(const Vector &angle);
	Matrix scale(const float x, const float y, const float z);
	Matrix scale(const Vector &scale);
	void getComp(Vector &trans, Vector &scale, Vector& rot) const;
	Vector getVector(int i) const;
	//create look at matrix for left handed
	//parameters:
	//	camPos:the position of the camera
	//	camLookAt:the point that the camera is looking
	//	camUpVector:vector that represent the "up" direction of the camera
	static Matrix LookAtLH(const Vector &camPos, const Vector &camLookAt, const Vector &camUpVector);
	//create perspective matrix for left handed
	//parameters:
	//	angle:the angle of the camera view
	//	ratio:the ratio of the width by the height of the window
	//	minRange:the nearest point that the camera can see
	//	maxRange:the longest point that the camera can see 
	static Matrix PerspectiveFovLH(float angle, float ratio, float minRange, float maxRange);
	//create look at matrix for right handed
	//parameters:
	//	camPos:the position of the camera
	//	camLookAt:the point that the camera is looking
	//	camUpVector:vector that represent the "up" direction of the camera
	static Matrix LookAtRH(const Vector &camPos, const Vector &camLookAt, const Vector &camUpVector);
	//create perspective matrix for right handed
	//parameters:
	//	angle:the angle of the camera view
	//	ratio:the ratio of the width by the height of the window
	//	minRange:the nearest point that the camera can see
	//	maxRange:the longest point that the camera can see
	static Matrix PerspectiveFovRH(float angle, float ratio, float minRange, float maxRange);
	void* operator new(size_t);
	void operator delete(void*);
	void* operator new[](size_t);
	void operator delete[](void*);
	static Matrix affineTransformation(const Vector& translation, const Vector& rotationQuat, const Vector &rotationOrigin, const Vector &scale);
	Matrix inverse() const;
public://public for engine
	Matrix(const DxMatrix &copy);
	DxMatrix getMatrix() const;
protected:
	
private:
	static const int ALIGNE = 16;
	DxMatrix matrix;
};
#endif