#include "LA_Matrix.h"
#include <iostream>
#include <Windows.h>

using namespace std;

Matrix::Matrix()
{
	// ����� �ʱ�ȭ�� ���� ��ķ� �ʱ�ȭ �Ѵ�. (�밢������ 1�� ���)
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			// ���׿����ڸ� ���� ���� �ε����� �밢�������� Ȯ�� �� �� ����
			m[i][k] = (i == k) ? 1.0f : 0.0f;
		}
	}
}

// ���� ����� �����Ѵ�.
Matrix Matrix::Identity()
{
	Matrix mat;

	return mat;
}

// ȸ��, ũ��, ������ ������ȯ�̶� 3X3 ����� ��İ����� ǥ���� �����ߴ�.
// �̵��� �������� ���� ���Ͽ� ��İ����� ǥ���� �Ұ����ϴ�.
// ����, ������ 1���� �÷��� ������. (������ǥ��)
// �� ��� ȸ��, ũ��, �̵�, ������ ��� ��İ����� ���� ����.

// �������̶� ? �������� 1�� �������� �����ϴ� �� �ǹ��Ѵ�.
// �������̶� ? f(x1 + x2) = f(x1) + f(x2)
// 1�� �������̶� ? f(k * x) = k * f(x)

/*

�̵� ��ȯ���� �������� �����ϱ� ���� ���� ��ǥ�踦 �̿��ϴµ�
�̷��� ���� �Ѱ� �÷��� ������ ���� �������� ���� ���� Ȱ���Ѵ�. (3���� ���� x,y,z �� 4���� ���� x,y,z,1��. 3���� ����� 4���� ��ķ�.) (��, ������ w ���� 1�� ������.)
������� 4X4 ��İ� 4���� ������ ��İ����� �̵� ��ȯ�� ���������� ���డ��������.

*/

Matrix Matrix::Translation(float tx, float ty, float tz)
{
	// �����ڿ� ���� ������ķ� �ʱ�ȭ ��. ���� �Լ��鵵 ����.
	Matrix mat;
	// ���� x���̵�, y���̵�, z���̵�
	// 4X4 ����� ������ ������ ���� �̵����� �־���.
	// �� ����� �̵��� ���ϴ� ���Ϳ� ��İ� �����ϸ�, �̵��� �Ϸ�� ���͸� ���� �� �ִ�.
	mat.m[0][3] = tx;
	mat.m[1][3] = ty;
	mat.m[2][3] = tz;

	return mat;
}

// �������� ���� ��ȯ ũ�� ��ķ� ǥ���Ҷ����� �밢 ������ ���� �����Ͽ� ����.

Matrix Matrix::Scale(float sx, float sy, float sz)
{
	Matrix mat;

	mat.m[0][0] = sx;
	mat.m[1][1] = sy;
	mat.m[2][2] = sz;

	return mat;
}

// ȸ�� ��ȯ�� �࿡ ���� �ٸ��� ǥ��

Matrix Matrix::RotationX(float rad)
{
	Matrix mat;

	mat.m[1][1] = cosf(rad);
	mat.m[1][2] = -sinf(rad);
	mat.m[2][1] = sinf(rad);
	mat.m[2][2] = cosf(rad);

	return mat;

}

Matrix Matrix::RotationY(float rad)
{
	Matrix mat;

	mat.m[0][0] = cosf(rad);
	mat.m[0][2] = sinf(rad);
	mat.m[2][0] = -sinf(rad);
	mat.m[2][2] = cosf(rad);

	return mat;
}

Matrix Matrix::RotationZ(float rad)
{
	Matrix mat;

	mat.m[0][0] = cosf(rad);
	mat.m[0][1] = -sinf(rad);
	mat.m[1][0] = sinf(rad);
	mat.m[1][1] = cosf(rad);

	return mat;
}

Matrix Matrix::PerspectiveFovLH(float fovY, float aspect, float zNear, float zFar)
{
	Matrix mat;

	float yScale = 1.0f / tanf(fovY * 0.5f);
	float xScale = yScale / aspect;

	// �޼� ��ǥ�� ���� 4 By 4 ���� ��� �ʱ�ȭ

	mat.m[0][0] = xScale;								// ��Ⱦ�� ���� x ���� ���� ����. (xScale)
	mat.m[1][1] = yScale;								// ���� �þ߰� / 2 �� tan������ ȭ���� ���� ���̰� ����(ī�޶�)�� ���� �󸶳� �������ִ��� Ȯ��. �׿� ���� �þ߰� �������ų� �о���. (ī�޶��� �ܰ� ����ϴٰ� �Ѵ�)
	mat.m[2][2] = zFar / (zFar - zNear);				// ī�޶� ���� ���� ����� ������ ���� �� ��鰣�� ���� ���� 0~1 ������ ����ȭ.
	mat.m[2][3] = (-zNear * zFar) / (zFar - zNear);		// z���� zNear�� ���ٸ�, 0 zFar�� ���ٸ� 1�� ����ȭ���ش�.
	mat.m[3][2] = 1.0f;									// ��������� ��ġ�� 4���� ���Ͱ� ��������µ� �ű⼭ w���� �������µ� ��������. �̷��� ���־�� �� w���� z���� ������ ���� ���ٰ��� �����. (���� �������� �����ϴϱ�)
	mat.m[3][3] = 0.0f;									// 0�̾�� w �и� ���ܼ� ���ٳ����� ���밡��. 1�̶�� �������� (Orthographic)

	return mat;
}

