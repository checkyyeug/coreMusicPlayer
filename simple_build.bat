@echo off
echo Building simplified coreMusicPlayer for Windows...

REM Create a minimal build with only working components
if not exist "simple_build" mkdir simple_build
cd simple_build

REM Configure with CMake - exclude problematic components
cmake .. -G "Visual Studio 16 2019" -A x64 -DENABLE_GUI=OFF -DCMAKE_CXX_FLAGS="/wd4819"

if %ERRORLEVEL% NEQ 0 (
    echo CMake configuration failed!
    exit /b 1
)

REM Build only core components that should work
cmake --build . --config Release --target coreMusicPlayer

if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    exit /b 1
)

echo Build completed successfully!
echo Output executable: simple_build\Release\coreMusicPlayer.exe