#!/bin/bash
# 构建脚本 - 用于验证Foobar2000集成

echo "正在构建coreMusicPlayer项目..."

# 创建build目录
mkdir -p build
cd build

# 使用CMake配置项目
cmake ..

# 编译项目
cmake --build .

if [ $? -eq 0 ]; then
    echo "构建成功！"
    echo "Foobar2000集成组件已正确编译。"
else
    echo "构建失败！"
    exit 1
fi

cd ..
echo "构建完成。"