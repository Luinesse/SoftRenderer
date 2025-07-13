#include <windows.h>
#include <iostream>
#include "LA_Matrix.h"
#include "Triangle.h"
#include "RenderingUtils.h"

#define PI 3.14

using namespace std;

// 전역 변수

HBITMAP g_hBitmap;
BYTE* g_pPixels = nullptr;
HDC g_hMemDC;

// 전역 변수

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;


// Z-Buffer 로 사용될 float 배열
// 화면 크기만큼 할당된다.
unique_ptr<float[]> g_pDepthBuffer = nullptr;

// 윈도우 프로시저 함수 선언
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// 더블 버퍼링을 위한 함수 선언
void InitBackBuffer(HWND hWnd, int width, int height);
void Present(HWND hWnd);
void ClearBuffer(BYTE r, BYTE g, BYTE b);
void SetPixel(int x, int y, BYTE r, BYTE g, BYTE b);
void Render(HWND hWnd);

// 삼각형 출력 함수
void DrawTriangle(const Triangle& tri3D, const Vec3& cameraPos, const Matrix& proj, const Matrix& view);

// WinMain: 윈도우 애플리케이션의 시작점
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow) {
    // 윈도우 클래스 정의
    const wchar_t CLASS_NAME[] = L"MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;            // 윈도우 메시지 처리 함수
    wc.hInstance = hInstance;            // 현재 인스턴스
    wc.lpszClassName = CLASS_NAME;       // 클래스 이름
    wc.hbrBackground = (HBRUSH)(WHITE_BRUSH); // 배경 흰색 설정


    RegisterClass(&wc); // 윈도우 클래스 등록

    // 윈도우 생성
    HWND hWnd = CreateWindowEx(
        0,                              // 확장 스타일
        CLASS_NAME,                     // 윈도우 클래스 이름
        L"삼각함수를 이용한 포신 그리기",               // 타이틀 바에 표시될 이름
        WS_OVERLAPPEDWINDOW,           // 윈도우 스타일
        CW_USEDEFAULT, CW_USEDEFAULT,  // 위치
        SCREEN_WIDTH, SCREEN_HEIGHT,                       // 크기
        NULL, NULL, hInstance, NULL
    );

    if (hWnd == NULL) {
        return 0;
    }

    InitBackBuffer(hWnd, SCREEN_WIDTH, SCREEN_HEIGHT);

    SetTimer(hWnd, 1, 16, NULL);

    ShowWindow(hWnd, nCmdShow); // 윈도우 표시

    // 메시지 루프
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    // 키들의 동시입력을 위해 if - else if가 아닌, if로만 구성
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
    case WM_DESTROY: // 윈도우가 닫힐 때
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

// 윈도우 핸들과 해상도를 파라미터로 받는다.
// 백 버퍼를 생성하는 함수
// g_pPixels 포인터를 통해 픽셀을 직접 조작 가능한 백버퍼를 만든다.
// 그 백버퍼를 더블 버퍼링용 DC에 연결한다.
// Present 함수에서 그 백버퍼를 고속복사해서 그림을 그린다.
void InitBackBuffer(HWND hWnd, int width, int height) {
    // DC를 가져온다.
    // DC란 ? Device Context로, GDI나 Bitmap에 뭔가를 그릴 수 있게 해줌.
    HDC hdc = GetDC(hWnd);

    // 비트맵 정보를 담을 구조체 BITMAPINFO
    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);            // bmiHeader의 사이즈.
    bmi.bmiHeader.biWidth = width;                              // 비트맵의 가로 사이즈
    bmi.bmiHeader.biHeight = -height;                           // 비트맵의 세로 사이즈 ( 음수로 설정하면 탑-다운 방식으로, 메모리의 첫 줄이 화면의 위쪽. 즉, 윈도우 좌표계와 일치 )
    bmi.bmiHeader.biPlanes = 1;                                 // Plane의 개수 ( 비트맵은 항상 1개만 사용 )
    bmi.bmiHeader.biBitCount = 32;                              // 1pixel당 몇 비트를 사용할 것인가 ? ( 32비트 설정했으므로, RGBA 각각 8비트 씩해서, 4바이트 )
    bmi.bmiHeader.biCompression = BI_RGB;                       // 압축할것인가? 안함.

    // 실제로 윈도우 DC로 사용되는 것과 호환되는 DC를 하나 더만든다. ( 더블 버퍼링 용 )
    g_hMemDC = CreateCompatibleDC(hdc);
    // 백버퍼로 사용할 DIB 생성
    // DIB란 ? Device Independent Bitmap 으로, 장치에 종속되지 않는 비트맵 이미지.
    // 따라서, 어디에서나 원래 모양대로 출력 가능.
    // 우리가 그릴 픽셀 데이터의 시작주소 g_pPixels 를 넘겨줌. ( 이걸로 픽셀의 RGBA 값 조작 가능 - 실제로 Windows 내에서는 BGRA 순으로 들어간다. )
    g_hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&g_pPixels, NULL, 0);
    // g_hBitmap을 g_hMemDC에 연결한다.
    // g_hMemDC는 그리기를 요청받을 때, g_hBitmap에 그림을 그린다.
    SelectObject(g_hMemDC, g_hBitmap);

    // Z-Buffer 생성.
    g_pDepthBuffer = make_unique<float[]>(SCREEN_WIDTH * SCREEN_HEIGHT);

    // 처음에 사용한 윈도우 DC는 이제 사용하지 않으니 해제한다.
    ReleaseDC(hWnd, hdc);
}

