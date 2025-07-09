#pragma once
#include "LA_Vector.h"

class Matrix {
public:
	float m[4][4];
public:
	Matrix();

	static Matrix Identity();
	static Matrix Translation(float tx, float ty, float tz);
	static Matrix Scale(float sx, float sy, float sz);
	static Matrix RotationX(float rad);
	static Matrix RotationY(float rad);
	static Matrix RotationZ(float rad);

	Matrix operator* (const Matrix& m) const;
	Vec4 operator* (const Vec4& v) const;

	void Print();
};