Matrix Matrix::LookAtLH(const Vec3& eye, const Vec3& target, const Vec3& up)
{
	// ī�޶� Ÿ���� �ٶ󺸴� ���� (����)
	// A -> B ���� ���ʹ� B - A �� ���� �� �־���.
	Vec3 zAxis = (target - eye).Normalize();

	// ������ ���� (�޼� ��ǥ���̹Ƿ� x���� �������̴�.)
	// ī�޶��� ���� ���� ���Ϳ� ������ ���� ������� ������ �������� ��������.
	// �� ������ ������ �� ���Ϳ� ������ ���͸� ���Ѵ�.
	Vec3 xAxis = up.Cross(zAxis).Normalize();

	// ���� ����
	// ���� �������� �� ���Ϳ� ������ ���͸� ���Ѵ�.
	Vec3 yAxis = zAxis.Cross(xAxis);          

	// �� �������� ī�޶� �������� �ϴ� ���ο� ���� ��ǥ�踦 �����. (����� ���� ����?)

	Matrix view;

	// �� �������� ������. (DirectX. OpenGL �������δ� �ش� ����� ��ġ���� �����ϸ� �ȴ�.)
	// 1�� (Right)
	// x���� ȸ�� ���� (0~2��)
	// x���� �̵� ���� (3��)
	// �̵� ������ ī�޶�� �� ������ ������ x����� ������ ���� ���� ������ ���̸� ���Ѵ�. (3D ���������� Back-face Culling �Ҷ��� ���÷�����)
	// ���� ���� ���� ���̸�ŭ ������ �����Ͽ� ī�޶� ������ �ְԲ� �Ѵ�. (�� ��Ŀ��� ��ü�� �����̴°� ī�޶� ������ �ְ�, ������ �����̴� ���°� ���״ϱ�)
	view.m[0][0] = xAxis.x;
	view.m[1][0] = xAxis.y;
	view.m[2][0] = xAxis.z;
	view.m[3][0] = -xAxis.Dot(eye);

	// 2�� (Up)
	view.m[0][1] = yAxis.x;
	view.m[1][1] = yAxis.y;
	view.m[2][1] = yAxis.z;
	view.m[3][1] = -yAxis.Dot(eye);

	// 3�� (Forward)
	view.m[0][2] = zAxis.x;
	view.m[1][2] = zAxis.y;
	view.m[2][2] = zAxis.z;
	view.m[3][2] = -zAxis.Dot(eye);

	// 4��
	// ������.
	view.m[0][3] = 0.0f;
	view.m[1][3] = 0.0f;
	view.m[2][3] = 0.0f;
	view.m[3][3] = 1.0f;

	return view;
}

// �� ����� ��

Matrix Matrix::operator*(const Matrix& _m) const
{
	Matrix mat;
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {
			mat.m[row][col] = 0.0f;
			for (int i = 0; i < 4; i++) {
				mat.m[row][col] += m[row][i] * _m.m[i][col];
			}
		}
	}

	return mat;
}

// ��İ� ������ ��

/*

�򰥸��ٸ� �Ʒ��� �����ؼ� ����.
�Ϲ����� ��İ��̶� ũ�� �ٸ��� ����.
�Ʒ��� �̵� ��ȯ�� �����̴�.

| 1  0  1 |   | 2 |   =   | 3 |
| 0  1 -2 | * | 3 |   =   | 1 |
| 0  0  1 |   | 1 |       | 1 |

*/

Vec4 Matrix::operator*(const Vec4& v) const
{
	return Vec4(m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
		m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w);
}

void Matrix::Print()
{
	// �̵� ��ȯ �׽�Ʈ

	Matrix trans = Translation(1, -2, 0);
	Vec4 v(2, 3, 0, 1);

	Vec4 res = trans * v;

	// ����� ��忡�� ����� Output���� ����Ͽ� Ȯ��

	char buffer[256];
	sprintf_s(buffer, "x = %.2f, y = %.2f, z = %.2f, w = %.2f\n", res.x, res.y, res.z, res.w);
	OutputDebugStringA(buffer);
}