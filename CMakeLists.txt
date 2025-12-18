# CMakeLists.txt for tests

cmake_minimum_required(VERSION 3.14)
project(coreMusicPlayerTests)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Google Test
find_package(GTest REQUIRED)

# 包含测试源文件
add_executable(end_to_end_tests tests/end_to_end_tests.cpp)
target_link_libraries(end_to_end_tests GTest::gtest GTest::gtest_main)

add_executable(platform_tests tests/platform_tests.cpp)
target_link_libraries(platform_tests GTest::gtest GTest::gtest_main)

add_executable(regression_tests tests/regression_tests.cpp)
target_link_libraries(regression_tests GTest::gtest GTest::gtest_main)

add_executable(user_experience_tests tests/user_experience_tests.cpp)
target_link_libraries(user_experience_tests GTest::gtest GTest::gtest_main)

# 添加Foobar2000集成测试
add_executable(foobar_integration_test tests/foobar_integration_test.cpp)
target_link_libraries(foobar_integration_test GTest::gtest GTest::gtest_main)

# 添加Foobar2000单元测试
add_executable(foobar_unit_test tests/foobar_unit_test.cpp)
target_link_libraries(foobar_unit_test GTest::gtest GTest::gtest_main)

# 添加测试
add_test(NAME end_to_end_tests COMMAND end_to_end_tests)
add_test(NAME platform_tests COMMAND platform_tests)
add_test(NAME regression_tests COMMAND regression_tests)
add_test(NAME user_experience_tests COMMAND user_experience_tests)
add_test(NAME foobar_integration_test COMMAND foobar_integration_test)
add_test(NAME foobar_unit_test COMMAND foobar_unit_test)