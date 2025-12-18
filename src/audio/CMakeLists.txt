# Audio source files
set(AUDIO_SOURCES
    audio_buffer.cpp
    audio_format.cpp
    device_manager.cpp
    decoder_manager.cpp
    sample_rate_converter.cpp
    audio_engine.cpp
)

# Create library for audio components
add_library(audio STATIC ${AUDIO_SOURCES})

# Add DSP module
add_subdirectory(dsp)

# Set include directories - using the correct relative path from build directory
target_include_directories(audio PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# Link with DSP module
target_link_libraries(audio PRIVATE dsp)