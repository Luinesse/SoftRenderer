#pragma once
#include <math.h>

class Vec2 {
public:
	float x, y;
	// �ﰢ���� �׸��� ����� ������ ����.
	float r, g, b;
public:
	Vec2() : x(0), y(0), r(255), g(255), b(255) {

	}

	Vec2(float x, float y) : x(x), y(y) {

	}

	// ���� ��
	Vec2 operator+ (const Vec2& v) const {
		return Vec2(x + v.x, y + v.y);
	}

	// ���� ��
	Vec2 operator- (const Vec2& v) const {
		return Vec2(x - v.x, y - v.y);
	}

	// ���� ��Į���
	Vec2 operator* (float s) const {
		return Vec2(x * s, y * s);
	}

	// ���� ��Į�� ������
	Vec2 operator/ (float s) const {
		if (s == 0.0f)	return Vec2(0, 0);
		return Vec2(x / s, y / s);
	}

	// ������ ����(ũ��)
	float Length() const {
		return sqrtf(x * x + y * y);
	}

	// ���� ����ȭ
	Vec2 Normalize() const {
		float len = Length();
		if (len == 0)	return Vec2(0, 0);
		return Vec2(x / len, y / len);
	}

	// ������ ���� (������ ����� ��Į��)
	float Dot(const Vec2& v) const {
		return x * v.x + y * v.y;
	}

	/*
	
	2���� ���Ϳ����� ������ �������� ����.
	�������� �� ���Ϳ� ������ ���͸� ���ϴµ�, z���� ���⶧����
	���� z���� 0�� ���͵��� ������ �� �ʿ� X.

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

	4���� ���Ϳ����� ������ ������ ����.
	��� ���ͷ� 3������ ��Ŭ���� ������ �ִ� ���͸��� ����̹Ƿ�
	������ �ʿ� X.

	*/
};