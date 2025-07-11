#include <windows.h>
#include <iostream>
#include "LA_Matrix.h"
#include "Triangle.h"
#include "RenderingUtils.h"

#define PI 3.14

using namespace std;

// ���� ����

HBITMAP g_hBitmap;
BYTE* g_pPixels = nullptr;
HDC g_hMemDC;

// ���� ����

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// ������ ���ν��� �Լ� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// ���� ���۸��� ���� �Լ� ����
void InitBackBuffer(HWND hWnd, int width, int height);
void Present(HWND hWnd);
void ClearBuffer(BYTE r, BYTE g, BYTE b);
void SetPixel(int x, int y, BYTE r, BYTE g, BYTE b);
void Render(HWND hWnd);



// WinMain: ������ ���ø����̼��� ������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    // ������ Ŭ���� ����
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;            // ������ �޽��� ó�� �Լ�
    wc.hInstance = hInstance;            // ���� �ν��Ͻ�
    wc.lpszClassName = CLASS_NAME;       // Ŭ���� �̸�
    wc.hbrBackground = (HBRUSH)(WHITE_BRUSH); // ��� ��� ����


    RegisterClass(&wc); // ������ Ŭ���� ���

    // ������ ����
    HWND hWnd = CreateWindowEx(
        0,                              // Ȯ�� ��Ÿ��
        CLASS_NAME,                     // ������ Ŭ���� �̸�
        L"�ﰢ�Լ��� �̿��� ���� �׸���",               // Ÿ��Ʋ �ٿ� ǥ�õ� �̸�
        WS_OVERLAPPEDWINDOW,           // ������ ��Ÿ��
        CW_USEDEFAULT, CW_USEDEFAULT,  // ��ġ
        SCREEN_WIDTH, SCREEN_HEIGHT,                       // ũ��
        NULL, NULL, hInstance, NULL
    );

    if (hWnd == NULL) {
        return 0;
    }

    InitBackBuffer(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

    SetTimer(hWnd, 1, 16, NULL);

    ShowWindow(hWnd, nCmdShow); // ������ ǥ��

    // �޽��� ����
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// �޽��� ó�� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // Ű���� �����Է��� ���� if - else if�� �ƴ�, if�θ� ����
    switch (message) {
    case WM_TIMER: {
        InvalidateRect(hWnd, nullptr, FALSE);
        return 0;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        Render(hWnd);

        Present(hWnd);

        EndPaint(hWnd, &ps);

        return 0;
    }
    case WM_DESTROY: // �����찡 ���� ��
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// ������ �ڵ�� �ػ󵵸� �Ķ���ͷ� �޴´�.
// �� ���۸� �����ϴ� �Լ�
// g_pPixels �����͸� ���� �ȼ��� ���� ���� ������ ����۸� �����.
// �� ����۸� ���� ���۸��� DC�� �����Ѵ�.
// Present �Լ����� �� ����۸� ��Ӻ����ؼ� �׸��� �׸���.
void InitBackBuffer(HWND hWnd, int width, int height) {
    // DC�� �����´�.
    // DC�� ? Device Context��, GDI�� Bitmap�� ������ �׸� �� �ְ� ����.
    HDC hdc = GetDC(hWnd);

    // ��Ʈ�� ������ ���� ����ü BITMAPINFO
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);            // bmiHeader�� ������.
    bmi.bmiHeader.biWidth = width;                              // ��Ʈ���� ���� ������
    bmi.bmiHeader.biHeight = -height;                           // ��Ʈ���� ���� ������ ( ������ �����ϸ� ž-�ٿ� �������, �޸��� ù ���� ȭ���� ����. ��, ������ ��ǥ��� ��ġ )
    bmi.bmiHeader.biPlanes = 1;                                 // Plane�� ���� ( ��Ʈ���� �׻� 1���� ��� )
    bmi.bmiHeader.biBitCount = 32;                              // 1pixel�� �� ��Ʈ�� ����� ���ΰ� ? ( 32��Ʈ ���������Ƿ�, RGBA ���� 8��Ʈ ���ؼ�, 4����Ʈ )
    bmi.bmiHeader.biCompression = BI_RGB;                       // �����Ұ��ΰ�? ����.

    // ������ ������ DC�� ���Ǵ� �Ͱ� ȣȯ�Ǵ� DC�� �ϳ� �������. ( ���� ���۸� �� )
    g_hMemDC = CreateCompatibleDC(hdc);
    // ����۷� ����� DIB ����
    // DIB�� ? Device Independent Bitmap ����, ��ġ�� ���ӵ��� �ʴ� ��Ʈ�� �̹���.
    // ����, ��𿡼��� ���� ����� ��� ����.
    // �츮�� �׸� �ȼ� �������� �����ּ� g_pPixels �� �Ѱ���. ( �̰ɷ� �ȼ��� RGBA �� ���� ���� - ������ Windows �������� BGRA ������ ����. )
    g_hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&g_pPixels, NULL, 0);
    // g_hBitmap�� g_hMemDC�� �����Ѵ�.
    // g_hMemDC�� �׸��⸦ ��û���� ��, g_hBitmap�� �׸��� �׸���.
    SelectObject(g_hMemDC, g_hBitmap);

    // ó���� ����� ������ DC�� ���� ������� ������ �����Ѵ�.
    ReleaseDC(hWnd, hdc);
}

