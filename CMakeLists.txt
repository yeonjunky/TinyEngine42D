cmake_minimum_required(VERSION 3.16)

project(Tiny42DEngine VERSION 1.0.0 LANGUAGES CXX)

# C++ 표준
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC UTF-8 소스 파일 지원
if(MSVC)
    add_compile_options(/utf-8)
endif()

# 플랫폼 확인
if(NOT (WIN32 OR APPLE OR UNIX))
    message(FATAL_ERROR "이 프로젝트는 Windows, macOS, Linux에서만 지원됩니다.")
endif()

# ============================================================
# 외부 라이브러리
# ============================================================

# Vulkan SDK 찾기
find_package(Vulkan REQUIRED)

# GLM (수학 라이브러리, 헤더 전용)
find_package(glm CONFIG QUIET)
if(NOT glm_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 1.0.1
    )
    FetchContent_MakeAvailable(glm)
endif()

# ============================================================
# 소스 파일 (공통)
# ============================================================
set(COMMON_SOURCES
    src/main.cpp
    src/renderer/vk_instance.cpp
)

# 플랫폼별 소스 파일
if(WIN32)
    set(PLATFORM_SOURCES src/platform/platform_win32.cpp)
else()
    set(PLATFORM_SOURCES src/platform/platform_glfw.cpp)
endif()

# ============================================================
# 실행 파일
# ============================================================
add_executable(Tiny42DEngine
    ${COMMON_SOURCES}
    ${PLATFORM_SOURCES}
)

# 소스 디렉토리를 include 경로에 추가
target_include_directories(Tiny42DEngine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
)

# Vulkan & GLM 링크 (모든 플랫폼 공통)
target_link_libraries(Tiny42DEngine PRIVATE
    Vulkan::Vulkan
    glm::glm
)

# GLM: Vulkan 좌표계 호환 매크로
target_compile_definitions(Tiny42DEngine PRIVATE
    GLM_FORCE_RADIANS
    GLM_FORCE_DEPTH_ZERO_TO_ONE
)

# ============================================================
# 플랫폼별 설정
# ============================================================

# --- Windows ---
if(WIN32)
    target_compile_definitions(Tiny42DEngine PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        UNICODE
        _UNICODE
    )

    target_link_libraries(Tiny42DEngine PRIVATE
        user32
        kernel32
        gdi32
    )

    if(CMAKE_GENERATOR MATCHES "Visual Studio")
        set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tiny42DEngine)
    endif()
endif()

# --- macOS ---
if(APPLE)
    find_package(glfw3 3.3 QUIET)
    if(NOT glfw3_FOUND)
        target_include_directories(Tiny42DEngine PRIVATE /opt/homebrew/include)
        target_link_directories(Tiny42DEngine PRIVATE /opt/homebrew/lib)
        target_link_libraries(Tiny42DEngine PRIVATE
            /opt/homebrew/lib/libglfw.3.4.dylib
        )
    else()
        target_link_libraries(Tiny42DEngine PRIVATE glfw)
    endif()
endif()

# --- Linux ---
if(UNIX AND NOT APPLE)
    find_package(glfw3 3.3 REQUIRED)
    find_package(X11 REQUIRED)

    target_link_libraries(Tiny42DEngine PRIVATE
        glfw
        ${X11_LIBRARIES}
        dl
        pthread
    )
endif()

# ============================================================
# 출력 디렉토리
# ============================================================
set_target_properties(Tiny42DEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)