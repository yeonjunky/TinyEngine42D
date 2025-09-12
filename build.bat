@echo off
echo Building Tiny42D Engine...

:: 빌드 디렉토리가 없으면 생성
if not exist build mkdir build

:: 빌드 디렉토리로 이동
cd build

:: Visual Studio 프로젝트 파일 생성 (VS 2022를 먼저 시도, 그 다음 VS 2019)
echo Generating Visual Studio project files...
cmake -G "Visual Studio 17 2022" -A x64 .. 2>nul
if %errorlevel% neq 0 (
    echo Visual Studio 2022 not found, trying Visual Studio 2019...
    cmake -G "Visual Studio 16 2019" -A x64 ..
    if %errorlevel% neq 0 (
        echo Error: Visual Studio 2019 or 2022 required.
        pause
        exit /b 1
    )
)

:: 프로젝트 빌드
echo Building project...
cmake --build . --config Release

if %errorlevel% equ 0 (
    echo Build successful! Executable is in build\bin\Release\
) else (
    echo Build failed!
)

pause