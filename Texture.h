#pragma once
#include <Windows.h>
#include <vector>

class Texture {
public:
	int width = 0;
	int height = 0;
	std::vector<COLORREF> pixels;

	// BMP �̹��� �ε��ؼ� pixels ���Ϳ� ������ ä���� �Լ�
	bool LoadFromBMP(const wchar_t* filename);
	// uv��ǥ�� ��ũ����ǥ�� ��ȯ
	COLORREF Sample(float u, float v) const;
};