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

Matrix Matrix::PerspectiveFovLH(float fovY, float aspect, float zNear, float zFar)
{
	Matrix mat;

	float yScale = 1.0f / tanf(fovY * 0.5f);
	float xScale = yScale / aspect;

	// 왼손 좌표계 기준 4 By 4 투영 행렬 초기화

	mat.m[0][0] = xScale;								// 종횡비에 따라 x 방향 비율 정함. (xScale)
	mat.m[1][1] = yScale;								// 수직 시야각 / 2 의 tan값으로 화면의 절반 높이가 원점(카메라)로 부터 얼마나 떨어져있는지 확인. 그에 따라 시야가 좁아지거나 넓어짐. (카메라의 줌과 비슷하다고 한다)
	mat.m[2][2] = zFar / (zFar - zNear);				// 카메라 기준 가장 가까운 근평면과 가장 먼 평면간의 깊이 값을 0~1 범위로 정규화.
	mat.m[2][3] = (-zNear * zFar) / (zFar - zNear);		// z값이 zNear와 같다면, 0 zFar와 같다면 1로 정규화해준다.
	mat.m[3][2] = 1.0f;									// 투영행렬을 거치면 4차원 벡터가 만들어지는데 거기서 w값이 정해지는데 역할을함. 이렇게 해주어야 이 w값이 z값과 연관이 생겨 원근감이 생긴다. (원근 나눗셈을 수행하니까)
	mat.m[3][3] = 0.0f;									// 0이어야 w 분모가 생겨서 원근나눗셈 적용가능. 1이라면 직교투영 (Orthographic)

	return mat;
}

Matrix Matrix::LookAtLH(const Vec3& eye, const Vec3& target, const Vec3& up)
{
	// 카메라가 타깃을 바라보는 방향 (전방)
	// A -> B 방향 벡터는 B - A 로 구할 수 있었다.
	Vec3 zAxis = (target - eye).Normalize();

	// 오른쪽 방향 (왼손 좌표계이므로 x축은 오른쪽이다.)
	// 카메라의 위쪽 방향 벡터와 위에서 구한 전방방향 벡터의 외적으로 구해진다.
	// 두 벡터의 외적은 두 벡터에 수직한 벡터를 구한다.
	Vec3 xAxis = up.Cross(zAxis).Normalize();

	// 위쪽 방향
	// 벡터 외적으로 두 벡터에 수직한 벡터를 구한다.
	Vec3 yAxis = zAxis.Cross(xAxis);          

	// 위 과정으로 카메라를 기준으로 하는 새로운 직교 좌표계를 만든다. (기즈모 같은 느낌?)

	Matrix view;

	// 열 기준으로 구현됨. (DirectX. OpenGL 기준으로는 해당 행렬을 전치연산 수행하면 된다.)
	// 1열 (Right)
	// x축의 회전 정보 (0~2행)
	// x축의 이동 정보 (3행)
	// 이동 정보는 카메라와 뷰 공간에 정의한 x축과의 내적을 통해 투영 벡터의 길이를 구한다. (3D 공간에서의 Back-face Culling 할때를 떠올려보자)
	// 구한 투영 벡터 길이만큼 뺄셈을 수행하여 카메라가 원점에 있게끔 한다. (뷰 행렬에서 물체가 움직이는건 카메라가 원점에 있고, 공간이 움직이는 형태가 될테니까)
	view.m[0][0] = xAxis.x;
	view.m[1][0] = xAxis.y;
	view.m[2][0] = xAxis.z;
	view.m[3][0] = -xAxis.Dot(eye);

	// 2열 (Up)
	view.m[0][1] = yAxis.x;
	view.m[1][1] = yAxis.y;
	view.m[2][1] = yAxis.z;
	view.m[3][1] = -yAxis.Dot(eye);

	// 3열 (Forward)
	view.m[0][2] = zAxis.x;
	view.m[1][2] = zAxis.y;
	view.m[2][2] = zAxis.z;
	view.m[3][2] = -zAxis.Dot(eye);

	// 4열
	// 고정값.
	view.m[0][3] = 0.0f;
	view.m[1][3] = 0.0f;
	view.m[2][3] = 0.0f;
	view.m[3][3] = 1.0f;

	return view;
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