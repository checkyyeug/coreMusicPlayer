# Test module CMakeLists.txt

find_package(GTest REQUIRED)

add_executable(core_tests
    core_test.cpp
    audio_buffer_test.cpp
    equalizer_tests.cpp
)

add_executable(audio_engine_tests
    audio_engine_test.cpp
)

target_include_directories(core_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_include_directories(audio_engine_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

target_link_libraries(core_tests
    core_lib
    audio_lib
    GTest::gtest
    GTest::gtest_main
)

target_link_libraries(audio_engine_tests
    audio_lib
    GTest::gtest
    GTest::gtest_main
)