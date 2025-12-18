#!/bin/bash

# 构建测试
echo "Building tests..."
mkdir -p build
cd build
cmake ..
make

# 运行测试
echo "Running all tests..."
./end_to_end_tests
./platform_tests  
./regression_tests
./user_experience_tests

echo "All tests completed successfully."