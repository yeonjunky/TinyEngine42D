#ifdef _WIN32

#include "platform.hpp"
#include <iostream>
#include <stdexcept>

namespace tiny {

class PlatformWin32 : public Platform {
public:
  ~PlatformWin32() override { destroyWindow(); }

  bool createWindow(const WindowConfig &config) override {
    m_width = config.width;
    m_height = config.height;

    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_CLASSDC;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.lpszClassName = L"Tiny42DEngine";

    if (!RegisterClassEx(&wc)) {
      std::cerr << "윈도우 클래스 등록 실패!" << std::endl;
      return false;
    }
    m_wndClass = wc;
    m_registered = true;

    // 클라이언트 영역 크기에 맞게 윈도우 크기 조정
    RECT wr = {0, 0, static_cast<LONG>(config.width),
               static_cast<LONG>(config.height)};
    DWORD style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    AdjustWindowRect(&wr, style, FALSE);

    // 화면 중앙에 배치
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    int posX = (screenW - (wr.right - wr.left)) / 2;
    int posY = (screenH - (wr.bottom - wr.top)) / 2;

    m_hwnd = CreateWindow(wc.lpszClassName, config.title.c_str(), style, posX,
                          posY, wr.right - wr.left, wr.bottom - wr.top, nullptr,
                          nullptr, wc.hInstance, nullptr);

    if (!m_hwnd) {
      std::cerr << "윈도우 생성 실패!" << std::endl;
      return false;
    }

    ShowWindow(m_hwnd, SW_SHOWDEFAULT);
    UpdateWindow(m_hwnd);

    std::cout << "Win32 윈도우 생성 완료: " << config.width << "x"
              << config.height << std::endl;
    return true;
  }

  bool pollEvents() override {
    MSG msg = {};
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      if (msg.message == WM_QUIT) {
        return false;
      }
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    return true;
  }

  WindowHandle getWindowHandle() const override {
    WindowHandle handle;
    handle.hwnd = m_hwnd;
    handle.hinstance = m_wndClass.hInstance;
    return handle;
  }

  void getFramebufferSize(uint32_t &width, uint32_t &height) const override {
    RECT rect;
    GetClientRect(m_hwnd, &rect);
    width = static_cast<uint32_t>(rect.right - rect.left);
    height = static_cast<uint32_t>(rect.bottom - rect.top);
  }

  void destroyWindow() override {
    if (m_hwnd) {
      DestroyWindow(m_hwnd);
      m_hwnd = nullptr;
    }
    if (m_registered) {
      UnregisterClass(m_wndClass.lpszClassName, m_wndClass.hInstance);
      m_registered = false;
    }
  }

  std::vector<const char *> getRequiredInstanceExtensions() const override {
    return {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
    };
  }

  VkSurfaceKHR createSurface(VkInstance instance) const override {
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = m_hwnd;
    createInfo.hinstance = m_wndClass.hInstance;

    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) !=
        VK_SUCCESS) {
      throw std::runtime_error("Win32 Vulkan Surface 생성 실패!");
    }

    std::cout << "Win32 Vulkan Surface 생성 완료" << std::endl;
    return surface;
  }

private:
  static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam,
                                  LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN:
      if (wParam == VK_ESCAPE) {
        PostQuitMessage(0);
        return 0;
      }
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_SYSCOMMAND:
      if ((wParam & 0xfff0) == SC_KEYMENU)
        return 0; // ALT 메뉴 비활성화
      break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }

  HWND m_hwnd = nullptr;
  WNDCLASSEX m_wndClass = {};
  bool m_registered = false;
  uint32_t m_width = 0;
  uint32_t m_height = 0;
};

// Platform::create() - Win32 구현
std::unique_ptr<Platform> Platform::create() {
  return std::make_unique<PlatformWin32>();
}

} // namespace tiny

#endif // _WIN32
