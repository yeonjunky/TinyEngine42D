#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#ifdef _WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#include <windows.h>
#else
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#include <vulkan/vulkan.h>

namespace tiny {

/// 디버그 빌드 여부를 나타내는 컴파일 타임 상수
/// NDEBUG가 정의되면 릴리즈 빌드, 그렇지 않으면 디버그 빌드
#ifdef NDEBUG
inline constexpr bool kDebugBuild = false;
#else
inline constexpr bool kDebugBuild = true;
#endif

/// 플랫폼별 윈도우 핸들을 추상화하는 구조체
struct WindowHandle {
#ifdef _WIN32
  HWND hwnd = nullptr;
  HINSTANCE hinstance = nullptr;
#else
  GLFWwindow *window = nullptr;
#endif
};

/// 윈도우 생성에 필요한 설정
struct WindowConfig {
  std::wstring title = L"Tiny42D Engine";
  uint32_t width = 1280;
  uint32_t height = 960;
};

/// 플랫폼 독립적인 윈도우 인터페이스
class Platform {
public:
  virtual ~Platform() = default;

  /// 윈도우를 생성합니다
  virtual bool createWindow(const WindowConfig &config) = 0;

  /// 메시지/이벤트를 처리합니다. false를 반환하면 종료 요청
  virtual bool pollEvents() = 0;

  /// 윈도우 핸들을 반환합니다
  virtual WindowHandle getWindowHandle() const = 0;

  /// 현재 프레임버퍼의 크기를 반환합니다
  virtual void getFramebufferSize(uint32_t &width, uint32_t &height) const = 0;

  /// 윈도우를 파괴합니다
  virtual void destroyWindow() = 0;

  /// Vulkan 인스턴스 확장 목록을 반환합니다
  virtual std::vector<const char *> getRequiredInstanceExtensions() const = 0;

  /// VkSurfaceKHR를 생성합니다
  virtual VkSurfaceKHR createSurface(VkInstance instance) const = 0;

  /// 현재 플랫폼에 맞는 Platform 인스턴스를 생성합니다
  static std::unique_ptr<Platform> create();
};

} // namespace tiny
