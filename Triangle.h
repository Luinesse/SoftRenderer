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

	// 벡터의 외적을 활용한 CCW 검사.
	// 2차원 벡터의 외적은 3차원 외적에서 Z축 성분만을 계산. (0, 0, z)
	// 삼각형의 세 변에 대해 결과가 모두 같은 부호라면 삼각형의 내부에 있다.
	float EdgeFunction(const Vec2& a, const Vec2& b, const Vec2& p) {
		return (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
	}

	// 삼각형 내부인지 검사
	bool IsInTriangle(const Vec2& p) {
		float w0 = EdgeFunction(v0, v1, p);
		float w1 = EdgeFunction(v1, v2, p);
		float w2 = EdgeFunction(v2, v0, p);

		return (w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0);
	}

	void SetBoundingBox(int screenWidth, int screenHeight);
};