#pragma once
#include "LA_Vector.h"

class Triangle {
public:
	Vec2 v0, v1, v2;

	int minX, maxX;
	int minY, maxY;
public:
	Triangle() {}
	Triangle(const Vec2& x1, const Vec2& x2, const Vec2& x3) : v0(x1), v1(x2), v2(x3), minX(0), maxX(0), minY(0), maxY(0) {}

	// ������ ������ Ȱ���� CCW �˻�.
	// 2���� ������ ������ 3���� �������� Z�� ���и��� ���. (0, 0, z)
	// �ﰢ���� �� ���� ���� ����� ��� ���� ��ȣ��� �ﰢ���� ���ο� �ִ�.
	float EdgeFunction(const Vec2& a, const Vec2& b, const Vec2& p) {
		return (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
	}

	// �ﰢ�� �������� �˻�
	bool IsInTriangle(const Vec2& p);
	void ComputeBarycentric(Vec2& p);

	void SetBoundingBox(int screenWidth, int screenHeight);
	
};