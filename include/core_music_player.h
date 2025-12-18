#pragma once

#include <string>
#include "core/result.h"

namespace coremusic {

/**
 * @brief Main music player interface for coreMusicPlayer
 */
class CoreMusicPlayer {
public:
    /**
     * @brief Initialize the music player
     * @return Result indicating success or error details
     */
    virtual core::Result<void> initialize() = 0;

    /**
     * @brief Shutdown the music player
     * @return Result indicating success or error details
     */
    virtual core::Result<void> shutdown() = 0;

    /**
     * @brief Load an audio file for playback
     * @param filename Path to the audio file
     * @return Result indicating success or error details
     */
    virtual core::Result<void> load_file(const std::string& filename) = 0;

    /**
     * @brief Start playback of loaded file
     * @return Result indicating success or error details
     */
    virtual core::Result<void> play() = 0;

    /**
     * @brief Pause current playback
     * @return Result indicating success or error details
     */
    virtual core::Result<void> pause() = 0;

    /**
     * @brief Stop current playback
     * @return Result indicating success or error details
     */
    virtual core::Result<void> stop() = 0;

    /**
     * @brief Get current player state
     * @return Current state as string
     */
    virtual std::string get_state() const = 0;

    /**
     * @brief Destructor
     */
    virtual ~CoreMusicPlayer() = default;
};

} // namespace coremusic