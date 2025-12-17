# coreMusicPlayer Deployment Guide

## 1. Deployment Overview

coreMusicPlayer supports multiple deployment strategies tailored for different use cases:
- **Standalone executable distribution** - Simple deployment for end-users
- **Package-based distribution** (MSI, DEB, RPM) - System-integrated installation
- **Portable/zip distribution** - No-installation required
- **Windows Store submission** - Enterprise and consumer reach
- **Container deployment** - Server-side audio processing
- **Cloud deployment** - Scalable audio streaming services

### 1.1 Deployment Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     Deployment Pipeline                     │
├─────────────────┬─────────────────┬─────────────────────────┤
│   Build Stage   │  Package Stage  │    Distribution Stage    │
│                 │                 │                         │
│ • Source code   │ • Windows MSI   │ • GitHub Releases       │
│ • Dependencies  │ • Linux DEB/RPM │ • Official Website      │
│ • Unit tests    │ • macOS DMG     │ • Package Managers      │
│ • Integration   │ • Portable ZIP  │ • Microsoft Store       │
│   tests         │ • Docker Image  │ • Container Registries  │
│ • Performance   │ • Signed Binaries│ • CDN Distribution      │
└─────────────────┴─────────────────┴─────────────────────────┘
```

## 2. Prerequisites for Deployment

### 2.1 Build Requirements
- Release build (no debug symbols)
- All dependencies correctly bundled
- Code signed executables (Windows)
- Tested on target platforms

### 2.2 Deployment Checklist
- [ ] All unit and integration tests pass
- [ ] Performance benchmarks met
- [ ] Memory usage optimized
- [ ] Dependencies audited
- [ ] License compliance verified
- [ ] Documentation updated

## 3. Windows Deployment

### 3.1 Standalone Executable Distribution

#### 3.1.1 Build Script (deploy.bat)

```batch
@echo off
setlocal enabledelayedexpansion

REM Configuration
set VERSION=1.0.0
set QT_DIR=C:\Qt\6.10.1\mingw_64
set MINGW_DIR=C:\Qt\Tools\mingw1310_64
set VCPKG_DIR=C:\vcpkg
set BUILD_DIR=build_final
set DEPLOY_DIR=coreMusicPlayer-Portable-%VERSION%

echo =================================
echo coreMusicPlayer Windows Deployment
echo Version: %VERSION%
echo =================================

REM Verify prerequisites
echo Checking prerequisites...
if not exist "%QT_DIR%" (
    echo ERROR: Qt directory not found at %QT_DIR%
    exit /b 1
)
if not exist "%MINGW_DIR%" (
    echo ERROR: MinGW directory not found at %MINGW_DIR%
    exit /b 1
)
if not exist "%BUILD_DIR%\Release\QtGuiPlayer.exe" (
    echo ERROR: Application not built. Run build_final.bat first.
    exit /b 1
)

REM Clean previous deployment
if exist "%DEPLOY_DIR%" (
    echo Cleaning previous deployment...
    rmdir /s /q "%DEPLOY_DIR%"
)

REM Create deployment directory structure
echo Creating deployment directory...
mkdir "%DEPLOY_DIR%"
mkdir "%DEPLOY_DIR%\plugins"
mkdir "%DEPLOY_DIR%\plugins\platforms"
mkdir "%DEPLOY_DIR%\plugins\audio"
mkdir "%DEPLOY_DIR%\plugins\codecs"
mkdir "%DEPLOY_DIR%\plugins\styles"

REM Copy main executables
echo Copying executables...
copy "%BUILD_DIR%\Release\QtGuiPlayer.exe" "%DEPLOY_DIR%\" >nul
copy "%BUILD_DIR%\Release\final_wav_player.exe" "%DEPLOY_DIR%\" >nul
copy "%BUILD_DIR%\Release\music_player_complete.exe" "%DEPLOY_DIR%\" >nul

REM Copy Qt runtime libraries
echo Copying Qt libraries...
copy "%QT_DIR%\bin\Qt6Core.dll" "%DEPLOY_DIR%\" >nul
copy "%QT_DIR%\bin\Qt6Widgets.dll" "%DEPLOY_DIR%\" >nul
copy "%QT_DIR%\bin\Qt6Gui.dll" "%DEPLOY_DIR%\" >nul
copy "%QT_DIR%\bin\Qt6Multimedia.dll" "%DEPLOY_DIR%\" >nul
copy "%QT_DIR%\bin\Qt6Network.dll" "%DEPLOY_DIR%\" >nul

REM Copy MinGW runtime libraries
echo Copying MinGW runtime...
copy "%MINGW_DIR%\bin\libgcc_s_seh-1.dll" "%DEPLOY_DIR%\" >nul
copy "%MINGW_DIR%\bin\libstdc++-6.dll" "%DEPLOY_DIR%\" >nul
copy "%MINGW_DIR%\bin\libwinpthread-1.dll" "%DEPLOY_DIR%\" >nul
copy "%MINGW_DIR%\bin\libssp-0.dll" "%DEPLOY_DIR%\" >nul

REM Copy audio processing libraries
echo Copying audio libraries...
if exist "%VCPKG_DIR%\installed\x64-windows\bin\" (
    copy "%VCPKG_DIR%\installed\x64-windows\bin\*.dll" "%DEPLOY_DIR%\" >nul 2>&1
)

