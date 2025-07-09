#pragma once
#include "LA_Vector.h"

class Triangle {
public:
	Vec2 v0, v1, v2;

	int minX, maxX;
	int minY, maxY;
public:
	Triangle() {}
	Triangle(const Vec2& x1, const Vec2& x2, const Vec2& x3) : v0(x1), v1(x2), v2(x3) {}

	// ������ ������ Ȱ���� CCW �˻�.
	// 2���� ������ ������ 3���� �������� Z�� ���и��� ���. (0, 0, z)
	// �ﰢ���� �� ���� ���� ����� ��� ���� ��ȣ��� �ﰢ���� ���ο� �ִ�.
	float EdgeFunction(const Vec2& a, const Vec2& b, const Vec2& p) {
		return (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
	}

	// �ﰢ�� �������� �˻�
	bool IsInTriangle(const Vec2& p) {
		float w0 = EdgeFunction(v0, v1, p);
		float w1 = EdgeFunction(v1, v2, p);
		float w2 = EdgeFunction(v2, v0, p);

		return (w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0);
	}

	void SetBoundingBox(int screenWidth, int screenHeight);
};