// ����۸� ���� ������ ȭ�鿡 ����.
void Present(HWND hWnd) {
    // ���� ������ ȭ���� DC�� �����´�.
    HDC hdc = GetDC(hWnd);
    // g_hMemDC�� (0,0) �������� (SCREEN_WIDTH, SCREEN_HEIGHT) ũ�⸸ŭ ����
    // �װ��� hdc�� (0, 0)�� ��Ӻ���
    // SRCCOPY�� �׳� ����� ���. ��, ����� ���ϴ� �Ķ����
    BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, g_hMemDC, 0, 0, SRCCOPY);
    // ��Ӻ��縦 �Ϸ������Ƿ�, ������� ������ ����.
    ReleaseDC(hWnd, hdc);
}

/*
ClearBuffer�� SetPixel ���� �� �ε��� ������ (y * SCREEN_WIDTH + x) * 4 �� ���·� �����ϴ°� ?
2���� �迭�� ������ ��ǻ�Ϳ� ����ɶ����� 1������ ���·� ����ȴ�.
����, �츮�� �����ϰ��� �ϴ� �ȼ��� ��ġ�� ���α��� * ���ϴ� ���� ���� + �츮�� ���ϴ� ���� ���� �� �ȴ�.
�������� 4�� ���ϴ� ������ RGBA��, �� �ȼ��� �����ϴ°� RGBA�� 4����Ʈ �̱� ������, 4����Ʈ ������ �����ؾ��Ѵ�.
*/

void ClearBuffer(BYTE r, BYTE g, BYTE b) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            int idx = (y * SCREEN_WIDTH + x) * 4;
            g_pPixels[idx + 0] = b;
            g_pPixels[idx + 1] = g;
            g_pPixels[idx + 2] = r;
            g_pPixels[idx + 3] = 255;
        }
    }
}

void SetPixel(int x, int y, BYTE r, BYTE g, BYTE b) {
    if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)  return;
    int idx = (y * SCREEN_WIDTH + x) * 4;
    g_pPixels[idx + 0] = b;
    g_pPixels[idx + 1] = g;
    g_pPixels[idx + 2] = r;
    g_pPixels[idx + 3] = 255;
}

