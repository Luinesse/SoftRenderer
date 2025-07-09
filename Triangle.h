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

	// 벡터의 외적을 활용한 CCW 검사.
	// 2차원 벡터의 외적은 3차원 외적에서 Z축 성분만을 계산. (0, 0, z)
	// 삼각형의 세 변에 대해 결과가 모두 같은 부호라면 삼각형의 내부에 있다.
	float EdgeFunction(const Vec2& a, const Vec2& b, const Vec2& p) {
		return (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
	}

	// 삼각형 내부인지 검사
	bool IsInTriangle(const Vec2& p);
	void ComputeBarycentric(Vec2& p);

	void SetBoundingBox(int screenWidth, int screenHeight);
	
};