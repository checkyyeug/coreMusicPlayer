# coreMusicPlayer Dependencies Documentation

## 1. Overview

coreMusicPlayer has a carefully managed dependency structure designed to balance functionality, performance, and maintainability. Dependencies are categorized as: core, optional, platform-specific, and development dependencies.

## 2. Core Dependencies

### 2.1 C++ Standard Library
- **Requirement**: C++17 standard library
- **Components**: STL containers, algorithms, smart pointers
- **Usage**: Throughout the application
- **Version**: C++17 or later

### 2.2 Audio Processing Libraries

#### minimp3 (MP3 Decoder)
- **URL**: https://github.com/lieff/minimp3
- **Version**: 3.3
- **License**: CC0-1.0
- **Integration**: Header-only
- **Usage**: src/minimp3.h
```cpp
#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
```

#### dr_wav (WAV Decoder)
- **URL**: https://github.com/mackron/dr_libs
- **Version**: 0.12.17
- **License**: Public Domain
- **Integration**: Header-only
- **Usage**: src/dr_wav.h

#### stb_vorbis (OGG Decoder - Optional)
- **URL**: https://github.com/nothings/stb
- **Version**: Latest
- **License**: Public Domain
- **Integration**: Header-only

### 2.3 JSON Library
- **Library**: nlohmann/json
- **Version**: 3.11.2
- **License**: MIT
- **Location**: third_party/nlohmann-json/
- **Usage**: Configuration and metadata
```cpp
#include <nlohmann/json.hpp>
using json = nlohmann::json;
```

## 3. GUI Dependencies

### 3.1 Qt6 Framework
- **Components Required**:
  - Qt6Core (Core functionality)
  - Qt6Widgets (GUI components)
  - Qt6Multimedia (Optional, for multimedia)
  - Qt6OpenGL (Optional, for visualizations)

- **Version**: 6.5.0 or later
- **License**: LGPLv3 (Commercial license available)

#### Windows Qt Installation
```batch
# Using Qt Online Installer
# Components:
# - Qt 6.5.0
# - MinGW 13.1.0
# - Qt Creator (optional)
# Location: C:\Qt\6.5.0\mingw_64
```

#### Linux Qt Installation
```bash
# Ubuntu/Debian
sudo apt install qt6-base-dev qt6-tools-dev qt6-multimedia-dev

# Fedora
sudo dnf install qt6-qtbase-devel qt6-qttools-devel qt6-qtmultimedia-devel
```

## 4. Platform-Specific Dependencies

### 4.1 Windows
- **Windows SDK**: 10.0 or later
  - WASAPI headers
  - DirectSound headers
  - Core Audio headers (future)

- **Visual Studio**: 2019 or 2022
  - C++ compiler (MSVC)
  - Build tools
  - Windows SDK

- **Runtime Libraries**:
  - ole32.dll (COM support)
  - uuid.dll (UUID generation)
  - winmm.dll (Legacy multimedia)

### 4.2 Linux
- **ALSA Development**:
  ```bash
  Ubuntu/Debian: libasound2-dev
  Fedora: alsa-lib-devel
  openSUSE: libasound2-devel
  ```

- **PulseAudio Development** (Optional):
  ```bash
  Ubuntu/Debian: libpulse-dev
  Fedora: pulseaudio-libs-devel
  ```

### 4.3 macOS (Future)
- **Xcode**: 12.0 or later
- **CoreAudio Framework**
- **AudioToolbox Framework**
- **AVFoundation Framework**

## 5. Foobar2000 SDK

### 5.1 SDK Components
- **Location**: foobar2000_sdk/
- **Version**: 1.5 or later
- **License**: Proprietary (for plugin compatibility)

### 5.2 SDK Components Used
```
foobar2000/
├── SDK/
│   ├── audio_stream_decoder.h
│   ├── audio_stream_encoder.h
│   ├── file_info.h
│   ├── input.h
│   ├── output.h
│   ├── dsp.h
│   ├── metadb.h
│   ├── playlist_loader.h
│   └── component_client.h
├── pfc/
│   ├── string.h
│   ├── array.h
│   ├── list.h
│   └── other utilities
└── libPPUI/
    └── UI components
```

### 5.3 SDK Integration
```cpp
// Include Foobar2000 headers
#include "foobar2000/SDK/input.h"
#include "foobar2000/SDK/file_info.h"

// Implementation
class FoobarDecoder : public input_stubs {
public:
    bool can_decode(const char *url);
    int decode_get_dynamic_info(const file_info *p_fileinfo, const playable_location *p_location, t_filesize hint_bytes, dynamic_info_request *p_out);
    int decode_initialize(const char *url, unsigned flags);
};
```

## 6. Development Dependencies

### 6.1 Build Tools
- **CMake**: 3.10 or later
- **Git**: 2.0 or later
- **Make/Makefiles**: GNU Make 3.81 or later (Linux)
- **MSBuild**: Included with Visual Studio

### 6.2 Debugging Tools
- **GDB**: GNU Debugger (Linux)
- **WinDbg**: Windows Debugger
- **Valgrind**: Memory profiling (Linux)
- **Visual Studio Debugger**: Windows

### 6.3 Profiling Tools
- **Intel VTune**: CPU/GPU profiling
- **AMD uProf**: AMD profiling
- **Perf**: Linux performance counters

## 7. Optional Dependencies

### 7.1 Audio Processing Libraries
- **FFTW3**: Fast Fourier Transform (advanced visualizations)
- **PortAudio**: Cross-platform audio I/O (alternative)
- **JUCE**: Alternative audio framework

