#include "LA_Matrix.h"
#include <iostream>
#include <Windows.h>

using namespace std;

Matrix::Matrix()
{
	// 행렬의 초기화는 단위 행렬로 초기화 한다. (대각성분이 1인 행렬)
	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 4; k++) {
			// 삼항연산자를 통해 현재 인덱스가 대각성분인지 확인 후 값 대입
			m[i][k] = (i == k) ? 1.0f : 0.0f;
		}
	}
}

// 단위 행렬을 리턴한다.
Matrix Matrix::Identity()
{
	Matrix mat;

	return mat;
}

// 회전, 크기, 전단은 선형변환이라서 3X3 행렬의 행렬곱으로 표현이 가능했다.
// 이동은 선형성을 띄지 못하여 행렬곱으로 표현이 불가능하다.
// 따라서, 차원을 1차원 늘려서 수행함. (동차좌표계)
// 그 결과 회전, 크기, 이동, 전단을 모두 행렬곱으로 통합 가능.

// 선형성이란 ? 가법성과 1차 동차성을 만족하는 걸 의미한다.
// 가법성이란 ? f(x1 + x2) = f(x1) + f(x2)
// 1차 동차성이란 ? f(k * x) = k * f(x)

/*

이동 변환에서 선형성을 만족하기 위해 동차 좌표계를 이용하는데
이렇게 축을 한개 늘려서 마지막 축을 원점과의 연결 고리로 활용한다. (3차원 벡터 x,y,z 를 4차원 벡터 x,y,z,1로. 3차원 행렬을 4차원 행렬로.) (즉, 벡터의 w 값이 1로 고정됨.)
만들어진 4X4 행렬과 4차원 벡터의 행렬곱으로 이동 변환이 정상적으로 수행가능해진다.

*/

Matrix Matrix::Translation(float tx, float ty, float tz)
{
	// 생성자에 의해 단위행렬로 초기화 됨. 이하 함수들도 동일.
	Matrix mat;
	// 각각 x축이동, y축이동, z축이동
	// 4X4 행렬의 오른쪽 마지막 열에 이동값을 넣어줌.
	// 이 행렬을 이동을 원하는 벡터와 행렬곱 수행하면, 이동이 완료된 벡터를 얻을 수 있다.
	mat.m[0][3] = tx;
	mat.m[1][3] = ty;
	mat.m[2][3] = tz;

	return mat;
}

// 스케일을 아핀 변환 크기 행렬로 표현할때에는 대각 성분의 값을 조절하여 수행.

Matrix Matrix::Scale(float sx, float sy, float sz)
{
	Matrix mat;

	mat.m[0][0] = sx;
	mat.m[1][1] = sy;
	mat.m[2][2] = sz;

	return mat;
}

// 회전 변환은 축에 따라 다르게 표현

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

// 두 행렬의 곱

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

// 행렬과 벡터의 곱

/*

헷갈린다면 아래걸 대입해서 보자.
일반적인 행렬곱이랑 크게 다를건 없다.
아래는 이동 변환의 예시이다.

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
	// 이동 변환 테스트

	Matrix trans = Translation(1, -2, 0);
	Vec4 v(2, 3, 0, 1);

	Vec4 res = trans * v;

	// 디버그 모드에서 결과를 Output으로 출력하여 확인

	char buffer[256];
	sprintf_s(buffer, "x = %.2f, y = %.2f, z = %.2f, w = %.2f\n", res.x, res.y, res.z, res.w);
	OutputDebugStringA(buffer);
}
