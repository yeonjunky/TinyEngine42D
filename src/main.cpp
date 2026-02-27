#include <iostream>
#include <memory>

#include "platform/platform.hpp"
#include "renderer/vk_instance.hpp"

int main() {
#ifdef _WIN32
  // Windows 콘솔 UTF-8 출력 설정
  SetConsoleOutputCP(CP_UTF8);
#endif

  // =============================================
  // 1. 플랫폼 윈도우 생성
  // =============================================
  auto platform = tiny::Platform::create();

  tiny::WindowConfig windowConfig;
  windowConfig.title = L"Tiny42D Engine";
  windowConfig.width = 1280;
  windowConfig.height = 960;

  if (!platform->createWindow(windowConfig)) {
    std::cerr << "윈도우 생성 실패!" << std::endl;
    return -1;
  }

  // =============================================
  // 2. Vulkan Instance 생성
  // =============================================
  tiny::VulkanInstance vulkanInstance;

  auto requiredExtensions = platform->getRequiredInstanceExtensions();

#ifdef NDEBUG
  const bool enableValidation = false;
#else
  const bool enableValidation = true;
#endif

  if (!vulkanInstance.create("Tiny42DEngine", requiredExtensions,
                             enableValidation)) {
    std::cerr << "Vulkan Instance 생성 실패!" << std::endl;
    return -1;
  }

  // =============================================
  // 3. Vulkan Surface 생성
  // =============================================
  VkSurfaceKHR surface = platform->createSurface(vulkanInstance.getInstance());
  if (surface == VK_NULL_HANDLE) {
    std::cerr << "Vulkan Surface 생성 실패!" << std::endl;
    return -1;
  }

  std::cout << "========================================" << std::endl;
  std::cout << "Tiny42D Engine 초기화 완료!" << std::endl;
  std::cout << "ESC 키를 눌러 종료하세요." << std::endl;
  std::cout << "========================================" << std::endl;

  // =============================================
  // 4. 메인 루프
  // =============================================
  while (platform->pollEvents()) {
    // Phase 2+에서 여기에 렌더링 코드가 들어갑니다
  }

  // =============================================
  // 5. 정리 (생성의 역순)
  // =============================================
  // Surface 파괴
  if (surface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(vulkanInstance.getInstance(), surface, nullptr);
    std::cout << "Vulkan Surface 파괴 완료" << std::endl;
  }

  // VulkanInstance는 소멸자에서 자동 정리
  // Platform 윈도우도 소멸자에서 자동 정리

  std::cout << "Tiny42D Engine 종료" << std::endl;
  return 0;
}