@echo off
REM 构建脚本 - 用于验证Foobar2000集成

echo 正在构建coreMusicPlayer项目...

REM 创建build目录
mkdir build
cd build

REM 使用CMake配置项目
cmake ..

REM 编译项目
cmake --build .

if %ERRORLEVEL% == 0 (
    echo 构建成功！
    echo Foobar2000集成组件已正确编译。
) else (
    echo 构建失败！
    exit /b 1
)

cd ..
echo 构建完成。