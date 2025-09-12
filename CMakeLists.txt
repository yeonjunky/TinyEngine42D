cmake_minimum_required(VERSION 3.16)

project(Tiny42DEngine VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Check for Windows platform
if(NOT WIN32)
    message(FATAL_ERROR "This project is designed for Windows only and requires Win32 API.")
endif()

# Windows specific settings
if(WIN32)
    # Enable Win32 API
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
endif()

# Create executable
add_executable(Tiny42DEngine
    src/main.cpp
)

# Link Windows libraries
if(WIN32)
    target_link_libraries(Tiny42DEngine
        user32
        kernel32
        gdi32
    )
endif()

# Set Visual Studio startup project
if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Tiny42DEngine)
endif()

# Output directories
set_target_properties(Tiny42DEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
)