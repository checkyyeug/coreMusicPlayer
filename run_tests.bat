# Run tests script for Windows

@echo off
echo Running coreMusicPlayer tests...

REM Change to build directory
cd build

REM Run the test executable
coreMusicPlayerTests.exe

if %ERRORLEVEL% NEQ 0 (
    echo Tests failed!
    exit /b 1
)

echo All tests passed!