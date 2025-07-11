#pragma once
#include "LA_Vector.h"

// Ŭ������ �Ϸ�� �� ��� ���� ���е��� w �������� ������ ���� ������ ����.
// �̰ɷ� NDC ��ǥ�� ���� �� �ִ�.
// NDC ��ǥ�� x,y,z ��� -1 ~ 1 ������ �ִ�.
inline Vec3 PerspectiveDivide(const Vec4& v) {
	if (v.w == 0.0f)	return Vec3(0, 0, 0);
	return Vec3(v.x / v.w, v.y / v.w, v.z / v.w);
}

// NDC ��ǥ�� ȭ�鿡 ǥ���ϱ� ���� ��ũ�� ��ǥ��� ��ȯ
// NDC.x �� -1�϶� 0�̴�.
// �켱 + 1�� �ؼ� ������ -1 ~ +1 ���� 0 ~ +2 �� ��ȯ�Ѵ�.
// �� �� * 0.5 �� �����Ͽ� 0 ~ 1�� �����ְ� screenWidth�� �����Ѵ�.
// NDC.y �� ���� ������ ����������, NDC��ǥ�� ��ũ�� ��ǥ�� �ݴ��̱� ������ 1���� ������ �����Ѵ�.
inline Vec2 ViewportTransform(const Vec3& ndcPos, int screenWidth, int screenHeight) {
	float x = (ndcPos.x + 1) * 0.5 * static_cast<float>(screenWidth);
	float y = (1 - (ndcPos.y + 1) * 0.5) * static_cast<float>(screenHeight);
	return Vec2(x, y);
}