### 7.2 Utility Libraries
- **Google Test**: Unit testing framework
- **Benchmark**: Performance benchmarking
- **spdlog**: Fast logging library
- **CLI11**: Command-line argument parsing

## 8. Dependency Management

### 8.1 Version Control
```cmake
# CMakeLists.txt
set(QT_MIN_VERSION 6.5)
find_package(Qt6 ${QT_MIN_VERSION} COMPONENTS Core Widgets REQUIRED)

# Check version
if(Qt6_VERSION VERSION_LESS ${QT_MIN_VERSION})
    message(FATAL_ERROR "Qt version too old")
endif()
```

### 8.2 Feature Flags
```cmake
# CMakeLists.txt
option(ENABLE_FOOBAR2000 "Enable Foobar2000 plugin support" ON)
option(ENABLE_OPENGL "Enable OpenGL visualizations" OFF)

if(ENABLE_FOOBAR2000)
    add_definitions(-DWITH_FOOBAR2000)
endif()
```

### 8.3 Package Detection
```cmake
# Detect optional packages
find_package(PkgConfig)
if(PkgConfig_FOUND)
    pkg_check_modules(PULSE libpulse QUIET)
    if(PULSE_FOUND)
        add_definitions(-DWITH_PULSE)
    endif()
endif()
```

## 9. Third-Party Library Integration

### 9.1 Header-Only Libraries
- Add to `third_party/` directory
- Update `.gitignore` appropriately
- Document license in source files
- Keep libraries up to date

### 9.2 Static Libraries
- Build with project if possible
- Provide pre-built binaries for Windows
- Use CMake `ExternalProject` for complex builds

### 9.3 Dynamic Libraries
- Check for system installation
- Provide fallback if not found
- Document version requirements

## 10. License Compatibility

### 10.1 License Matrix
| Library | License | Commercial Use | Distribution | Modifications |
|---------|---------|---------------|------------|--------------|
| minimp3 | CC0-1.0 | ✅ | ✅ | ✅ |
| dr_wav | Public Domain | ✅ | ✅ | ✅ |
| nlohmann/json | MIT | ✅ | ✅ | ✅ |
| Qt6 | LGPLv3 | Requires | ✅ | ✅ |
| Foobar2000 SDK | Proprietary | ❌ | ❌ | ❌ |
| FFTW3 | GPL | ❌ | ✅ | ✅ |

### 10.2 Compliance Checklist
- [ ] Verify all open-source licenses are compatible
- [ ] Document proprietary license restrictions
- [ ] Include license files in distribution
- [ ] Add license notices in source files

## 11. Dependency Security

### 11.1 Security Considerations
- Keep all dependencies updated
- Monitor CVE databases
- Use only trusted sources
- Verify checksums

### 11.2 Vulnerability Management
```bash
# Check for vulnerabilities
# Linux
cve-search --cpe minimp3

# Windows
# Use Visual Studio's vulnerability scanner
```

## 12. Dependency Updates

### 12.1 Update Strategy
- Monthly security updates
- Quarterly feature updates
- Annual major version updates

### 12.2 Update Process
```bash
# Update script
#!/bin/bash

# Update Qt
brew upgrade qt@6  # macOS
sudo apt update && sudo apt upgrade qt6-base-dev  # Ubuntu

# Update submodules
git submodule update --recursive

# Verify build
make clean && make
```

## 13. Alternative Dependencies

### 13.1 Audio Decoders
| Format | Primary | Alternative |
|--------|---------|-----------|
| MP3 | minimp3 | mpg123, ffmpeg |
| WAV | dr_wav | libsndfile |
| FLAC | builtin | libFLAC |
| OGG | builtin | libvorbis |

### 13.2 GUI Frameworks
| Framework | Current | Alternative |
|-----------|---------|-----------|
| Qt6 | Yes | wxWidgets, Dear ImGui |
| Qt5 | No | (Previous version) |
| GTKmm | No | (Linux only) |

### 13.3 Build Systems
| Tool | Current | Alternative |
|------|---------|-----------|
| CMake | Yes | Meson, Bazel |
| Make | Yes | Ninja |
| MSBuild | Yes | (Windows only) |

## 14. Dependency Troubleshooting

### 14.1 Common Issues

#### Qt Not Found
```bash
# Error: Qt6 not found
# Solution:
export CMAKE_PREFIX_PATH=/path/to/qt6
cmake ..
```

#### ALSA Headers Missing
```bash
# Error: alsa/asoundlib.h not found
# Solution (Ubuntu):
sudo apt install libasound2-dev
```

#### MinGW Not Found
```bash
# Error: g++.exe not found
# Solution:
set PATH=C:\Qt\Tools\mingw1310_64\bin;%PATH%
```

### 14.2 Verification Commands
```bash
# Check Qt installation
qmake --version

# Check CMake version
cmake --version

# Check compiler version
g++ --version  # Linux/MinGW
cl           # MSVC
```

## 15. Future Dependencies

### 15.1 Planned Additions
- **Raylib**: For 3D visualizations
- **OpenCV**: For audio analysis
- **WebRTC**: For network audio streaming
- **Vulkan**: For high-performance graphics

### 15.2 Evaluation Criteria
- License compatibility
- Maintenance status
- Performance impact
- Community support

## 16. Dependency Best Practices

1. **Minimal Dependencies**: Use only what's necessary
2. **Header-First**: Prefer header-only libraries
3. **Static Linking**: For easier distribution
4. **Version Pinning**: Avoid breaking changes
5. **Regular Updates**: Security and features
6. **License Tracking**: Maintain compliance

This dependency documentation ensures the project remains maintainable, secure, and compatible with all licensing requirements.