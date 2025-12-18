@echo off
REM coreMusicPlayer build script

echo Building coreMusicPlayer...
echo.

REM Create build directory
if not exist "build" mkdir "build"
cd build

REM Configure with CMake
echo Configuring with CMake...
cmake .. -G "Visual Studio 16 2019" -A x64

if %errorlevel% neq 0 (
    echo Error: CMake configuration failed
    exit /b 1
)

REM Build the project
echo Building project...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo Error: Build failed
    exit /b 1
)

echo.
echo Build completed successfully!
echo.
echo Executable is located at: build\Release\coreMusicPlayer.exe