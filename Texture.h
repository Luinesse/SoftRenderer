#pragma once
#include <Windows.h>
#include <vector>

class Texture {
public:
	int width = 0;
	int height = 0;
	std::vector<COLORREF> pixels;

	// BMP 이미지 로드해서 pixels 벡터에 색상값을 채워줄 함수
	bool LoadFromBMP(const wchar_t* filename);
	// uv좌표를 스크린좌표로 변환
	COLORREF Sample(float u, float v) const;
};