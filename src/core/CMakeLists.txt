# Core module CMakeLists.txt

add_library(core_lib STATIC
    result.cpp
    error.cpp
    feature_manager.cpp
    config_manager.cpp
    unified_music_player.cpp
    strategy_factory.cpp
    strategies/legacy_strategy.cpp
    strategies/complete_strategy.cpp
    strategies/realtime_strategy.cpp
    strategies/production_strategy.cpp
    strategies/multi_format_strategy.cpp
    equalizer_config.cpp
)

target_include_directories(core_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include/core
)

target_link_libraries(core_lib PRIVATE Qt6::Core)