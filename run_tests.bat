@echo off
REM 构建测试
echo Building tests...
mkdir build
cd build
cmake ..
cmake --build .

REM 运行测试
echo Running tests...
.\end_to_end_tests.exe
.\platform_tests.exe
.\regression_tests.exe
.\user_experience_tests.exe

echo Test execution completed.