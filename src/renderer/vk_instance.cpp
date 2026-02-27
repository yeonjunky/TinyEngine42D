#include "vk_instance.h"
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace tiny {

// 기본 유효성 검사 레이어
const std::vector<const char *> VulkanInstance::s_validationLayers = {
    "VK_LAYER_KHRONOS_validation"};

VulkanInstance::~VulkanInstance() { destroy(); }

bool VulkanInstance::create(const std::string &appName,
                            const std::vector<const char *> &requiredExtensions,
                            bool enableValidation) {
  m_validationEnabled = enableValidation;

  // 유효성 검사 레이어 확인
  if (m_validationEnabled && !checkValidationLayerSupport()) {
    std::cerr
        << "경고: 유효성 검사 레이어를 사용할 수 없습니다. 비활성화합니다."
        << std::endl;
    m_validationEnabled = false;
  }

  // 앱 정보
  VkApplicationInfo appInfo = {};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName.c_str();
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "Tiny42D";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_2;

  // 확장 목록 구성
  std::vector<const char *> extensions(requiredExtensions);
  if (m_validationEnabled) {
    extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  // 사용 가능한 확장 출력
  uint32_t extensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount,
                                         availableExtensions.data());

  std::cout << "사용 가능한 Vulkan 확장 (" << extensionCount
            << "개):" << std::endl;
  for (const auto &ext : availableExtensions) {
    std::cout << "  " << ext.extensionName << std::endl;
  }

  // 인스턴스 생성 정보
  VkInstanceCreateInfo createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
  createInfo.ppEnabledExtensionNames = extensions.data();

  // Debug Messenger 생성 정보 (인스턴스 생성/파괴 시에도 디버그 메시지 수신)
  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
  if (m_validationEnabled) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(s_validationLayers.size());
    createInfo.ppEnabledLayerNames = s_validationLayers.data();

    debugCreateInfo.sType =
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.messageSeverity =
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType =
        VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback;

    createInfo.pNext = &debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;
  }

  // VkInstance 생성
  VkResult result = vkCreateInstance(&createInfo, nullptr, &m_instance);
  if (result != VK_SUCCESS) {
    std::cerr << "VkInstance 생성 실패! 에러 코드: " << result << std::endl;
    return false;
  }

  std::cout << "VkInstance 생성 완료" << std::endl;

  // Debug Messenger 설정
  if (m_validationEnabled) {
    setupDebugMessenger();
  }

  return true;
}

void VulkanInstance::destroy() {
  if (m_instance != VK_NULL_HANDLE) {
    destroyDebugMessenger();
    vkDestroyInstance(m_instance, nullptr);
    m_instance = VK_NULL_HANDLE;
    std::cout << "VkInstance 파괴 완료" << std::endl;
  }
}

bool VulkanInstance::checkValidationLayerSupport() const {
  uint32_t layerCount = 0;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : s_validationLayers) {
    bool found = false;
    for (const auto &layerProps : availableLayers) {
      if (std::strcmp(layerName, layerProps.layerName) == 0) {
        found = true;
        break;
      }
    }
    if (!found) {
      std::cerr << "유효성 검사 레이어 없음: " << layerName << std::endl;
      return false;
    }
  }
  return true;
}

void VulkanInstance::setupDebugMessenger() {
  VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr;

  // vkCreateDebugUtilsMessengerEXT는 확장 함수이므로 직접 로드
  auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(m_instance, "vkCreateDebugUtilsMessengerEXT"));

  if (func != nullptr) {
    VkResult result = func(m_instance, &createInfo, nullptr, &m_debugMessenger);
    if (result == VK_SUCCESS) {
      std::cout << "Vulkan Debug Messenger 활성화" << std::endl;
    } else {
      std::cerr << "Debug Messenger 생성 실패" << std::endl;
    }
  } else {
    std::cerr << "vkCreateDebugUtilsMessengerEXT 함수를 찾을 수 없음"
              << std::endl;
  }
}

void VulkanInstance::destroyDebugMessenger() {
  if (m_debugMessenger != VK_NULL_HANDLE) {
    auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
        vkGetInstanceProcAddr(m_instance, "vkDestroyDebugUtilsMessengerEXT"));
    if (func != nullptr) {
      func(m_instance, m_debugMessenger, nullptr);
    }
    m_debugMessenger = VK_NULL_HANDLE;
  }
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData) {
  const char *prefix = "[VULKAN]";
  if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
    prefix = "[VULKAN ERROR]";
  } else if (messageSeverity &
             VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
    prefix = "[VULKAN WARNING]";
  }

  std::cerr << prefix << " " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

} // namespace tiny
