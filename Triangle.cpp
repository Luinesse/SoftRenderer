#include "Triangle.h"
#include <iostream>

using namespace std;

bool Triangle::IsInTriangle(const Vec2& p)
{
	float w0 = EdgeFunction(v0, v1, p);
	float w1 = EdgeFunction(v1, v2, p);
	float w2 = EdgeFunction(v2, v0, p);

	return (w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0);
}

void Triangle::ComputeBarycentric(Vec2& p)
{
	// 삼각형의 전체 면적을 구한다.
	// 이 연산의 실제 결과는 삼각형의 면적 * 2 이지만, 비율을 구하는 것이므로 문제X
	float area = EdgeFunction(v0, v1, v2);
	// 삼각형이 생성되지 않음.
	if (area == 0.0f)	return;

	// 임의의 정점 P를 기준으로 삼각형을 3개로 나누고 삼각형 전체 면적에서 얼마나 차지하는지 비율을 구한다.
	// 이 비율을 통해 색상 보간이 일어날 예정
	p.r = EdgeFunction(v1, v2, p) / area;
	p.g = EdgeFunction(v2, v0, p) / area;
	p.b = EdgeFunction(v0, v1, p) / area;
}

// 삼각형의 바운딩 박스 구현
void Triangle::SetBoundingBox(int screenWidth, int screenHeight)
{
	minX = static_cast<int>(floorf(min({ v0.x, v1.x, v2.x })));
	maxX = static_cast<int>(ceilf(max({ v0.x, v1.x, v2.x })));
	minY = static_cast<int>(floorf(min({ v0.y, v1.y, v2.y })));
	maxY = static_cast<int>(ceilf(max({ v0.y, v1.y, v2.y })));
	
	minX = max(minX, 0);
	maxX = min(maxX, screenWidth - 1);
	minY = max(minY, 0);
	maxY = min(maxY, screenHeight - 1);
}