void Render(HWND hWnd) {
    ClearBuffer(255, 255, 255);

    // Z = 5 ��ġ�� �ﰢ���� �׸���.
    // ���� ��ǥ�� ���� ���� ��ǥ�̴�. (3D)
    Triangle tri(Vec3(0, 1, 5), Vec3(1, -1, 7), Vec3(-1, -1, 3));

    Vec3 cameraPos(0, 0, 0);
    // ī�޶�� �ﰢ���� ���ֺ����� Ȯ���Ѵ�. (Back-face Culling)
    if (!tri.IsFrontFacing(cameraPos))   return;

    // ī�޶��� ��Ⱦ��� ������ ����� (����ü ���ۺκа� ���κ� �����Ϸ���)
    float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    float zNear = 1.0f;
    float zFar = 100.0f;
    // ������� ����
    Matrix proj = Matrix::PerspectiveFovLH(PI / 2.0f, aspect, zNear, zFar);

    // Ŭ�� ������ ���� ���� (Ŭ�� �������� ��ȯ)
    // Ŭ�� ���������� Ŭ������ �����ؾ������� ������ �����ϰ� �Ѿ��.
    // ���� Ŭ������ �ϴ°� ? EX) �ﰢ���� �������� ����ü�� ����麸�� �ڿ� �ִٸ� ? �׸��� �ʴ´�. Ȥ�� ����麸�� �����ٸ� ? �׸��� �ʴ´�. �¿�� ����� �Ȱ���.
    Vec4 p0 = proj * Vec4(tri.vt0, 1.0f);
    Vec4 p1 = proj * Vec4(tri.vt1, 1.0f);
    Vec4 p2 = proj * Vec4(tri.vt2, 1.0f);

    // ���� �������� ���� Vec3 ��ȯ
    // NDC ��ǥ�� ��Եȴ�.
    Vec3 ndc0 = PerspectiveDivide(p0);
    Vec3 ndc1 = PerspectiveDivide(p1);
    Vec3 ndc2 = PerspectiveDivide(p2);

    // ����Ʈ ��ȯ�� ���� NDC��ǥ�� 2D ��ũ�� ��ǥ�� ��ȯ
    tri.v0 = ViewportTransform(ndc0, SCREEN_WIDTH, SCREEN_HEIGHT);
    tri.v1 = ViewportTransform(ndc1, SCREEN_WIDTH, SCREEN_HEIGHT);
    tri.v2 = ViewportTransform(ndc2, SCREEN_WIDTH, SCREEN_HEIGHT);

    // �ٿ�� �ڽ� ���� (AABB�� �̿��� 2D��ǥ������ �ܼ�Ŭ����)
    tri.SetBoundingBox(SCREEN_WIDTH, SCREEN_HEIGHT);

    // ��ũ�� ��ǥ�� �ȼ��� �ﰢ�� ���ο� �����ϴ��� Ȯ�� �� ���� �߽� ��ǥ�踦 ���Ͽ� ���� ���� ��ĥ.
    for (int x = tri.minX; x <= tri.maxX; x++) {
        for (int y = tri.minY; y <= tri.maxY; y++) {
            Vec2 point(static_cast<float>(x), static_cast<float>(y));
            if (tri.IsInTriangle(point)) {
                tri.ComputeBarycentric(point);
                SetPixel(x, y, 255 * point.r, 255 * point.g, 255 *point.b);
            }
        }
    }

    // ���⼭ ���� ��ũ�� ��ǥ�� �ﰢ�� ������ ��� ������ ��, �����߽� ��ǥ�踦 ���� ���� ���� ����.
    /*

    // ����, Render���� �׸��� ������ ����� ���� ����Ͽ� ClearBuffer �Լ��� Render �ֻ�ܿ� �ٷ� �������ش�.
    // �׷� ������ ���� ��, ������ ������ ����� �ٽ� �׸��Ƿ�, �ִϸ��̼��� ���¸� �׸� �� �ִ�.
    // �̷��� �۾��� ���� �ʾҴٸ�, �ִϸ��̼� �� �ܻ��� ���� �� ��.
    ClearBuffer(255, 255, 255);

    // �ո����� ������ֱ� ���� ���� ��ġ�� �ٲ���.
    Triangle tri(Vec2(300, 100), Vec2(600, 300), Vec2(100, 300));

    if (!tri.IsFrontFacing())    return;

    tri.SetBoundingBox(SCREEN_WIDTH, SCREEN_HEIGHT);

    for (int x = tri.minX; x <= tri.maxX; x++) {
        for (int y = tri.minY; y <= tri.maxY; y++) {
            Vec2 point(static_cast<float>(x), static_cast<float>(y));

            
            
            // �� ���� �Ǻ� �� �ٿ�� �ڽ� Ȯ�� �ڵ�

            // point�� �ﰢ�� ���ο� �ִ��� Ȯ���ϰ� �ִٸ� �Ķ���, �ƴ϶�� ���������� ���
            // �̸� ���� �ٿ���ڽ��� ũ�� Ȯ�� ����
            // if (tri.IsInTriangle(point)) SetPixel(x, y, 0, 0, 255);
            // else SetPixel(x, y, 255, 0, 0);
            
            

            if (tri.IsInTriangle(point)) {
                // �ش� ������ ��ġ�� �ﰢ�� ���ο��� ��� ������ ����ġ�� �� ������
                tri.ComputeBarycentric(point);
                // �� ����ġ�� ���� ĥ�ϱ�.
                SetPixel(x, y, 255 * point.r, 255 * point.g, 255 * point.b);
            }
        }
    }

    SetPixel(300, 100, 255, 0, 0);
    SetPixel(100, 300, 255, 0, 0);
    SetPixel(600, 300, 255, 0, 0);

    */
}