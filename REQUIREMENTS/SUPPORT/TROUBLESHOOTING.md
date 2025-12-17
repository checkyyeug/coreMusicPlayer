# coreMusicPlayer Troubleshooting Guide

## 1. Build Issues

### 1.1 Common Compilation Errors

#### CMake Configuration Errors

**Error: CMake cannot find Qt6**
```
CMake Error at CMakeLists.txt:XX (find_package):
  Could not find a package configuration file provided by "Qt6"
```

**Solutions:**
1. Set Qt6 path manually:
   ```bash
   export CMAKE_PREFIX_PATH=/path/to/qt6
   # Windows
   set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\mingw_64
   ```

2. Install Qt6 development packages:
   ```bash
   # Ubuntu/Debian
   sudo apt install qt6-base-dev qt6-tools-dev

   # Fedora
   sudo dnf install qt6-qtbase-devel qt6-qttools-devel
   ```

3. Use Qt5 fallback (if Qt5 is installed):
   - Edit CMakeLists.txt to enable Qt5 support
   - Install Qt5 development packages

**Error: MinGW compiler not found**
```
Error: Could NOT find CMAKE_C_COMPILER
Error: Could NOT find CMAKE_CXX_COMPILER
```

**Solutions:**
1. Add MinGW to PATH:
   ```bash
   # Windows (Command Prompt)
   set PATH=C:\Qt\Tools\mingw1310_64\bin;%PATH%

   # Windows (PowerShell)
   $env:PATH = "C:\Qt\Tools\mingw1310_64\bin;$env:PATH"
   ```

2. Specify compiler in CMake:
   ```bash
   cmake -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ ..
   ```

#### Linking Errors

**Error: Undefined reference to audio functions**
```
undefined reference to `WASAPIOutput::initialize()'
```

**Solutions:**
1. Check platform-specific includes:
   - Windows: Ensure `#ifdef _WIN32` is present
   - Linux: Ensure ALSA development libraries are installed

2. Link required libraries:
   ```cmake
   # Windows
   target_link_libraries(your_target ole32 uuid winmm)

   # Linux
   target_link_libraries(your_target asound pthread)
   ```

**Error: Qt5Core.dll not found**
```
The program can't start because Qt5Core.dll is missing
```

**Solutions:**
1. Copy required Qt DLLs to executable directory
2. Add Qt bin directory to PATH
3. Use windeployqt (Windows) or linuxdeployqt (Linux)

#### Thread Library Error

**Error: CMAKE_HAVE_LIBC_PTHREAD failed**
```
-- Performing Test CMAKE_HAVE_LIBC_PTHREAD - Failed
```

**Solutions:**
1. This is normal on Windows - Windows uses native threading
2. Add to CMakeLists.txt:
   ```cmake
   if(WIN32)
     set(CMAKE_USE_PTHREADS_INIT 0)
     set(THREADS_PREFER_PTHREAD_FLAG 0)
   endif()
   ```

### 1.2 Runtime Errors

#### Audio Device Errors

**Error: Audio device initialization failed**
```
Failed to initialize audio device: Device not found
```

**Solutions:**
1. Check if audio device is available:
   - Windows: Open Sound settings
   - Linux: Run `aplay -l` to list devices

2. Update audio drivers:
   - Windows: Update through Device Manager
   - Linux: Update ALSA drivers

3. Check permissions:
   - Linux: Add user to audio group: `sudo usermod -a -G audio $USER`

#### File Loading Errors

**Error: Cannot open audio file**
```
Failed to load file: Permission denied
```

**Solutions:**
1. Check file permissions
2. Verify file exists at specified path
3. Check if file format is supported:
   - Supported: WAV, MP3, FLAC, OGG
   - Check file integrity

## 2. Platform-Specific Issues

### 2.1 Windows Issues

#### DLL Hell
**Problem: Multiple versions of Qt DLLs**
```
Qt5Widgets.dll: Cannot find procedure entry point
```

**Solution:**
1. Clean all Qt DLLs from executable directory
2. Copy correct DLL versions from Qt installation
3. Use windeployqt to ensure correct versions

#### WASAPI Issues
**Problem: No audio output with WASAPI**
```
AUDCLNT_E_DEVICE_INVALIDATED
```

**Solutions:**
1. Restart Windows Audio service
2. Change default audio device
3. Update audio drivers
4. Run as Administrator

#### MinGW vs MSVC Issues
**Problem: Mixing runtimes**
```
Runtime Error! R6034: An application has made an attempt to load the C runtime library incorrectly
```

