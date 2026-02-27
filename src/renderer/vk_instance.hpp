#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>


namespace tiny {

/// VkInstance 생성 및 유효성 검사 레이어 관리
class VulkanInstance {
public:
  VulkanInstance() = default;
  ~VulkanInstance();

  // 복사/이동 금지
  VulkanInstance(const VulkanInstance &) = delete;
  VulkanInstance &operator=(const VulkanInstance &) = delete;

  /// VkInstance를 생성합니다
  /// @param appName 애플리케이션 이름
  /// @param requiredExtensions 필수 확장 목록 (플랫폼별 Surface 확장 포함)
  /// @param enableValidation 유효성 검사 레이어 활성화 여부
  bool create(const std::string &appName,
              const std::vector<const char *> &requiredExtensions,
              bool enableValidation = true);

  /// 리소스 정리
  void destroy();

  VkInstance getInstance() const { return m_instance; }
  bool isValidationEnabled() const { return m_validationEnabled; }

private:
  /// 유효성 검사 레이어 지원 여부 확인
  bool checkValidationLayerSupport() const;

  /// Debug Messenger 설정
  void setupDebugMessenger();
  void destroyDebugMessenger();

  /// Vulkan 디버그 콜백
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                void *pUserData);

  VkInstance m_instance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
  bool m_validationEnabled = false;

  static const std::vector<const char *> s_validationLayers;
};

} // namespace tiny
