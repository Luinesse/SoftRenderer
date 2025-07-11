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
	// 원근 투영 행렬 (왼손 좌표계 | DirectX 환경이라고 가정 OpenGL 이라면 오른손 좌표계)
	static Matrix PerspectiveFovLH(float fovY, float aspect, float zNear, float zFar);

	Matrix operator* (const Matrix& m) const;
	Vec4 operator* (const Vec4& v) const;

	void Print();
};