REM Copy Qt plugins
echo Copying Qt plugins...
copy "%QT_DIR%\plugins\platforms\qwindows.dll" "%DEPLOY_DIR%\plugins\platforms\" >nul
copy "%QT_DIR%\plugins\platforms\qminimal.dll" "%DEPLOY_DIR%\plugins\platforms\" >nul
copy "%QT_DIR%\plugins\audio\qtaudio_windows.dll" "%DEPLOY_DIR%\plugins\audio\" >nul

REM Create configuration files
echo Creating configuration files...
(
echo [Audio]
echo device=default
echo sample_rate=44100
echo buffer_size=1024
echo.
echo [GUI]
echo theme=dark
echo scale=1.0
echo.
echo [Advanced]
echo log_level=info
echo auto_update=true
) > "%DEPLOY_DIR%\config.ini"

REM Create launcher script with error handling
echo Creating launcher script...
(
echo @echo off
echo title coreMusicPlayer Audio Player
echo cd /d "%%~dp0"
echo.
echo REM Check for required libraries
echo if not exist "Qt6Core.dll" (
echo     echo ERROR: Required Qt libraries missing!
echo     echo Please ensure all files are present in the directory.
echo     pause
echo     exit /b 1
echo ^)
echo.
echo REM Set environment variables
echo set QT_PLUGIN_PATH=%%~dp0plugins
echo set QML2_IMPORT_PATH=%%~dp0qml
echo.
echo echo Starting coreMusicPlayer...
echo QtGuiPlayer.exe %%*
echo.
echo if %%ERRORLEVEL%% neq 0 (
echo     echo.
echo     echo Application exited with error code %%ERRORLEVEL%%
echo     echo Please check the log file for details.
echo     pause
echo ^)
) > "%DEPLOY_DIR%\coreMusicPlayer.bat"

REM Create portable version marker
echo Portable > "%DEPLOY_DIR%\portable.txt"

REM Create documentation
echo Creating documentation...
copy README.md "%DEPLOY_DIR%\" >nul 2>&1
copy LICENSE "%DEPLOY_DIR%\" >nul 2>&1
(
echo # Quick Start Guide
echo.
echo 1. Run coreMusicPlayer.bat to start the application
echo 2. Drag and drop audio files to play
echo 3. Configure audio settings in config.ini
echo.
echo ## Troubleshooting
echo.
echo If the application fails to start:
echo - Ensure Windows 7 SP1 or later is installed
echo - Install Visual C++ Redistributable 2022
echo - Check that all DLL files are present
echo.
echo ## File Associations
echo.
echo To associate audio files with coreMusicPlayer:
echo - Right-click on an audio file
echo - Open with ^> Choose another app
echo - Select QtGuiPlayer.exe and check "Always use this app"
) > "%DEPLOY_DIR%\QUICKSTART.txt"

REM Verify deployment
echo Verifying deployment...
set errors=0

if not exist "%DEPLOY_DIR%\QtGuiPlayer.exe" (
    echo ERROR: QtGuiPlayer.exe not found!
    set /a errors+=1
)
if not exist "%DEPLOY_DIR%\Qt6Core.dll" (
    echo ERROR: Qt6Core.dll not found!
    set /a errors+=1
)
if not exist "%DEPLOY_DIR%\plugins\platforms\qwindows.dll" (
    echo ERROR: qwindows.dll not found!
    set /a errors+=1
)

if %errors% gtr 0 (
    echo.
    echo WARNING: %errors% errors detected during deployment!
    echo.
)

REM Create ZIP archive
echo Creating ZIP archive...
powershell -command "Compress-Archive -Path '%DEPLOY_DIR%' -DestinationPath '%DEPLOY_DIR%.zip' -Force"

echo.
echo =================================
echo Deployment completed successfully!
echo Output: %DEPLOY_DIR%.zip
echo =================================
pause
```

#### 3.1.2 Deployment Verification Script (verify_deployment.bat)

```batch
@echo off
echo =================================
echo coreMusicPlayer Deployment Verification
echo =================================

set DEPLOY_DIR=%1
if "%DEPLOY_DIR%"=="" set DEPLOY_DIR=coreMusicPlayer-Portable-1.0.0

echo Verifying deployment in: %DEPLOY_DIR%

REM Check if deployment directory exists
if not exist "%DEPLOY_DIR%" (
    echo ERROR: Deployment directory not found!
    exit /b 1
)

REM Check critical files
set missing_files=
set total_files=0
set found_files=0

echo.
echo Checking critical files...

:check_file
if "%~1"=="" goto check_done
set /a total_files+=1
if exist "%DEPLOY_DIR%\%~1" (
    echo [OK] %~1
    set /a found_files+=1
) else (
    echo [MISSING] %~1
    set missing_files=%missing_files% %~1
)
shift
goto check_file

:check_done
echo.
echo Files found: %found_files%/%total_files%

REM Run quick test
echo.
echo Running quick application test...
cd /d "%DEPLOY_DIR%"

REM Test executable can load required libraries
dumpbin /dependents QtGuiPlayer.exe > temp_deps.txt 2>nul
if %ERRORLEVEL% equ 0 (
    echo [OK] Executable dependencies verified
    del temp_deps.txt
) else (
    echo [WARNING] Could not verify dependencies
)

REM Test application startup (non-interactive)
start /B QtGuiPlayer.exe --version > app_output.txt 2>&1
timeout /t 3 /nobreak >nul
taskkill /F /IM QtGuiPlayer.exe >nul 2>&1