// 백버퍼를 실제 윈도우 화면에 복사.
void Present(HWND hWnd) {
    // 실제 윈도우 화면의 DC를 가져온다.
    HDC hdc = GetDC(hWnd);
    // g_hMemDC의 (0,0) 에서부터 (SCREEN_WIDTH, SCREEN_HEIGHT) 크기만큼 복사
    // 그것을 hdc의 (0, 0)에 고속복사
    // SRCCOPY는 그냥 덮어쓰기 방식. 즉, 방식을 정하는 파라미터
    BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, g_hMemDC, 0, 0, SRCCOPY);
    // 고속복사를 완료했으므로, 사용하지 않으니 해제.
    ReleaseDC(hWnd, hdc);
}

/*
ClearBuffer와 SetPixel 에서 왜 인덱스 접근을 (y * SCREEN_WIDTH + x) * 4 의 형태로 수행하는가 ?
2차원 배열은 실제로 컴퓨터에 저장될때에는 1차원의 형태로 저장된다.
따라서, 우리가 접근하고자 하는 픽셀의 위치는 가로길이 * 원하는 세로 길이 + 우리가 원하는 가로 길이 가 된다.
마지막에 4를 곱하는 이유는 RGBA로, 각 픽셀을 구성하는건 RGBA로 4바이트 이기 때문에, 4바이트 단위로 접근해야한다.
*/

