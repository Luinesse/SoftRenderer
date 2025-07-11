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

	// ������ ������ Ȱ���� CCW �˻�.
	// 2���� ������ ������ 3���� �������� Z�� ���и��� ���. (0, 0, z)
	// �ﰢ���� �� ���� ���� ����� ��� ���� ��ȣ��� �ﰢ���� ���ο� �ִ�.
	float EdgeFunction(const Vec2& a, const Vec2& b, const Vec2& p) {
		return (b.x - a.x) * (p.y - a.y) - (p.x - a.x) * (b.y - a.y);
	}

	// �ﰢ�� �������� �˻�
	bool IsInTriangle(const Vec2& p);
	void ComputeBarycentric(Vec2& p);

	// AABB ���� Ŭ���� ����.
	void SetBoundingBox(int screenWidth, int screenHeight);
	
	// �������� �ո����� �޸����� �˻�
	// ������ ���� �� �ڼ��� ���� !
	// ������ v0, v1, v2�� �־����� ��, v0 -> v1���� ���� ���� A�� v0 -> v2�� ���� ���� B�� ���� �����Ѵ�.
	// �� ���, B�� A�� �ݽð�� ������ �� ���´ٸ� ���. (�ո�)
	// B�� A�� �ð�� ������ �� ���´ٸ� ����. (�޸�)
	// ���ٸ� ���� ���� Ȥ�� ������.
	// �̰ɷ� CCW �˻簡 �����ϴ�.
	// ��, ������ ������ ��ǥ�踦 ���� ������ y���� �ݴ��ΰ� ����ؼ� ��������.
	bool IsFrontFacing() {
		return ((v1 - v0).x * (v2 - v0).y - (v1 - v0).y * (v2 - v0).x) > 0.0f;
	}

	// ������ �� back-face culling�� ȭ�鿡 ����� 2D ���� ��ǥ�� �̿��ؼ� �ﰢ���� CCW �Ǵ����� ����.
	// �̰Ŵ� ����/�� ���������� �ﰢ�� �븻 ���Ϳ� ī�޶� ���� ���͸� �̿��Ͽ� �޸������� �Ǵ��ϴ� �Լ�.
	// �ﰢ�� ���� �������� �� ���� edge1, edge2�� �����.
	// �� �� ������ �������� �븻 ���͸� �����.
	// �� �븻 ���Ϳ� ī�޶��� ���⺤�Ϳ��� �������� ���������� ���̸� ���Ѵ�.
	// ���������� ���̰� ������°�, ���� ���ֺ��� �ִٴ� ���̹Ƿ�, ���� ī�޶� �ٶ󺸰� �ֱ⶧���� �׷��ش�.

	// �� ������ �������Ͷ� ������ �ִ°� ?
	// �� ���� A, B�� �����ϸ�, ���� A���� ������ ���� ���� B�� �����ϰ� �ȴ�.
	// �� ��� �� ���ͻ��� cos��Ÿ�� ���� ���̰� ��������. (Acos��Ÿ)
	// ������ ũ�ٸ�, ���������� ���̴� ª��, ������ �۴ٸ� ���������� ���̴� Ŀ����.
	// �̰� �̿��� ����, ���������� ���̰� ������� ? ���� ���ֺ��� �ִٴ� ��.
	bool IsFrontFacing(const Vec3& cameraPos) const {
		Vec3 edge1 = vt1 - vt0;
		Vec3 edge2 = vt2 - vt0;

		// ��� ���� (���� ����)
		Vec3 normal = edge1.Cross(edge2);

		// ī�޶� �ﰢ�� ������ �ٶ󺸴� ���⺤��
		Vec3 viewDir = vt0 - cameraPos;

		// ���� ���� (������ ����� ��Į���̴�.)
		float dot = normal.Dot(viewDir);

		// �������, ���� ���ֺ��� �����Ƿ� �׷��־����.
		return dot < 0;
	}
};