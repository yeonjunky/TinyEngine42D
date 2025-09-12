# Tiny42D-Engine
Tiny engine for 2D

## Overview
A simple Win32 application written in C++ that creates a 1920x1080 window. This project uses CMake for build configuration and is designed to work with Visual Studio.

## Requirements
- Windows operating system
- CMake 3.16 or higher
- Visual Studio 2019 or higher (or compatible C++ compiler)
- Windows SDK

## Building the Project

### Using Visual Studio with CMake
1. Clone the repository
2. Open a command prompt in the project directory
3. Create a build directory:
   ```cmd
   mkdir build
   cd build
   ```
4. Generate Visual Studio project files:
   ```cmd
   cmake -G "Visual Studio 16 2019" -A x64 ..
   ```
   Or for Visual Studio 2022:
   ```cmd
   cmake -G "Visual Studio 17 2022" -A x64 ..
   ```
5. Open the generated `.sln` file in Visual Studio or build from command line:
   ```cmd
   cmake --build . --config Release
   ```

### Using Visual Studio Code
1. Install the CMake Tools extension
2. Open the project folder
3. Use Ctrl+Shift+P and run "CMake: Configure"
4. Use Ctrl+Shift+P and run "CMake: Build"

## Running the Application
After building, the executable will be located in the `build/bin/` directory. Run `Tiny42DEngine.exe` to see a 1920x1080 window with the Tiny42D Engine.

## Features
- Creates a 1920x1080 window
- Centers the window on screen
- Basic Win32 message handling
- Simple graphics rendering with text display