void ClearBuffer(BYTE r, BYTE g, BYTE b) {
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            int idx = (y * SCREEN_WIDTH + x) * 4;
            g_pPixels[idx + 0] = b;
            g_pPixels[idx + 1] = g;
            g_pPixels[idx + 2] = r;
            g_pPixels[idx + 3] = 255;

            // 각 픽셀의 깊이 값 초기화.
            // 아주 큰 값을 통해 어떤 픽셀이 들어와도 갱신 될것임.
            // FLT_MAX인건 아직 아무것도 그려지지 않은 상태.
            g_pDepthBuffer[y * SCREEN_WIDTH + x] = FLT_MAX;
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

    // 화면의 종횡비와 근평면과 원평면 (절두체 시작부분과 끝부분 정도일려나)
    float aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
    float zNear = 1.0f;
    float zFar = 100.0f;

    // 카메라 생성
    Vec3 cameraPos(0, 0, 0);
    Vec3 target(0, 0, 1);
    Vec3 up(0, 1, 0);

    // 뷰 행렬 생성
    Matrix view = Matrix::LookAtLH(cameraPos, target, up);

    // 투영행렬 생성
    Matrix proj = Matrix::PerspectiveFovLH(PI / 2.0f, aspect, zNear, zFar);

    // 삼각뿔 생성을 위한 정점 4개.
    Vec3 v0(0, 1, 4.5);
    Vec3 v1(-1, -1, 5);
    Vec3 v2(1, -1, 5);
    Vec3 v3(0, -1, 4);

    // 삼각뿔의 면 생성
    Triangle face1(v0, v1, v2);
    Triangle face2(v0, v2, v3);
    Triangle face3(v0, v3, v1);
    Triangle face4(v1, v3, v2);

    // 삼각뿔 그리기
    DrawTriangle(face1, cameraPos, proj, view);
    DrawTriangle(face2, cameraPos, proj, view);
    DrawTriangle(face3, cameraPos, proj, view);
    DrawTriangle(face4, cameraPos, proj, view);

    // Z = 5 위치에 삼각형을 그린다.
    // 사용된 좌표는 월드 공간 좌표이다. (3D)
    // Triangle tri(Vec3(0, 1, 5), Vec3(1, -1, 5), Vec3(-1, -1, 5));

    // Z-Buffer 테스트를 위한 삼각형 2개
    // Triangle tri1(Vec3(0, 1, 4), Vec3(1.5f, -0.5f, 7), Vec3(-1, -1, 4));
    // Triangle tri2(Vec3(0.5f, 1.5f, 6), Vec3(1.5f, -0.5f, 6), Vec3(-0.5f, -0.5f, 6));
    // 
    // DrawTriangle(tri2, cameraPos, proj);
    // DrawTriangle(tri1, cameraPos, proj);

    // 여기서 부턴 스크린 좌표에 삼각형 정점을 찍어 렌더링 후, 무게중심 좌표계를 통해 색상 보간 수행.
    /*

    // 만일, Render에서 그리는 내용이 변경될 때를 대비하여 ClearBuffer 함수를 Render 최상단에 바로 실행해준다.
    // 그럼 변경이 있을 때, 이전의 내용을 지우고 다시 그리므로, 애니메이션의 형태를 그릴 수 있다.
    // 이러한 작업을 하지 않았다면, 애니메이션 시 잔상이 남게 될 것.
    ClearBuffer(255, 255, 255);

    // 앞면으로 만들어주기 위해 정점 위치를 바꿔줌.
    Triangle tri(Vec2(300, 100), Vec2(600, 300), Vec2(100, 300));

    if (!tri.IsFrontFacing())    return;

    tri.SetBoundingBox(SCREEN_WIDTH, SCREEN_HEIGHT);

    for (int x = tri.minX; x <= tri.maxX; x++) {
        for (int y = tri.minY; y <= tri.maxY; y++) {
            Vec2 point(static_cast<float>(x), static_cast<float>(y));



            // 점 내부 판별 및 바운딩 박스 확인 코드

            // point가 삼각형 내부에 있는지 확인하고 있다면 파란색, 아니라면 빨간색으로 출력
            // 이를 통해 바운딩박스의 크기 확인 가능
            // if (tri.IsInTriangle(point)) SetPixel(x, y, 0, 0, 255);
            // else SetPixel(x, y, 255, 0, 0);



            if (tri.IsInTriangle(point)) {
                // 해당 정점의 위치가 삼각형 내부에서 어느 정점에 가중치가 더 높은가
                tri.ComputeBarycentric(point);
                // 얻어낸 가중치로 색상 칠하기.
                SetPixel(x, y, 255 * point.r, 255 * point.g, 255 * point.b);
            }
        }
    }

    SetPixel(300, 100, 255, 0, 0);
    SetPixel(100, 300, 255, 0, 0);
    SetPixel(600, 300, 255, 0, 0);

    */
}

