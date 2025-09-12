#include <windows.h>
#include <iostream>
#include <memory>


// 윈도우 크기
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 960;

// 윈도우 프로시저 함수 선언
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// 콘솔창이 있으면 디버깅에 편리합니다.
// 디버깅할 때 애매한 값들을 cout으로 출력해서 확인해보세요.
int main()
{

    const int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;

    WNDCLASSEX wc = {sizeof(WNDCLASSEX),
                     CS_CLASSDC,
                     WndProc,
                     0L,
                     0L,
                     GetModuleHandle(NULL),
                     NULL,
                     LoadCursor(nullptr, IDC_ARROW),
                     NULL,
                     NULL,
                     L"Tiny42DEngine", // 창 이름 string 
                     NULL};

    RegisterClassEx(&wc);

    // 우리가 원하는 그림이 그려질 부분의 해상도
    RECT wr = {0, 0, width, height};

    // 윈도우 스타일 명시적으로 정의 (최소화, 최대화, 닫기 버튼 포함)
    DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    // WS_OVERLAPPEDWINDOW는 다음을 포함합니다:
    // WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX

    // 필요한 윈도우 크기(해상도) 계산
    // wr의 값이 바뀜
    AdjustWindowRect(&wr, windowStyle, FALSE);

    // 윈도우를 화면 중앙에 배치하기 위한 계산
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - (wr.right - wr.left)) / 2;
    int posY = (screenHeight - (wr.bottom - wr.top)) / 2;

    // 윈도우를 만들때 위에서 계산한 wr과 명시적 스타일 사용
    HWND hwnd = CreateWindow(wc.lpszClassName, L"Tiny42D Engine",
                             windowStyle,
                             posX, // 윈도우 좌측 상단의 x 좌표 (중앙 배치)
                             posY, // 윈도우 좌측 상단의 y 좌표 (중앙 배치)
                             wr.right - wr.left, // 윈도우 가로 방향 해상도
                             wr.bottom - wr.top, // 윈도우 세로 방향 해상도
                             NULL, NULL, wc.hInstance, NULL);

    if (!hwnd) {
        std::cerr << "윈도우 생성에 실패했습니다!" << std::endl;
        return -1;
    }

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    std::cout << "Tiny42D Engine이 시작되었습니다." << std::endl;
    std::cout << "윈도우 크기: " << width << "x" << height << std::endl;

    // 메인 메시지 루프
    MSG msg = {};
    while (WM_QUIT != msg.message)
    {
        // 메시지 처리만 합니다
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        // 실제 게임 로직 구현은 여기에 하시면 됩니다.
        else
        {
              
        }
    }

    DestroyWindow(hwnd);
    UnregisterClass(wc.lpszClassName, wc.hInstance);

    std::cout << "Tiny42D Engine이 종료되었습니다." << std::endl;
    return 0;
}

// 윈도우 프로시저 함수
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_SIZE:
        // 윈도우 크기 변경 시 처리
        if (wParam == SIZE_MINIMIZED) {
            std::cout << "윈도우가 최소화되었습니다" << std::endl;
        } else if (wParam == SIZE_MAXIMIZED) {
            std::cout << "윈도우가 최대화되었습니다" << std::endl;
        } else if (wParam == SIZE_RESTORED) {
            std::cout << "윈도우가 복원되었습니다" << std::endl;
        } else {
            std::cout << "윈도우 크기 변경됨" << std::endl;
        }
        return 0;
    case WM_SYSCOMMAND:
        // 시스템 명령 처리 (최소화, 최대화, 닫기 등)
        switch (wParam & 0xfff0) {
        case SC_MINIMIZE:
            std::cout << "최소화 버튼 클릭" << std::endl;
            break;
        case SC_MAXIMIZE:
            std::cout << "최대화 버튼 클릭" << std::endl;
            break;
        case SC_RESTORE:
            std::cout << "복원 버튼 클릭" << std::endl;
            break;
        case SC_CLOSE:
            std::cout << "닫기 버튼 클릭" << std::endl;
            break;
        case SC_KEYMENU: // ALT 애플리케이션 메뉴 비활성화
            return 0;
        }
        break;
    case WM_MOUSEMOVE:
        break;
    case WM_LBUTTONUP:
        std::cout << "왼쪽 마우스 버튼 클릭" << std::endl;
        break;
    case WM_RBUTTONUP:
        std::cout << "오른쪽 마우스 버튼 클릭" << std::endl;
        break;
    case WM_KEYDOWN:
        std::cout << "키 입력: " << (int)wParam << std::endl;
        if (wParam == VK_ESCAPE) {
            PostQuitMessage(0);
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            // 윈도우를 어두운 회색 배경으로 채우기
            RECT rect;
            GetClientRect(hWnd, &rect);
            HBRUSH hBrush = CreateSolidBrush(RGB(64, 64, 64));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            
            // 텍스트 그리기
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            const wchar_t* text = L"Tiny42D Engine - 게임 엔진 개발 중...";
            TextOut(hdc, 50, 50, text, wcslen(text));
            
            const wchar_t* instruction = L"ESC 키를 눌러 종료하세요";
            TextOut(hdc, 50, 80, instruction, wcslen(instruction));
            
            EndPaint(hWnd, &ps);
        }
        return 0;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }

    return ::DefWindowProc(hWnd, msg, wParam, lParam);
}