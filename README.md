## Tiny Engine for 2D project

#### 개발환경
- Windows 11, Visual Studio 2022, CMake 3.16이상을 기준으로 합니다.
- 맥 환경을 원하시는 분은 MoltenVK를 이용하시면 됩니다. 다만 디버깅이 조금 힘들 수 있습니다.

#### 목표
- 동아리 부원들에게 의미있는 프로젝트의 완성과 협업의 기회를 제공하고자 만든 프로젝트 입니다.
- 아주아주 간단한 프로토타입을 1차목표로 하고 그 다음을 계획할 예정입니다.
- 함께 프로그래밍 하는 연습을 하는 것이 목표입니다. 잘 할 필요 없고 성실할 필요도 없습니다.

#### 규칙
- 서로간의 예의를 지켜주세요. 만약 어길시 프로젝트에서 제외될 수 있습니다.
- 기본적으로 패닉어택분들께만 열려있는 프로젝트 입니다.
- 적어도 한명이상이 리뷰 후에 머지를 가능하게 할 계획입니다. 본인이 PR보내고 머지하는것은 금지합니다.

---

## 빌드 방법

### 사전 준비

| 플랫폼 | 필요 사항 |
|--------|----------|
| **Windows** | [Vulkan SDK](https://vulkan.lunarg.com/), Visual Studio 2022, CMake 3.16 이상 |
| **Linux** | `sudo apt install libvulkan-dev libglfw3-dev libglm-dev libx11-dev cmake g++` |
| **macOS** | [Vulkan SDK](https://vulkan.lunarg.com/), `brew install glfw glm` |

### 빌드 명령어

```bash
# 프로젝트 구성
cmake -S . -B build

# 빌드 (Debug)
cmake --build build --config Debug

# 빌드 (Release)
cmake --build build --config Release
```

### 빌드 결과물 위치

```
build/bin/Tiny42DEngine.exe    # Windows
build/bin/Tiny42DEngine        # Linux / macOS
```

### 프로젝트 구조

```
src/
├── main.cpp                    # 진입점
├── platform/
│   ├── platform.h              # 플랫폼 추상화 인터페이스
│   ├── platform_win32.cpp      # Win32 구현
│   └── platform_glfw.cpp       # GLFW 구현 (Linux/macOS)
└── renderer/
    ├── vk_instance.h/cpp       # Vulkan 인스턴스 + 유효성 검사 레이어
    └── ...                     # (추가 모듈 예정)
```
