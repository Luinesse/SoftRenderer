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
	// �ﰢ���� ��ü ������ ���Ѵ�.
	// �� ������ ���� ����� �ﰢ���� ���� * 2 ������, ������ ���ϴ� ���̹Ƿ� ����X
	float area = EdgeFunction(v0, v1, v2);
	// �ﰢ���� �������� ����.
	if (area == 0.0f)	return;

	// ������ ���� P�� �������� �ﰢ���� 3���� ������ �ﰢ�� ��ü �������� �󸶳� �����ϴ��� ������ ���Ѵ�.
	// �� ������ ���� ���� ������ �Ͼ ����
	p.r = EdgeFunction(v1, v2, p) / area;
	p.g = EdgeFunction(v2, v0, p) / area;
	p.b = EdgeFunction(v0, v1, p) / area;
}

// �ﰢ���� �ٿ�� �ڽ� ����
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
