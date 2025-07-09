#include "Triangle.h"
#include <iostream>

using namespace std;

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
