#if !defined(_WIN32)

#include "platform.hpp"
#include <iostream>
#include <stdexcept>

namespace tiny {

class PlatformGLFW : public Platform {
public:
  ~PlatformGLFW() override { destroyWindow(); }

  bool createWindow(const WindowConfig &config) override {
    // Force to use X11 protocol for Wayland session
    // After implementing redering phase, It may can be deleted.
#if defined(__linux__)
#if defined(GLFW_VERSION_MAJOR) &&                                              \
    (GLFW_VERSION_MAJOR > 3 ||                                                  \
     (GLFW_VERSION_MAJOR == 3 && GLFW_VERSION_MINOR >= 4))
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_X11);
#endif
#endif

    if (!glfwInit()) {
      std::cerr << "GLFW 초기화 실패!" << std::endl;
      return false;
    }

    // Vulkan 사용 시 OpenGL 컨텍스트 생성하지 않음
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // wstring → string 변환 (GLFW는 UTF-8)
    std::string titleUtf8(config.title.begin(), config.title.end());

    m_window = glfwCreateWindow(static_cast<int>(config.width),
                                static_cast<int>(config.height),
                                titleUtf8.c_str(), nullptr, nullptr);

    if (!m_window) {
      std::cerr << "GLFW 윈도우 생성 실패!" << std::endl;
      glfwTerminate();
      return false;
    }

    // ESC 키로 종료
    glfwSetKeyCallback(m_window,
                       [](GLFWwindow *win, int key, int, int action, int) {
                         if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                           glfwSetWindowShouldClose(win, GLFW_TRUE);
                         }
                       });

    if constexpr (kDebugBuild) {
      std::cout << "[DEBUG] GLFW 윈도우 생성 완료: " << config.width << "x"
                << config.height << std::endl;
    }
    return true;
  }

  bool pollEvents() override {
    if (glfwWindowShouldClose(m_window)) {
      return false;
    }
    glfwPollEvents();
    return true;
  }

  WindowHandle getWindowHandle() const override {
    WindowHandle handle;
    handle.window = m_window;
    return handle;
  }

  void getFramebufferSize(uint32_t &width, uint32_t &height) const override {
    int w, h;
    glfwGetFramebufferSize(m_window, &w, &h);
    width = static_cast<uint32_t>(w);
    height = static_cast<uint32_t>(h);
  }

  void destroyWindow() override {
    if (m_window) {
      glfwDestroyWindow(m_window);
      m_window = nullptr;
    }
    glfwTerminate();
  }

  std::vector<const char *> getRequiredInstanceExtensions() const override {
    uint32_t count = 0;
    const char **extensions = glfwGetRequiredInstanceExtensions(&count);
    return std::vector<const char *>(extensions, extensions + count);
  }

  VkSurfaceKHR createSurface(VkInstance instance) const override {
    VkSurfaceKHR surface = VK_NULL_HANDLE;
    if (glfwCreateWindowSurface(instance, m_window, nullptr, &surface) !=
        VK_SUCCESS) {
      throw std::runtime_error("GLFW Vulkan Surface 생성 실패!");
    }

    if constexpr (kDebugBuild) {
      std::cout << "[DEBUG] GLFW Vulkan Surface 생성 완료" << std::endl;
    }
    return surface;
  }

private:
  GLFWwindow *m_window = nullptr;
};

// Platform::create() - GLFW 구현
std::unique_ptr<Platform> Platform::create() {
  return std::make_unique<PlatformGLFW>();
}

} // namespace tiny

#endif // !_WIN32
