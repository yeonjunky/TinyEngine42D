#include <windows.h>

// Window dimensions
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

// Window procedure function
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    
    case WM_CLOSE:
        DestroyWindow(hwnd);
        return 0;
    
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            // Fill the window with a dark gray background
            RECT rect;
            GetClientRect(hwnd, &rect);
            HBRUSH hBrush = CreateSolidBrush(RGB(64, 64, 64));
            FillRect(hdc, &rect, hBrush);
            DeleteObject(hBrush);
            
            // Draw some text
            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));
            const char* text = "Tiny42D Engine - 1920x1080 Window";
            TextOutA(hdc, 50, 50, text, static_cast<int>(strlen(text)));
            
            EndPaint(hwnd, &ps);
        }
        return 0;
    }
    
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // Register window class
    const char* CLASS_NAME = "Tiny42DEngineWindow";
    
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    
    if (!RegisterClassA(&wc))
    {
        MessageBoxA(nullptr, "Failed to register window class!", "Error", MB_ICONERROR | MB_OK);
        return -1;
    }
    
    // Calculate window size with borders
    RECT windowRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
    DWORD windowStyle = WS_OVERLAPPEDWINDOW;
    AdjustWindowRect(&windowRect, windowStyle, FALSE);
    
    int adjustedWidth = windowRect.right - windowRect.left;
    int adjustedHeight = windowRect.bottom - windowRect.top;
    
    // Center the window on screen
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenWidth - adjustedWidth) / 2;
    int posY = (screenHeight - adjustedHeight) / 2;
    
    // Create window
    HWND hwnd = CreateWindowExA(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "Tiny42D Engine",               // Window text
        windowStyle,                    // Window style
        posX, posY,                     // Position
        adjustedWidth, adjustedHeight,  // Size
        nullptr,                        // Parent window
        nullptr,                        // Menu
        hInstance,                      // Instance handle
        nullptr                         // Additional application data
    );
    
    if (hwnd == nullptr)
    {
        MessageBoxA(nullptr, "Failed to create window!", "Error", MB_ICONERROR | MB_OK);
        return -1;
    }
    
    // Show and update window
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    // Message loop
    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}