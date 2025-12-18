# Main executable source files
set(SOURCES
    core_music_player.cpp
    audio/audio_engine.cpp
    audio/audio_format.cpp
    audio/audio_buffer.cpp
    audio/device_manager.cpp
    audio/decoder_manager.cpp
    audio/sample_rate_converter.cpp
    core/equalizer_config.cpp
    core/feature_manager.cpp
    core/config_manager.cpp
    core/unified_music_player.cpp
    core/strategy_factory.cpp
    core/strategies/legacy_strategy.cpp
    core/strategies/complete_strategy.cpp
    core/strategies/realtime_strategy.cpp
    core/strategies/production_strategy.cpp
    core/strategies/multi_format_strategy.cpp
    audio/dsp/equalizer.cpp
    audio/dsp/volume_control.cpp
    foobar/foobar_plugin_manager.cpp
    foobar/foobar_input_adapter.cpp
    foobar/foobar_dsp_adapter.cpp
    foobar/foobar_output_adapter.cpp
)

# Create main executable
add_executable(coreMusicPlayer ${SOURCES})

# Link with audio library
target_link_libraries(coreMusicPlayer audio)