**Solutions:**
1. Ensure all libraries use same runtime
2. Rebuild all dependencies with same compiler
3. Use static linking where possible

### 2.2 Linux Issues

#### ALSA Issues
**Problem: Device busy**
```
ALSA lib pcm.c:2565:(snd_pcm_open_noupdate) Unknown PCM cards.pcm.rear
```

**Solutions:**
1. Kill other audio processes:
   ```bash
   sudo pkill pulseaudio
   sudo pkill jackd
   ```

2. Configure ALSA properly:
   - Edit ~/.asoundrc
   - Test with `speaker-test -t wav`

#### PulseAudio Issues
**Problem: No sound with PulseAudio**
```
Connection refused: pa_context_connect() failed
```

**Solutions:**
1. Restart PulseAudio:
   ```bash
   pulseaudio -k
   pulseaudio --start
   ```

2. Check configuration:
   ```bash
   pacmd list-sinks
   ```

#### Package Conflicts
**Problem: Conflicting Qt versions**
```
qt6-base-dev : Conflicts: qtbase5-dev
```

**Solutions:**
1. Use container or virtual environment
2. Build from source
3. Use distribution-specific packages

### 2.3 macOS Issues

#### Code Signing
**Problem: App cannot be opened**
```
"QtGuiPlayer" can't be opened because Apple cannot check it for malicious software
```

**Solutions:**
1. Allow in System Preferences
2. Sign the application:
   ```bash
   codesign --force --deep --sign "Developer ID" QtGuiPlayer.app
   ```

#### Permission Issues
**Problem: Microphone access denied**
```
[access] This app has crashed because it attempted to access privacy-sensitive data without a usage description
```

**Solutions:**
1. Add to Info.plist:
   ```xml
   <key>NSMicrophoneUsageDescription</key>
   <string>This app needs microphone access for audio processing</string>
   ```

## 3. Qt GUI Issues

### 3.1 Qt Version Conflicts

**Error: QT_VERSION_CHECK not defined**
```
error: 'QT_VERSION_CHECK' was not declared in this scope
```

**Solutions:**
1. Include correct Qt headers:
   ```cpp
   #include <QtGlobal>
   ```

2. Check Qt version in CMake:
   ```cmake
   message(STATUS "Qt version: ${Qt6_VERSION}")
   ```

### 3.2 MOC Issues

**Error: Undefined reference to vtable**
```
undefined reference to `vtable for MainWindow'
```

**Solutions:**
1. Add Q_OBJECT macro to class definition
2. Run qmake/moc manually:
   ```bash
   moc mainwindow.h -o moc_mainwindow.cpp
   ```

3. Ensure CMAKE_AUTOMOC is enabled in CMake

### 3.3 Widget Issues

**Problem: Window doesn't appear**
```
QWidget::winId(): Must construct a QApplication before a QWidget
```

**Solutions:**
1. Create QApplication before any widgets
2. Check for multiple QApplication instances

**Problem: Styles not applied**
```
QStyleSheet: Could not parse application stylesheet
```

**Solutions:**
1. Validate CSS syntax
2. Use Qt Designer for style testing
3. Check Qt version compatibility

## 4. Performance Issues

### 4.1 Audio Glitches

**Symptoms:**
- Popping/clicking sounds
- Audio dropouts
- Stuttering playback

**Diagnosis:**
1. Check buffer size
2. Monitor CPU usage
3. Verify sample rate settings

**Solutions:**
```cpp
// Increase buffer size
audioOutput->setBufferSize(4096);

// Use lower latency
audioOutput->setLowLatency(true);

// Adjust sample rate
audioFormat.setSampleRate(44100);
```

### 4.2 Memory Leaks

**Detection Tools:**
- Windows: Task Manager, Process Explorer
- Linux: Valgrind, Massif
- macOS: Instruments

**Common Causes:**
1. Not deleting QObject subclasses
2. Circular references with shared_ptr
3. Audio buffers not released

**Solutions:**
```cpp
// Use RAII
class AudioPlayer {
    std::unique_ptr<AudioOutput> output_;
public:
    AudioPlayer() : output_(std::make_unique<AudioOutput>()) {}
};

