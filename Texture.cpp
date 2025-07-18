#include "Texture.h"

bool Texture::LoadFromBMP(const wchar_t* filename)
{
	// 파일 경로로 가서 비트맵 리소스 이미지를 원본 크기로 파일 시스템으로 로드한다.
	HBITMAP hBitmap = (HBITMAP)LoadImageW(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// 실패하면 false 리턴
	if (!hBitmap)	return false;

	// hBitmap 의 메타데이터를 bmp 구조체에 채운다.
	BITMAP bmp;
	GetObject(hBitmap, sizeof(BITMAP), &bmp);

	// 이미지의 가로, 세로 픽셀수
	width = bmp.bmWidth;
	height = bmp.bmHeight;

	// 비트맵 한 픽셀당 비트.
	int bpp = bmp.bmBitsPixel;
	// 바이트 단위로 변환.
	int bytesPerPixel = bpp / 8;

	// 임시 버퍼로, 이미지 전체 픽셀수 * 픽셀 당 바이트 수로 채움.
	// 여기에 BGR 순으로 바이트 데이터 채워짐.
	std::vector<BYTE> raw(width * height * bytesPerPixel);

	// 여기는 InitBackBuffer() 를 참고하자.
	HDC hdc = GetDC(NULL);
	BITMAPINFO bmi = {};
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = -height;
	bmi.bmiHeader.biPlanes = 1;
	// 이건 InitBackBuffer() 랑 다른데, 우리가 구한 픽셀 당 바이트를 사용할것이다. (24bit일지 32bit일지 모르니까)
	bmi.bmiHeader.biBitCount = bpp;
	bmi.bmiHeader.biCompression = BI_RGB;

	// 실제 픽셀 데이터(hBitmap의 전체)를 raw.data() 에 복사.
	GetDIBits(hdc, hBitmap, 0, height, raw.data(), &bmi, DIB_RGB_COLORS);
	ReleaseDC(NULL, hdc);
	DeleteObject(hBitmap);

	// pixels의 크기를 화면 픽셀 크기만큼 재조정
	pixels.resize(width * height);
	// 전체 픽셀을 순회하며 원시 데이터값 (raw) 를 가져와서 pixels로 변환
	for (int i = 0, p = 0; i < width * height; i++, p += bytesPerPixel) {
		// RGB값을 가져올거니까 0,1,2 (A가 있다면 3까지)
		BYTE b = raw[p + 0];
		BYTE g = raw[p + 1];
		BYTE r = raw[p + 2];
		// 변환
		pixels[i] = RGB(r, g, b);
	}

	return true;
}

COLORREF Texture::Sample(float u, float v) const
{
	// uv가 0~1 범위 밖으로 나가지 않도록 클램핑
	u = max(0.0f, min(1.0f, u));
	v = max(0.0f, min(1.0f, v));

	// uv좌표를 스크린 좌표로 변환
	int x = static_cast<int>(u * (width - 1));
	int y = static_cast<int>(v * (height - 1));
	return pixels[y * width + x];
}
