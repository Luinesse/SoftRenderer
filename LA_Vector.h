#pragma once
#include <math.h>

class Vec2 {
public:
	float x, y;
	// 삼각형을 그릴때 사용할 색상을 저장.
	float r, g, b;
public:
	Vec2() : x(0), y(0), r(255), g(255), b(255) {

	}

	Vec2(float x, float y) : x(x), y(y) {

	}

	// 벡터 합
	Vec2 operator+ (const Vec2& v) const {
		return Vec2(x + v.x, y + v.y);
	}

	// 벡터 차
	Vec2 operator- (const Vec2& v) const {
		return Vec2(x - v.x, y - v.y);
	}

	// 벡터 스칼라곱
	Vec2 operator* (float s) const {
		return Vec2(x * s, y * s);
	}

	// 벡터 스칼라 나눗셈
	Vec2 operator/ (float s) const {
		if (s == 0.0f)	return Vec2(0, 0);
		return Vec2(x / s, y / s);
	}

	// 벡터의 길이(크기)
	float Length() const {
		return sqrtf(x * x + y * y);
	}

	// 벡터 정규화
	Vec2 Normalize() const {
		float len = Length();
		if (len == 0)	return Vec2(0, 0);
		return Vec2(x / len, y / len);
	}

	// 벡터의 내적 (내적의 결과는 스칼라)
	float Dot(const Vec2& v) const {
		return x * v.x + y * v.y;
	}

	/*
	
	2차원 벡터에서는 외적을 구현하지 않음.
	외적으로 두 벡터에 수직인 벡터를 구하는데, z축이 없기때문에
	굳이 z축이 0인 벡터들의 외적을 할 필요 X.

	*/
};

class Vec3 {
public:
	float x, y, z;
	Vec2 uv;
public:
	Vec3() : x(0), y(0), z(0) {

	}

	Vec3(float x, float y, float z) : x(x), y(y), z(z) {

	}

	Vec3(float x, float y, float z, Vec2 uv) : x(x), y(y), z(z), uv(uv) {

	}

	Vec3 operator+ (const Vec3& v) const {
		return Vec3(x + v.x, y + v.y, z + v.z);
	}

	Vec3 operator- (const Vec3& v) const {
		return Vec3(x - v.x, y - v.y, z - v.z);
	}

	Vec3 operator* (float s) const {
		return Vec3(x * s, y * s, z * s);
	}

	Vec3 operator/ (float s) const {
		if (s == 0)	return Vec3(0, 0, 0);
		return Vec3(x / s, y / s, z / s);
	}

	float Length() const {
		return sqrtf(x * x + y * y + z * z);
	}

	Vec3 Normalize() const {
		float len = Length();
		if (len == 0)	return Vec3(0, 0, 0);
		return Vec3(x / len, y / len, z / len);
	}

	float Dot(const Vec3& v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	Vec3 Cross(const Vec3& v) const {
		return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

class Vec4 {
public:
	float x, y, z, w;
public:
	Vec4() : x(0), y(0), z(0), w(0) {

	}

	Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {

	}

	Vec4(Vec3 xyz, float w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {

	}

	Vec4 operator+ (const Vec4& v) const {
		return Vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}

	Vec4 operator- (const Vec4& v) const {
		return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
	}

	Vec4 operator* (float s) const {
		return Vec4(x * s, y * s, z * s, w * s);
	}

	Vec4 operator/ (float s) const {
		if (s == 0)	return Vec4(0, 0, 0, 0);
		return Vec4(x / s, y / s, z / s, w / s);
	}

	float Length() const {
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	Vec4 Normalize() const {
		float len = Length();
		if (len == 0)	return Vec4(0, 0, 0, 0);
		return Vec4(x / len, y / len, z / len, w / len);
	}

	float Dot(const Vec4& v) const {
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	/*

	4차원 벡터에서도 외적을 구하지 않음.
	대상 벡터로 3차원의 유클리드 공간에 있는 벡터만이 대상이므로
	구현할 필요 X.

	*/
};