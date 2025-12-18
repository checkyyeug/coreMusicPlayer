# CMakeLists.txt for coreMusicPlayer

cmake_minimum_required(VERSION 3.14)
project(coreMusicPlayer VERSION 1.0.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置构建类型默认为Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 查找必要的包
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)

# 包含目录
include_directories(${PROJECT_SOURCE_DIR}/include)

# 定义源文件
set(SOURCES
    src/core_music_player.cpp
    src/core/result.cpp
    src/core/error.cpp
    src/core/config_manager.cpp
    src/core/feature_manager.cpp
    src/core/unified_music_player.cpp
    src/core/strategy_factory.cpp
    src/core/strategies/legacy_strategy.cpp
    src/core/strategies/complete_strategy.cpp
    src/core/strategies/realtime_strategy.cpp
    src/core/strategies/production_strategy.cpp
    src/core/strategies/multi_format_strategy.cpp
    src/core/equalizer_config.cpp
    src/audio/audio_engine.cpp
    src/audio/audio_format.cpp
    src/audio/audio_buffer.cpp
    src/audio/device_manager.cpp
    src/audio/decoder_manager.cpp
    src/audio/decoder_factory.cpp
    src/audio/sample_rate_converter.cpp
    src/audio/dsp/volume_control.cpp
    src/audio/dsp/equalizer.cpp
    src/audio/decoders/wav_decoder.cpp
    src/audio/decoders/mp3_decoder.cpp
    src/audio/decoders/flac_decoder.cpp
    src/audio/decoders/ogg_decoder.cpp
    src/audio/simd/resampler_sse.cpp
    src/audio/simd/resampler_avx.cpp
    src/platform/platform_utils.cpp
    src/platform/file_utils.cpp
    src/platform/thread_manager.cpp
    src/platform/memory_manager.cpp
    src/utils/logger.cpp
    src/utils/performance_counter.cpp
    src/utils/debug_tools.cpp
    src/foobar/foobar_plugin_manager.cpp
    src/foobar/foobar_input_adapter.cpp
    src/foobar/foobar_dsp_adapter.cpp
    src/foobar/foobar_output_adapter.cpp
    src/gui/main_window.cpp
    src/gui/theme_manager.cpp
    src/main.cpp
)

# 创建主可执行文件
add_executable(coreMusicPlayer ${SOURCES})

# 链接系统库
target_link_libraries(coreMusicPlayer 
    ${CMAKE_THREAD_LIBS_INIT}
    GTest::gtest
    GTest::gtest_main
)

# 针对不同平台的特定设置
if(WIN32)
    target_compile_definitions(coreMusicPlayer PRIVATE _WIN32_WINNT=0x0601)
    target_link_libraries(coreMusicPlayer 
        winmm
        ole32
        oleaut32
    )
elseif(UNIX AND NOT APPLE)
    # Linux特定设置
    target_link_libraries(coreMusicPlayer 
        pthread
        dl
    )
endif()

# 针对不同构建类型的优化
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(coreMusicPlayer PRIVATE 
        $<$<COMPILE_LANGUAGE:CXX>:-O3>
        $<$<COMPILE_LANGUAGE:CXX>:-DNDEBUG>
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(coreMusicPlayer PRIVATE 
        $<$<COMPILE_LANGUAGE:CXX>:-g>
        $<$<COMPILE_LANGUAGE:CXX>:-O0>
    )
endif()

# 添加测试
enable_testing()

# 测试源文件
set(TEST_SOURCES
    tests/core_test.cpp
    tests/audio_buffer_test.cpp
    tests/audio_engine_test.cpp
    tests/core_functionality_test.cpp
    tests/audio_functionality_test.cpp
    tests/core_utility_test.cpp
    tests/performance_test.cpp
    tests/foobar_unit_test.cpp
    tests/foobar_integration_test.cpp
    tests/end_to_end_tests.cpp
    tests/platform_tests.cpp
    tests/regression_tests.cpp
    tests/user_experience_tests.cpp
)

# 创建测试可执行文件
add_executable(coreMusicPlayerTests ${TEST_SOURCES})

# 链接测试库
target_link_libraries(coreMusicPlayerTests 
    coreMusicPlayer
    GTest::gtest
    GTest::gtest_main
)

# 添加测试
add_test(NAME coreMusicPlayerTests COMMAND coreMusicPlayerTests)