// Use QObject cleanup
auto widget = new QWidget(parent);
widget->setAttribute(Qt::WA_DeleteOnClose);
```

### 4.3 High CPU Usage

**Profiling Tools:**
- Windows: Visual Studio Profiler, Intel VTune
- Linux: perf, gprof
- macOS: Instruments

**Optimization Strategies:**
1. Use SIMD optimizations
2. Implement buffer pooling
3. Reduce sample rate conversions
4. Use lock-free data structures

## 5. Development Environment Issues

### 5.1 IDE Configuration

#### Visual Studio Code
**Problem: IntelliSense not working**
```
#include errors detected
```

**Solutions:**
1. Configure include paths in c_cpp_properties.json
2. Install C/C++ extension
3. Add CMake Tools extension

#### Qt Creator
**Problem: Kit not configured**
```
No valid kit found
```

**Solutions:**
1. Configure kits in Options > Kits
2. Set correct compiler
3. Set correct Qt version
4. Set correct CMake installation

### 5.2 Debugger Issues

**GDB on Windows**
```
Error: dll starting at 0x... cannot be found
```

**Solutions:**
1. Use MSVC debugger with Visual Studio
2. Install GDB with MinGW
3. Add symbol files

**Breakpoints not hit**
1. Ensure debug build (-g flag)
2. Check optimization flags (-O0)
3. Verify source paths

## 6. Testing Issues

### 6.1 Unit Test Failures

**Google Test Issues**
```
gtest: Could not find GoogleTest
```

**Solutions:**
1. Install GoogleTest:
   ```bash
   sudo apt install libgtest-dev  # Linux
   vcpkg install gtest            # Windows
   ```

2. Build from source
3. Use CMake FetchContent

**Test takes too long**
1. Use mock objects
2. Limit test data
3. Run tests in parallel

### 6.2 Audio Test Failures

**Test audio not available**
1. Check test audio files exist
2. Verify permissions
3. Use mock audio output for testing

## 7. Deployment Issues

### 7.1 Windows Deployment

**Missing DLLs**
```
The code execution cannot proceed because MSVCP140.dll was not found
```

**Solutions:**
1. Install Visual C++ Redistributable
2. Bundle required DLLs
3. Use static linking

**UAC Issues**
```
The application requires elevated permissions
```

**Solutions:**
1. Add manifest file
2. Sign the application
3. Use installer with elevation

### 7.2 Linux Deployment

**Library not found**
```
error while loading shared libraries: libasound.so.2
```

**Solutions:**
1. Install required packages
2. Use AppImage for bundling
3. Create deb package with dependencies

## 8. Network and Repository Issues

### 8.1 Git Issues

**Push fails with network error**
```
fatal: unable to access 'https://github.com/.../': Failed to connect to github.com port 443
```

**Solutions:**
1. Check internet connection
2. Use SSH instead of HTTPS
3. Configure proxy if needed

**Merge conflicts**
1. Pull latest changes
2. Resolve conflicts manually
3. Test before merging

## 9. Quick Reference

### 9.1 Common Commands

**Clean build:**
```bash
# Windows
build_final.bat clean

# Linux
make clean && rm -rf build/
mkdir build && cd build && cmake .. && make
```

**Check dependencies:**
```bash
# Check Qt
qmake --version

# Check CMake
cmake --version

# Check compiler
g++ --version  # Linux/MinGW
cl             # MSVC
```

**Debug run:**
```bash
# With GDB
gdb ./QtGuiPlayer

# With Visual Studio
devenv /debugexe QtGuiPlayer.exe
```

### 9.2 Environment Variables

```bash
# Qt paths (Windows)
set QT_DIR=C:\Qt\6.5.0\mingw_64
set PATH=%QT_DIR%\bin;%PATH%

# Qt paths (Linux)
export QTDIR=/usr/local/Qt-6.5.0
export PATH=$QTDIR/bin:$PATH
export LD_LIBRARY_PATH=$QTDIR/lib:$LD_LIBRARY_PATH
```

### 9.3 Log Locations

- Windows: `%APPDATA%\coreMusicPlayer\logs\`
- Linux: `~/.local/share/coreMusicPlayer/logs/`
- macOS: `~/Library/Logs/coreMusicPlayer/`

## 10. Getting Help

### 10.1 Resources

- **Documentation**: Check claude-glm_requirements/ directory
- **GitHub Issues**: https://github.com/checkyyeug/coreMusicPlayer/issues
- **Qt Documentation**: https://doc.qt.io/
- **CMake Documentation**: https://cmake.org/documentation/

### 10.2 Reporting Issues

When reporting issues, include:
1. Operating system and version
2. Qt version
3. Compiler version
4. Full error message
5. Steps to reproduce
6. Expected vs actual behavior

This troubleshooting guide should help resolve the most common issues encountered while developing, building, and deploying coreMusicPlayer.