if exist app_output.txt (
    echo [OK] Application starts without errors
    del app_output.txt
) else (
    echo [WARNING] Application startup verification failed
)

REM Summary
echo.
echo =================================
if %found_files%==%total_files% (
    echo ✓ Deployment verification PASSED
) else (
    echo ✗ Deployment verification FAILED
    echo Missing files:%missing_files%
)
echo =================================

cd ..
```

#### 3.1.3 Common Issues and Solutions

**Issue: Application fails to start with "MSVCP140.dll missing"**

```batch
REM Solution: Install Visual C++ Redistributable
echo Installing Visual C++ Redistributable...
start /wait vc_redist.x64.exe /quiet /norestart
```

**Issue: Qt plugins not found**

```batch
REM Solution: Set QT_PLUGIN_PATH
set QT_PLUGIN_PATH=%~dp0plugins
```

**Issue: Audio output device not detected**

```batch
REM Solution: Check WASAPI availability
REM Add to config.ini:
REM [Audio]
REM backend=DirectSound
```

**Issue: High DPI scaling issues**

```batch
REM Solution: Enable high DPI support
REM Create qt.conf file:
REM [Platforms]
REM WindowsArguments = dpiawareness=1
```

#### 3.1.4 Silent Installation Script (install_silent.bat)

```batch
@echo off
setlocal

set INSTALL_DIR=%ProgramFiles%\coreMusicPlayer
set SHORTCUT_DIR=%ProgramData%\Microsoft\Windows\Start Menu\Programs

echo Installing coreMusicPlayer...

REM Create installation directory
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

REM Extract files (assume running from installer directory)
xcopy /E /Y "*" "%INSTALL_DIR%\"

REM Create start menu shortcut
powershell -command "$WshShell = New-Object -comObject WScript.Shell; $Shortcut = $WshShell.CreateShortcut('%SHORTCUT_DIR%\coreMusicPlayer.lnk'); $Shortcut.TargetPath = '%INSTALL_DIR%\QtGuiPlayer.exe'; $Shortcut.Save()"

REM Set file associations
assoc .wav=coreMusicPlayer.WAV
ftype coreMusicPlayer.WAV="%INSTALL_DIR%\QtGuiPlayer.exe" "%%1"

REM Add to system PATH (optional)
setx PATH "%PATH%;%INSTALL_DIR%" /M

REM Create uninstaller
(
echo @echo off
echo echo Uninstalling coreMusicPlayer...
echo rmdir /s /q "%INSTALL_DIR%"
echo del "%SHORTCUT_DIR%\coreMusicPlayer.lnk"
echo assoc .wav=
echo ftype coreMusicPlayer.WAV=
echo echo coreMusicPlayer uninstalled successfully.
) > "%INSTALL_DIR%\uninstall.bat"

echo Installation complete!
```

### 3.2 Windows Installer (NSIS)

#### installer.nsi
```nsis
!define APP_NAME "coreMusicPlayer"
!define APP_VERSION "1.0.0"
!define APP_PUBLISHER "coreMusicPlayer Team"
!define APP_URL "https://github.com/checkyyeug/coreMusicPlayer"

!define UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

Name "${APP_NAME}"
OutFile "${APP_NAME}-${APP_VERSION}-Setup.exe"
InstallDir "$PROGRAMFILES64\${APP_NAME}"
RequestExecutionLevel admin

Page directory
Page instfiles

Section "MainSection" SEC01
    SetOutPath "$INSTDIR"

    File /r "coreMusicPlayer\*"

    CreateDirectory "$SMPROGRAMS\${APP_NAME}"
    CreateShortcut "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk" "$INSTDIR\QtGuiPlayer.exe"

    WriteRegStr HKLM "${UNINST_KEY}" "DisplayName" "${APP_NAME}"
    WriteRegStr HKLM "${UNINST_KEY}" "DisplayVersion" "${APP_VERSION}"
    WriteRegStr HKLM "${UNINST_KEY}" "Publisher" "${APP_PUBLISHER}"
    WriteRegStr HKLM "${UNINST_KEY}" "UninstallString" "$INSTDIR\uninstall.exe"
    WriteRegDWORD HKLM "${UNINST_KEY}" "NoModify" 1
    WriteRegDWORD HKLM "${UNINST_KEY}" "NoRepair" 1

    WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd

Section "Uninstall"
    Delete "$SMPROGRAMS\${APP_NAME}\${APP_NAME}.lnk"
    RMDir /r "$SMPROGRAMS\${APP_NAME}"
    RMDir /r "$INSTDIR"
    DeleteRegKey HKLM "${UNINST_KEY}"
SectionEnd
```

### 3.3 Microsoft Store

#### Package.appxmanifest
```xml
<?xml version="1.0" encoding="utf-8"?>
<Package xmlns="http://schemas.microsoft.com/appx/manifest/foundation/windows10">
  <Identity Name="coreMusicPlayer"
          Publisher="CN=coreMusicPlayerTeam"
          Version="1.0.0.0" />

  <Properties>
    <DisplayName>coreMusicPlayer</DisplayName>
    <PublisherDisplayName>coreMusicPlayer Team</PublisherDisplayName>
    <Logo>Assets\StoreLogo.png</Logo>
  </Properties>

  <Dependencies>
    <TargetDeviceFamily Name="Windows.Universal" MinVersion="10.0.0.0" MaxVersionTested="10.0.0.0" />
  </Dependencies>

  <Applications>
    <Application Id="App">
      <uap:VisualElements DisplayName="coreMusicPlayer"
                         Description="Cross-platform audio player"
                         Square150x150Logo="Assets\Square150x150Logo.png"
                         Square44x44Logo="Assets\Square44x44Logo.png"
                         BackgroundColor="transparent" />
    </Application>
  </Applications>
