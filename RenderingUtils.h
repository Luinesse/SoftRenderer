#pragma once
#include "LA_Vector.h"

// 클리핑이 완료된 후 모든 정점 성분들을 w 성분으로 나누는 원근 나눗셈 수행.
// 이걸로 NDC 좌표를 얻을 수 있다.
// NDC 좌표는 x,y,z 모두 -1 ~ 1 범위에 있다.
inline Vec3 PerspectiveDivide(const Vec4& v) {
	if (v.w == 0.0f)	return Vec3(0, 0, 0);
	return Vec3(v.x / v.w, v.y / v.w, v.z / v.w);
}

// NDC 좌표를 화면에 표시하기 위해 스크린 좌표계로 변환
// NDC.x 가 -1일때 0이다.
// 우선 + 1을 해서 범위를 -1 ~ +1 에서 0 ~ +2 로 변환한다.
// 그 후 * 0.5 를 수행하여 0 ~ 1로 맞춰주고 screenWidth에 매핑한다.
// NDC.y 는 같은 원리로 수행하지만, NDC좌표와 스크린 좌표가 반대이기 때문에 1에서 뺄셈을 수행한다.
inline Vec2 ViewportTransform(const Vec3& ndcPos, int screenWidth, int screenHeight) {
	float x = (ndcPos.x + 1) * 0.5 * static_cast<float>(screenWidth);
	float y = (1 - (ndcPos.y + 1) * 0.5) * static_cast<float>(screenHeight);
	return Vec2(x, y);
}