# DSP module CMakeLists.txt

add_library(dsp STATIC
    equalizer.cpp
    volume_control.cpp
)

target_include_directories(dsp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/audio/dsp
)

target_link_libraries(dsp PRIVATE 
    audio
)