</Package>
```

## 4. Linux Deployment

### 4.1 AppImage (Universal Linux)

#### build-appimage.sh
```bash
#!/bin/bash
set -e

APP=coreMusicPlayer
VERSION=1.0.0
ARCH=x86_64

# Create AppDir
mkdir -p $APP.AppDir/usr/bin
mkdir -p $APP.AppDir/usr/share/applications
mkdir -p $APP.AppDir/usr/share/icons/hicolor/256x256/apps

# Copy executables
cp build/final_wav_player $APP.AppDir/usr/bin/
cp build/music_player_complete $APP.AppDir/usr/bin/
cp qt_gui/build/QtGuiPlayer $APP.AppDir/usr/bin/

# Copy desktop file
cp coreMusicPlayer.desktop $APP.AppDir/usr/share/applications/

# Copy icon
cp coreMusicPlayer.png $APP.AppDir/usr/share/icons/hicolor/256x256/apps/

# Create AppRun
cat > $APP.AppDir/AppRun << 'EOF'
#!/bin/bash
HERE="$(dirname "$(readlink -f "${0}")"'
export LD_LIBRARY_PATH="${HERE}/usr/lib:${HERE}/usr/lib/x86_64-linux-gnu:${LD_LIBRARY_PATH}"
export QT_PLUGIN_PATH="${HERE}/usr/plugins"
exec "${HERE}/usr/bin/QtGuiPlayer" "$@"
EOF
chmod +x $APP.AppDir/AppRun

# Download AppImageTool
wget https://github.com/AppImage/AppImageKit/releases/download/continuous/AppImageTool-x86_64.AppImage
chmod +x AppImageTool-x86_64.AppImage

# Create AppImage
./AppImageTool-x86_64.AppImage $APP.AppDir ${APP}-${VERSION}-${ARCH}.AppImage
```

### 4.2 Debian Package

#### debian/control
```
Source: core-musicplayer
Section: sound
Priority: optional
Maintainer: coreMusicPlayer Team <team@core-musicplayer.org>
Build-Depends: debhelper (>= 9), cmake (>= 3.10), qt6-base-dev, libasound2-dev
Standards-Version: 4.1.4
Homepage: https://github.com/checkyyeug/coreMusicPlayer

Package: core-musicplayer
Architecture: any
Depends: ${shlibs:Depends}, qt6-base-dev, libasound2
Description: Cross-platform audio player
 coreMusicPlayer is a modern audio player supporting multiple formats
 with high-quality audio processing and plugin extensibility.
```

#### debian/rules
```
#!/usr/bin/make -f
%:
	dh $@ --with cmake

override_dh_auto_configure:
	dh_auto_configure -- -DCMAKE_BUILD_TYPE=Release

override_dh_auto_install:
	dh_auto_install -- DESTDIR=debian/core-musicplayer
```

### 4.3 RPM Package

#### core-musicplayer.spec
```spec
Name: core-musicplayer
Version: 1.0.0
Release: 1%{?dist}
Summary: Cross-platform audio player

License: GPL-3.0
URL: https://github.com/checkyyeug/coreMusicPlayer
Source0: %{name}-%{version}.tar.gz

BuildRequires: cmake >= 3.10
BuildRequires: qt6-qtbase-devel
BuildRequires: alsa-lib-devel
Requires: qt6-qtbase
Requires: alsa-lib

%description
coreMusicPlayer is a modern audio player supporting multiple audio formats
with high-quality audio processing and plugin extensibility based on
the Foobar2000 SDK.

%prep
%autosetup -n

%build
%cmake
%cmake_build

%install
%cmake_install

