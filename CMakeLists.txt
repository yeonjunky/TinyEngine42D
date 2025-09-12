cmake_minimum_required(VERSION 3.16)

project(Tiny42DEngine VERSION 1.0.0 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows 플랫폼 확인
if(NOT WIN32)
    message(FATAL_ERROR "이 프로젝트는 Windows 전용으로 설계되었으며 Win32 API가 필요합니다.")
endif()

# Windows 전용 설정
if(WIN32)
    # Win32 API 활성화
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    # Unicode 지원 활성화
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
endif()

# 실행 파일 생성 (콘솔 애플리케이션으로 변경)
add_executable(Tiny42DEngine
    src/main.cpp
)

# Windows 라이브러리 링크
if(WIN32)
    target_link_libraries(Tiny42DEngine
        user32
        kernel32
        gdi32
    )
endif()

# Visual Studio 시작 프로젝트 설정
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tiny42DEngine)
endif()

# 출력 디렉토리
set_target_properties(Tiny42DEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)