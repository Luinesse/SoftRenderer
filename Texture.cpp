#include "Texture.h"

bool Texture::LoadFromBMP(const wchar_t* filename)
{
	// ���� ��η� ���� ��Ʈ�� ���ҽ� �̹����� ���� ũ��� ���� �ý������� �ε��Ѵ�.
	HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// �����ϸ� false ����
	if (!hBitmap)	return false;

	// hBitmap �� ��Ÿ�����͸� bmp ����ü�� ä���.
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	// �̹����� ����, ���� �ȼ���
	width = bmp.bmWidth;
	height = bmp.bmHeight;

	// ��Ʈ�� �� �ȼ��� ��Ʈ.
	int bpp = bmp.bmBitsPixel;
	// ����Ʈ ������ ��ȯ.
	int bytesPerPixel = bpp / 8;

	// �ӽ� ���۷�, �̹��� ��ü �ȼ��� * �ȼ� �� ����Ʈ ���� ä��.
	// ���⿡ BGR ������ ����Ʈ ������ ä����.
	std::vector<BYTE> raw(width * height * bytesPerPixel);

	// ����� InitBackBuffer() �� ��������.
	HDC hdc = GetDC(NULL);
	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	// �̰� InitBackBuffer() �� �ٸ���, �츮�� ���� �ȼ� �� ����Ʈ�� ����Ұ��̴�. (24bit���� 32bit���� �𸣴ϱ�)
	bmi.bmiHeader.biBitCount = bpp;
	bmi.bmiHeader.biCompression = BI_RGB;

	// ���� �ȼ� ������(hBitmap�� ��ü)�� raw.data() �� ����.
	GetDIBits(hdc, hBitmap, 0, height, raw.data(), &bmi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);
	DeleteObject(hBitmap);

	// pixels�� ũ�⸦ ȭ�� �ȼ� ũ�⸸ŭ ������
	pixels.resize(width * height);
	// ��ü �ȼ��� ��ȸ�ϸ� ���� �����Ͱ� (raw) �� �����ͼ� pixels�� ��ȯ
	for (int i = 0, p = 0; i < width * height; i++, p += bytesPerPixel) {
		// RGB���� �����ðŴϱ� 0,1,2 (A�� �ִٸ� 3����)
		BYTE b = raw[p + 0];
		BYTE g = raw[p + 1];
		BYTE r = raw[p + 2];
		// ��ȯ
		pixels[i] = RGB(r, g, b);
	}

	return true;
}

COLORREF Texture::Sample(float u, float v) const
{
	// uv�� 0~1 ���� ������ ������ �ʵ��� Ŭ����
	u = max(0.0f, min(1.0f, u));
	v = max(0.0f, min(1.0f, v));

	// uv��ǥ�� ��ũ�� ��ǥ�� ��ȯ
	int x = static_cast<int>(u * (width - 1));
	int y = static_cast<int>(v * (height - 1));
	return pixels[y * width + x];
}