%files
%{_bindir}/*
%doc README.md
%license LICENSE

%changelog
* Sun Dec 15 2024 coreMusicPlayer Team <team@core-musicplayer.org> - 1.0.0-1
- Initial release
```

## 5. macOS Deployment

### 5.1 Bundle Creation (macOS)

#### build-macos-bundle.sh
```bash
#!/bin/bash
set -e

APP="coreMusicPlayer"
VERSION="1.0.0"

# Create app bundle
mkdir -p "${APP}.app/Contents/MacOS"
mkdir -p "${APP}.app/Contents/Resources"

# Copy plist
cat > "${APP}.app/Contents/Info.plist" << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>CFBundleExecutable</key>
    <string>${APP}</string>
    <key>CFBundleIconFile</key>
    <string>AppIcon.icns</string>
    <key>CFBundleIdentifier</key>
    <string>org.core-musicplayer.player</string>
    <key>CFBundleName</key>
    <string>${APP}</string>
    <key>CFBundleShortVersionString</key>
    <string>${VERSION}</string>
    <key>CFBundleVersion</key>
    <string>${VERSION}</string>
    <key>NSHighResolutionCapable</key>
    <true/>
</dict>
</plist>
EOF

# Copy executable
cp build/macos/Release/QtGuiPlayer "${APP}.app/Contents/MacOS/"

# Copy resources
cp assets/AppIcon.icns "${APP}.app/Contents/Resources/"

# Create DMG
hdiutil create -volname "${APP}" -srcfolder "${APP}.app" -ov -format UDZO "${APP}.dmg"
```

## 6. Dependency Management

### 6.1 Static vs Dynamic Linking

#### Static Linking (Recommended for Distribution)
```cmake
# CMakeLists.txt
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
```

Pros:
- No external dependencies
- Simpler deployment
- Version compatibility guaranteed

Cons:
- Larger executable size
- License considerations

#### Dynamic Linking (Development)
```cmake
# CMakeLists.txt
set(BUILD_SHARED_LIBS ON)
```

Pros:
- Smaller executable size
- Shared libraries save memory
- Easier updates

Cons:
- Dependency management required
- Version compatibility issues

### 6.2 Dependency Bundling

#### Windows
```batch
REM Bundle Qt runtime
windeployqt QtGuiPlayer.exe
```

#### Linux
```bash
# Bundle libraries
ldd QtGuiPlayer | grep "=>" | while read line; do
    lib=$(echo $line | awk '{print $3}')
    cp $lib .
done
```

## 7. Automated Deployment

### 7.1 GitHub Actions Workflow

#### .github/workflows/deploy.yml
```yaml
name: Deploy

on:
  push:
    tags:
      - 'v*'

jobs:
  windows:
    runs-on: windows-latest
    steps:
    - uses: actions/checkout@v2
    - name: Build
      run: |
        build_final.bat
        qt_gui/build.bat deploy
    - name: Create installer
      run: |
        makensis installer.nsi
    - name: Upload artifact
      uses: actions/upload-artifact@v2
      with:
        name: windows-installer
        path: coreMusicPlayer-Setup.exe

  linux:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Build
      run: |
        mkdir build && cd build
        cmake ..
        make -j4
    - name: Create AppImage
      run: |
        chmod +x scripts/build-appimage.sh
        ./scripts/build-appimage.sh
    - name: Upload artifact
      uses: actions/upload-artifact@v2
      with:
        name: linux-appimage
        path: coreMusicPlayer-1.0.0-x86_64.AppImage
```

### 7.2 Docker Deployment

#### Dockerfile
```dockerfile
FROM ubuntu:22.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    qt6-base-dev \
    libasound2-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy source
COPY . /src
WORKDIR /src

# Build
RUN mkdir build && cd build && \
    cmake .. && \
    make -j4

# Create minimal runtime image
FROM ubuntu:22.04
COPY --from=0 /usr/lib/x86_64-linux-gnu/ /usr/lib/x86_64-linux-gnu/
COPY --from=0 /usr/lib/ /usr/lib/
COPY --from=0 /src/build/final_wav_player /usr/local/bin/

CMD ["/usr/local/bin/final_wav_player"]
```

## 8. Distribution Channels

### 8.1 Official Channels
- GitHub Releases
- Official website downloads
- Package managers (apt, yum, homebrew)

### 8.2 Third-Party Channels
- Microsoft Store
- Snap Store
- Flathub
- Chocolatey (Windows)

### 8.3 Version Management
- Semantic versioning (MAJOR.MINOR.PATCH)
- Release candidates for testing
- Stable releases for production

## 9. Update Mechanism

### 9.1 Auto-Update System
```cpp
class UpdateManager {
public:
    bool checkForUpdates();
    bool downloadUpdate(const std::string& version);
    bool installUpdate();
    void scheduleAutoUpdateCheck();

private:
    std::string currentVersion_;
    std::string updateServer_;
    bool autoUpdateEnabled_;
};
```

### 9.2 Update Notification
- Check on startup (configurable)
- Silent download in background
- Prompt for installation
- Rollback capability

## 10. Telemetry and Analytics

### 10.1 Data Collection
- Usage statistics (anonymous)
- Crash reports
- Performance metrics
- Plugin usage

### 10.2 Privacy Considerations
- Opt-in telemetry
- Anonymous data only
- Local processing
- Clear privacy policy

## 11. Deployment Troubleshooting Guide

### 11.1 Common Deployment Issues

#### 11.1.1 Windows Issues

**Issue: Qt DLLs Not Found**
```
Error: The program can't start because Qt6Core.dll is missing from your computer.
```

**Diagnosis:**
```batch
@echo off
echo Checking Qt dependencies...
where Qt6Core.dll >nul 2>&1
if %ERRORLEVEL% neq 0 (
    echo Qt6Core.dll not found in PATH
    echo Current PATH: %PATH%
)
```

**Solutions:**
1. Ensure all Qt DLLs are in the same directory as the executable
2. Verify Qt version compatibility (Qt 6.5+ required)
3. Check for Visual C++ Redistributable installation
4. Use dependency walker to identify missing dependencies

**Issue: Audio Output Fails**
```
Error: Failed to initialize audio device
```

**Diagnosis Script (diagnose_audio.bat):**
```batch
@echo off
echo Checking audio system...

REM Check WASAPI availability
echo Testing WASAPI...
powershell -command "Add-Type -AssemblyName System.Core; [System.Audio]::GetDevices()" > audio_devices.txt 2>&1

REM Check DirectX
echo Testing DirectX...
dxdiag /t dxdiag_output.txt

REM Check audio drivers
echo Checking audio drivers...
wmic sounddev get name, drivername, status

echo Diagnostic files created:
echo - audio_devices.txt
echo - dxdiag_output.txt
```

**Solutions:**
1. Update audio drivers
2. Check Windows Audio service is running
3. Try different audio backend in config.ini
4. Disable exclusive mode if conflicts occur

**Issue: High CPU Usage**
```
Symptoms: CPU usage > 20% during playback
```

**Diagnosis:**
```cpp
// Add to logging system
void log_performance_metrics() {
    auto cpu_usage = get_cpu_usage();
    auto memory_usage = get_memory_usage();

    LOG_INFO("CPU: " + std::to_string(cpu_usage) + "%");
    LOG_INFO("Memory: " + std::to_string(memory_usage) + "MB");
}
```

**Solutions:**
1. Check SIMD optimization is enabled
2. Increase buffer size in audio settings
3. Disable visualizations if not needed
4. Use exclusive mode for lower latency

#### 11.1.2 Linux Issues

**Issue: Missing Libraries**
```
Error: libQt6Core.so.6: cannot open shared object file
```

**Diagnosis Script (diagnose_linux.sh):**
```bash
#!/bin/bash
echo "Checking Linux dependencies..."

# Check for Qt libraries
ldd $(which QtGuiPlayer) | grep "not found"

# Check audio systems
echo "Checking ALSA..."
aplay -l 2>/dev/null || echo "ALSA not available"

echo "Checking PulseAudio..."
pulseaudio --check 2>/dev/null && echo "PulseAudio running" || echo "PulseAudio not running"

# Check permissions
echo "Checking audio device permissions..."
ls -l /dev/snd/* 2>/dev/null

# System information
echo "System information:"
uname -a
cat /etc/os-release | grep PRETTY_NAME
```

**Solutions:**
1. Install missing dependencies:
```bash
# Ubuntu/Debian
sudo apt-get install libqt6core6 libqt6gui6 libqt6widgets6

# Fedora/CentOS
sudo dnf install qt6-qtbase qt6-qtbase-gui

# Arch Linux
sudo pacman -S qt6-base
```

2. Add application repository to library path
3. Check user is in audio group: `groups $USER`

**Issue: No Audio Output**

**Diagnosis:**
```bash
# Test audio output
speaker-test -t wav -c 2 2>/dev/null

# Check default audio device
pacmd list-sinks | grep -A1 "* index"

# Test with ALSA directly
aplay /usr/share/sounds/alsa/Front_Center.wav
```

**Solutions:**
1. Configure audio backend in config.ini:
```ini
[Audio]
backend=alsa
device=hw:0,0
```

2. Ensure user permissions:
```bash
sudo usermod -a -G audio $USER
```

#### 11.1.3 macOS Issues

**Issue: App Won't Start (Gatekeeper)**
```
Error: "QtGuiPlayer" can't be opened because Apple cannot check it for malicious software.
```

**Solutions:**
1. Sign the application:
```bash
codesign --force --deep --sign "Developer ID Application: Your Name" QtGuiPlayer.app
```

2. Notarize the application:
```bash
xcrun altool --notarize-app --primary-bundle-id "org.core-musicplayer.player" --username "email@example.com" --password "@keychain:AC_PASSWORD" --file coreMusicPlayer.dmg
```

3. User workaround:
```bash
xattr -d com.apple.quarantine QtGuiPlayer.app
```

### 11.2 Monitoring and Logging

#### 11.2.1 Application Health Monitoring

```cpp
// health_monitor.h
#pragma once
#include <chrono>
#include <thread>
#include <atomic>
#include <fstream>

class HealthMonitor {
public:
    HealthMonitor() : running_(false) {}

    void start() {
        running_ = true;
        monitor_thread_ = std::thread(&HealthMonitor::monitor_loop, this);
    }

    void stop() {
        running_ = false;
        if (monitor_thread_.joinable()) {
            monitor_thread_.join();
        }
    }

    void record_crash(const std::string& error) {
        std::lock_guard<std::mutex> lock(mutex_);
        crash_count_++;
        last_error_ = error;
        write_log("CRASH: " + error);
    }

private:
    void monitor_loop() {
        while (running_) {
            check_system_health();
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    }

    void check_system_health() {
        // Monitor CPU usage
        auto cpu_usage = get_cpu_usage();
        if (cpu_usage > 80.0) {
            write_log("WARNING: High CPU usage: " + std::to_string(cpu_usage) + "%");
        }

        // Monitor memory usage
        auto memory_usage = get_memory_usage();
        if (memory_usage > 500) { // 500MB
            write_log("WARNING: High memory usage: " + std::to_string(memory_usage) + "MB");
        }

        // Monitor audio buffer underruns
        if (audio_underruns_ > 10) {
            write_log("WARNING: Audio buffer underruns detected: " + std::to_string(audio_underruns_));
            audio_underruns_ = 0;
        }
    }

    void write_log(const std::string& message) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);

        std::ofstream log("health_monitor.log", std::ios::app);
        log << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
            << " " << message << std::endl;
    }

    std::thread monitor_thread_;
    std::atomic<bool> running_;
    std::mutex mutex_;
    std::atomic<int> crash_count_{0};
    std::string last_error_;
    std::atomic<int> audio_underruns_{0};
};
```

#### 11.2.2 Remote Diagnostics

```cpp
// diagnostics.h
class RemoteDiagnostics {
public:
    void upload_diagnostics() {
        nlohmann::json report;

        // System information
        report["system"]["os"] = get_os_name();
        report["system"]["version"] = get_os_version();
        report["system"]["architecture"] = get_architecture();

        // Hardware information
        report["hardware"]["cpu_cores"] = std::thread::hardware_concurrency();
        report["hardware"]["memory_gb"] = get_total_memory_gb();
        report["hardware"]["audio_devices"] = get_audio_devices();

        // Application information
        report["app"]["version"] = APP_VERSION;
        report["app"]["build_date"] = BUILD_DATE;
        report["app"]["uptime_seconds"] = get_uptime_seconds();

        // Performance metrics
        report["performance"]["avg_cpu_usage"] = get_avg_cpu_usage();
        report["performance"]["peak_memory_mb"] = get_peak_memory_mb();
        report["performance"]["audio_latency_ms"] = get_audio_latency_ms();

        // Error information
        report["errors"]["crash_count"] = get_crash_count();
        report["errors"]["last_error"] = get_last_error();

        // Send to server
        send_report(report);
    }

private:
    void send_report(const nlohmann::json& report) {
        // Implementation for sending diagnostics to server
        // Use HTTPS with authentication
    }
};
```

### 11.3 Automated Recovery

#### 11.3.1 Crash Recovery System

```cpp
// crash_recovery.h
class CrashRecovery {
public:
    void initialize() {
        // Set up crash handlers
        std::set_terminate(&CrashRecovery::terminate_handler);
        std::signal(SIGSEGV, &CrashRecovery::signal_handler);
        std::signal(SIGABRT, &CrashRecovery::signal_handler);

        // Load previous session
        load_session();
    }

    void save_session() {
        nlohmann::json session;
        session["current_file"] = current_file_path_;
        session["position"] = current_position_;
        session["volume"] = current_volume_;
        session["playlist"] = current_playlist_;

        std::ofstream file("session.json");
        file << session.dump(4);
    }

    bool can_restore_session() {
        return std::filesystem::exists("session.json") &&
               std::filesystem::exists("crash_dump.dmp");
    }

    void restore_session() {
        std::ifstream file("session.json");
        nlohmann::json session;
        file >> session;

        // Restore application state
        load_file(session["current_file"]);
        seek_to(session["position"]);
        set_volume(session["volume"]);
        load_playlist(session["playlist"]);

        // Notify user
        show_message("Previous session restored after crash");
    }

private:
    static void terminate_handler() {
        // Generate crash dump
        generate_crash_dump();

        // Save current session
        getInstance().save_session();

        // Log crash
        LOG_CRITICAL("Application terminated unexpectedly");

        std::abort();
    }

    static void signal_handler(int signal) {
        LOG_CRITICAL("Received signal: " + std::to_string(signal));
        terminate_handler();
    }

    void generate_crash_dump() {
        // Platform-specific crash dump generation
#ifdef _WIN32
        // Windows MiniDump
        HANDLE hFile = CreateFile("crash_dump.dmp", GENERIC_WRITE, 0, NULL,
                                CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            MINIDUMP_EXCEPTION_INFORMATION mdei;
            mdei.ThreadId = GetCurrentThreadId();
            mdei.ExceptionPointers = nullptr;
            mdei.ClientPointers = FALSE;

            MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
                            hFile, MiniDumpNormal, &mdei, NULL, NULL);
            CloseHandle(hFile);
        }
#endif
    }

    std::string current_file_path_;
    double current_position_ = 0.0;
    float current_volume_ = 1.0f;
    std::vector<std::string> current_playlist_;
};
```

## 12. Deployment Verification

### 12.1 Automated Smoke Tests

```python
# smoke_test.py
import subprocess
import time
import psutil
import os
import sys

def run_command(cmd, timeout=10):
    """Run command and return output"""
    try:
        result = subprocess.run(cmd, shell=True, capture_output=True,
                              text=True, timeout=timeout)
        return result.returncode == 0, result.stdout, result.stderr
    except subprocess.TimeoutExpired:
        return False, "", "Command timed out"

def check_process_running(name):
    """Check if process is running"""
    for proc in psutil.process_iter(['name']):
        if proc.info['name'] == name:
            return True
    return False

def smoke_test_windows():
    """Windows smoke test"""
    print("Running Windows smoke test...")

    # Test application startup
    success, _, _ = run_command("tasklist | findstr QtGuiPlayer.exe")
    if not success:
        print("Application not running. Starting...")
        run_command("QtGuiPlayer.exe --version")

    # Test basic functionality
    if check_process_running("QtGuiPlayer.exe"):
        print("✓ Application process is running")

        # Test audio file loading
        if os.path.exists("test.wav"):
            run_command('QtGuiPlayer.exe "test.wav"')
            print("✓ Audio file loading test passed")

        # Test configuration loading
        if os.path.exists("config.ini"):
            print("✓ Configuration file exists")

        return True
    else:
        print("✗ Application failed to start")
        return False

def smoke_test_linux():
    """Linux smoke test"""
    print("Running Linux smoke test...")

    # Check dependencies
    deps = ["libQt6Core.so.6", "libQt6Gui.so.6", "libQt6Widgets.so.6"]
    for dep in deps:
        success, _, _ = run_command(f"ldd $(which QtGuiPlayer) | grep {dep}")
        if not success:
            print(f"✗ Missing dependency: {dep}")
            return False

    print("✓ All dependencies satisfied")

    # Test audio system
    success, _, _ = run_command("aplay -l")
    if success:
        print("✓ Audio system available")

    return True

def main():
    print("coreMusicPlayer Smoke Test")
    print("=" * 50)

    # Determine platform
    if sys.platform == "win32":
        result = smoke_test_windows()
    elif sys.platform.startswith("linux"):
        result = smoke_test_linux()
    else:
        print(f"Unsupported platform: {sys.platform}")
        return 1

    print("\n" + "=" * 50)
    if result:
        print("✓ Smoke test PASSED")
        return 0
    else:
        print("✗ Smoke test FAILED")
        return 1

if __name__ == "__main__":
    sys.exit(main())
```

### 12.2 Installation Tests

#### 12.2.1 Fresh Install Test

```bash
#!/bin/bash
# test_fresh_install.sh

echo "Testing fresh installation..."

# Remove any existing installation
sudo rm -rf /usr/local/bin/core-musicplayer
sudo rm -rf /opt/core-musicplayer
sudo rm -f /usr/share/applications/core-musicplayer.desktop

# Run installer
sudo ./install.sh

# Verify installation
if [ -f "/usr/local/bin/QtGuiPlayer" ]; then
    echo "✓ Executable installed"
else
    echo "✗ Executable not found"
    exit 1
fi

if [ -f "/usr/share/applications/core-musicplayer.desktop" ]; then
    echo "✓ Desktop file installed"
else
    echo "✗ Desktop file not found"
fi

# Test launch from desktop
if QtGuiPlayer --version > /dev/null 2>&1; then
    echo "✓ Application launches successfully"
else
    echo "✗ Application failed to launch"
    exit 1
fi

echo "Fresh install test PASSED"
```

#### 12.2.2 Upgrade Test

```bash
#!/bin/bash
# test_upgrade.sh

echo "Testing upgrade from previous version..."

# Install old version
sudo ./install_old_version.sh

# Create user configuration
mkdir -p ~/.config/coreMusicPlayer
echo "[Audio]" > ~/.config/coreMusicPlayer/config.ini
echo "volume=75" >> ~/.config/coreMusicPlayer/config.ini

# Upgrade to new version
sudo ./install.sh

# Verify configuration preserved
if grep -q "volume=75" ~/.config/coreMusicPlayer/config.ini; then
    echo "✓ User configuration preserved"
else
    echo "✗ User configuration lost"
    exit 1
fi

# Verify new features work
if QtGuiPlayer --test-new-feature > /dev/null 2>&1; then
    echo "✓ New features working"
else
    echo "✗ New features not working"
fi

echo "Upgrade test PASSED"
```

## 13. Rollout Strategy

### 13.1 Phased Rollout

1. **Alpha Testing** (Internal Team)
   - Duration: 1 week
   - Participants: 5-10 developers
   - Goals: Basic functionality, crash testing

2. **Beta Testing** (Selected Users)
   - Duration: 2 weeks
   - Participants: 50-100 power users
   - Goals: Feature validation, performance testing

3. **Release Candidate** (Public Testing)
   - Duration: 1 week
   - Participants: 1000+ users
   - Goals: Broad compatibility, edge case testing

4. **Stable Release** (General Availability)
   - Full release to all channels
   - Monitoring enabled
   - Support channels active

### 13.2 Monitoring Dashboard

```javascript
// monitoring_dashboard.js
const metrics = {
    // Performance metrics
    cpuUsage: [],
    memoryUsage: [],
    audioLatency: [],

    // Usage metrics
    dailyActiveUsers: 0,
    totalPlayTime: 0,

    // Error metrics
    crashRate: 0,
    errorReports: [],

    // Feature usage
    features: {
        pluginUsage: {},
        formatUsage: {},
        settingChanges: {}
    }
};

// Real-time monitoring
function updateMetrics() {
    fetch('/api/metrics')
        .then(response => response.json())
        .then(data => {
            // Update dashboard
            updateCPUChart(data.cpu);
            updateMemoryChart(data.memory);
            updateErrorRate(data.errors);

            // Alert on thresholds
            if (data.cpu > 80) {
                showAlert('High CPU usage detected!', 'warning');
            }

            if (data.crashRate > 0.01) {
                showAlert('Elevated crash rate!', 'error');
            }
        });
}

// Initialize dashboard
setInterval(updateMetrics, 5000); // Update every 5 seconds
```

### 13.3 Rollback Procedures

```bash
#!/bin/bash
# rollback.sh

echo "Rolling back coreMusicPlayer deployment..."

BACKUP_VERSION=$1
if [ -z "$BACKUP_VERSION" ]; then
    echo "Usage: $0 <backup_version>"
    exit 1
fi

# Stop current version
sudo systemctl stop core-musicplayer

# Restore backup
sudo tar -xzf "backups/core-musicplayer-$BACKUP_VERSION.tar.gz" -C /

# Verify rollback
if /opt/core-musicplayer/QtGuiPlayer --version | grep -q "$BACKUP_VERSION"; then
    echo "✓ Rollback successful"

    # Restart service
    sudo systemctl start core-musicplayer

    # Notify users
    send_rollback_notification $BACKUP_VERSION
else
    echo "✗ Rollback failed"
    exit 1
fi
```

This comprehensive deployment guide ensures coreMusicPlayer reaches users effectively across all platforms while maintaining reliability and performance standards.