#pragma once
#include "LA_Vector.h"

class Triangle {
public:
	Vec2 v0, v1, v2;
	Vec3 vt0, vt1, vt2;

	int minX, maxX;
	int minY, maxY;
public:
	Triangle() {}
	Triangle(const Vec2& x1, const Vec2& x2, const Vec2& x3) : v0(x1), v1(x2), v2(x3), minX(0), maxX(0), minY(0), maxY(0) {}
	Triangle(const Vec3& x1, const Vec3& x2, const Vec3& x3) : vt0(x1), vt1(x2), vt2(x3), minX(0), maxX(0), minY(0), maxY(0) {}

	// 벡터의 외적을 활용한 CCW 검사.
	// 2차원 벡터의 외적은 3차원 외적에서 Z축 성분만을 계산. (0, 0, z)
	// 삼각형의 세 변에 대해 결과가 모두 같은 부호라면 삼각형의 내부에 있다.
	float EdgeFunction(const Vec2& a, const Vec2& b, const Vec2& p) {
		return (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
	}

	// 삼각형 내부인지 검사
	bool IsInTriangle(const Vec2& p);
	void ComputeBarycentric(Vec2& p);

	// AABB 단위 클리핑 수행.
	void SetBoundingBox(int screenWidth, int screenHeight);
	
	// 외적으로 앞면인지 뒷면인지 검사
	// 외적에 대한 더 자세한 설명 !
	// 정점이 v0, v1, v2로 주어졌을 때, v0 -> v1으로 가는 벡터 A와 v0 -> v2로 가는 벡터 B를 만들어서 외적한다.
	// 그 결과, B가 A를 반시계로 돌렸을 때 나온다면 양수. (앞면)
	// B가 A를 시계로 돌렸을 때 나온다면 음수. (뒷면)
	// 같다면 같은 방향 혹은 일직선.
	// 이걸로 CCW 검사가 가능하다.
	// 단, 지금은 윈도우 좌표계를 쓰기 때문에 y축이 반대인걸 고려해서 생각하자.
	bool IsFrontFacing() {
		return ((v1 - v0).x * (v2 - v0).y - (v1 - v0).y * (v2 - v0).x) > 0.0f;
	}

	// 위에서 한 back-face culling은 화면에 출력할 2D 정점 좌표를 이용해서 삼각형의 CCW 판단으로 수행.
	// 이거는 월드/뷰 공간에서의 삼각형 노말 벡터와 카메라 방향 벡터를 이용하여 뒷면인지를 판단하는 함수.
	// 삼각형 면의 정점으로 두 벡터 edge1, edge2를 만든다.
	// 그 두 벡터의 외적으로 노말 벡터를 만든다.
	// 그 노말 벡터와 카메라의 방향벡터와의 내적으로 투영벡터의 길이를 구한다.
	// 투영벡터의 길이가 음수라는건, 서로 마주보고 있다는 뜻이므로, 면은 카메라를 바라보고 있기때문에 그려준다.

	// 왜 내적이 투영벡터랑 연관이 있는가 ?
	// 두 벡터 A, B를 내적하면, 벡터 A에서 수선의 발을 내려 B에 투영하게 된다.
	// 그 결과 두 벡터사이 cos세타에 따라 길이가 정해진다. (Acos세타)
	// 각도가 크다면, 투영벡터의 길이는 짧고, 각도가 작다면 투영벡터의 길이는 커진다.
	// 이걸 이용해 만일, 투영벡터의 길이가 음수라면 ? 서로 마주보고 있다는 것.
	bool IsFrontFacing(const Vec3& cameraPos) const {
		Vec3 edge1 = vt1 - vt0;
		Vec3 edge2 = vt2 - vt0;

		// 노멀 벡터 (법선 벡터)
		Vec3 normal = edge1.Cross(edge2);

		// 카메라가 삼각형 정점을 바라보는 방향벡터
		Vec3 viewDir = vt0 - cameraPos;

		// 내적 수행 (내적의 결과는 스칼라값이다.)
		float dot = normal.Dot(viewDir);

		// 음수라면, 서로 마주보고 있으므로 그려주어야함.
		return dot < 0;
	}
};