void DrawTriangle(const Triangle& tri3D, const Vec3& cameraPos, const Matrix& proj, const Matrix& view) {
    // 카메라와 삼각형이 마주보는지 확인한다. (Back-face Culling)
    if (!tri3D.IsFrontFacing(cameraPos))   return;

    // 클립 공간의 정점 생성 (클립 공간으로 변환)
    // 클립 공간에서는 클리핑을 수행해야하지만 아직은 생략하고 넘어간다.
    // 언제 클리핑을 하는가 ? EX) 삼각형의 정점들이 절두체의 원평면보다 뒤에 있다면 ? 그리지 않는다. 혹은 근평면보다 가깝다면 ? 그리지 않는다. 좌우로 벗어나도 똑같다.

    // 뷰 행렬을 먼저 곱하고 그 결과에 투영 행렬이 곱해져야 하므로, 순서는 투영 행렬 * 뷰 행렬 * 모델
    Vec4 p0 = proj * view * Vec4(tri3D.vt0, 1.0f);
    Vec4 p1 = proj * view * Vec4(tri3D.vt1, 1.0f);
    Vec4 p2 = proj * view * Vec4(tri3D.vt2, 1.0f);

    // 원근 나눗셈을 통한 Vec3 변환
    // NDC 좌표를 얻게된다.
    Vec3 ndc0 = PerspectiveDivide(p0);
    Vec3 ndc1 = PerspectiveDivide(p1);
    Vec3 ndc2 = PerspectiveDivide(p2);

    // 뷰포트 변환을 통해 NDC좌표를 2D 스크린 좌표로 변환
    Triangle tri;
    tri.v0 = ViewportTransform(ndc0, SCREEN_WIDTH, SCREEN_HEIGHT);
    tri.v1 = ViewportTransform(ndc1, SCREEN_WIDTH, SCREEN_HEIGHT);
    tri.v2 = ViewportTransform(ndc2, SCREEN_WIDTH, SCREEN_HEIGHT);

    // 바운딩 박스 생성 (AABB를 이용한 2D좌표에서의 단순클리핑)
    tri.SetBoundingBox(SCREEN_WIDTH, SCREEN_HEIGHT);

    // 스크린 좌표의 픽셀이 삼각형 내부에 존재하는지 확인 후 무게 중심 좌표계를 통하여 색상 보간 색칠.
    for (int x = tri.minX; x <= tri.maxX; x++) {
        for (int y = tri.minY; y <= tri.maxY; y++) {
            Vec2 point(static_cast<float>(x), static_cast<float>(y));
            if (tri.IsInTriangle(point)) {
                tri.ComputeBarycentric(point);

                float a = point.r;
                float b = point.g;
                float c = point.b;

                // z값을 무게중심좌표를 통해 보간한다.
                // 이렇게 해주어야 각 정점의 z값에 알맞게 적절히 z-buffer 역할이 수행될것이다.
                // 가령, 삼각형의 정점 중 하나가 뒤의 삼각형에 겹치게끔 되어 있다면, 그 겹치는 지점부터는 뒤의 삼각형이 보여야할것.
                // z값 보간을 하지 않는다면, 겹치는 지점부터 뒤의 삼각형이 보이는게 아닌 그냥 뒤에 삼각형이 가려지게 될것이다.
                // 해당 코드를 그대로 실행하면 겹쳐지는 부분부터 가려지는걸 확인가능하다.
                // 아래 코드에서 if문에 ndc0.z 로 넣고, g_pDepthBuffer[idx]에 ndc0.z 로 넣어 보간을 수행하지 않도록 해보면
                // 위의 주석의 말이 이해될것이다.
                float z = ndc0.z * a + ndc1.z * b + ndc2.z * c;

                int idx = y * SCREEN_WIDTH + x;
                if (z < g_pDepthBuffer[idx]) {
                    g_pDepthBuffer[idx] = z;
                    SetPixel(x, y, 255 * a, 255 * b, 255 * c);
                }
            }
        }
    }
}