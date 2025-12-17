# coreMusicPlayer Source Code Examples

This document contains key code examples from the coreMusicPlayer project to help understand the implementation patterns and best practices.

## 0. Unified Player Architecture Examples (New)

**[重点]** The unified player implementation demonstrates how the 5 separate players have been consolidated into a single, mode-switchable architecture.

### 0.1 Unified Music Player Implementation

```cpp
// src/core/unified_music_player.h
#pragma once

#include "player_strategy.h"
#include "audio_engine.h"
#include "decoder_manager.h"
#include "feature_manager.h"
#include <memory>
#include <string>

/**
 * @brief Unified music player supporting multiple operational modes
 *
 * This class consolidates the 5 separate players (legacy, complete, realtime,
 * production, multi-format) into a single unified implementation using the
 * strategy pattern for mode switching.
 */
class UnifiedMusicPlayer {
public:
    /**
     * @brief Player modes corresponding to original separate players
     */
    enum class PlayerMode {
        Legacy,      // music_player_legacy equivalent
        Complete,    // music_player_complete equivalent
        Realtime,    // real_player equivalent
        Production,  // final_wav_player equivalent
        MultiFormat  // multi_format_player equivalent (default)
    };

    /**
     * @brief Constructor
     * @param mode Initial player mode
     */
    explicit UnifiedMusicPlayer(PlayerMode mode = PlayerMode::MultiFormat);

    /**
     * @brief Destructor
     */
    ~UnifiedMusicPlayer();

    // Core playback controls
    bool loadFile(const std::string& filename);
    void play();
    void pause();
    void stop();
    void seek(double position);

    // Volume control
    void setVolume(double volume);
    double getVolume() const;

    // Position and duration
    double getPosition() const;
    double getDuration() const;

    // Mode management
    bool setMode(PlayerMode mode);
    PlayerMode getCurrentMode() const { return current_mode_; }

    // Status queries
    bool isPlaying() const;
    bool isPaused() const;
    bool isStopped() const;
    std::string getCurrentTrack() const;

    // Feature management
    void enableFeature(FeatureManager::Features feature);
    void disableFeature(FeatureManager::Features feature);
    bool isFeatureEnabled(FeatureManager::Features feature) const;

private:
    PlayerMode current_mode_;
    std::unique_ptr<PlayerStrategy> strategy_;
    std::unique_ptr<AudioEngine> audio_engine_;
    std::unique_ptr<DecoderManager> decoder_manager_;
    std::unique_ptr<FeatureManager> feature_manager_;

    std::string current_file_;
    std::atomic<bool> is_playing_{false};
    std::atomic<bool> is_paused_{false};
    std::atomic<double> volume_{1.0};

    void initializeStrategy(PlayerMode mode);
    void updatePlaybackState();
};
```

### 0.2 Player Strategy Implementations

```cpp
// src/core/player_strategy.cpp
#include "player_strategy.h"
#include "audio_engine.h"
#include <iostream>

// Legacy Strategy - Minimal player for basic WAV playback
bool LegacyStrategy::initialize(AudioEngine* engine) {
    engine_ = engine;

    // Enable only basic features
    engine->enableFeature(FeatureManager::BasicPlayback);

    // Disable advanced features
    engine->disableFeature(FeatureManager::PluginSupport);
    engine->disableFeature(FeatureManager::AdvancedDSP);
    engine->disableFeature(FeatureManager::AudioVisualization);

    // Configure for minimal resource usage
    engine->setBufferSize(2048);
    engine->setMaxPreBufferCount(2);

    std::cout << "Initialized in Legacy mode - Basic WAV playback only" << std::endl;
    return true;
}

bool LegacyStrategy::loadFile(const std::string& filename) {
    // Only support WAV files in legacy mode
    if (filename.substr(filename.find_last_of('.') + 1) != "wav") {
        std::cerr << "Legacy mode only supports WAV files" << std::endl;
        return false;
    }

    return engine_->loadFile(filename);
}

void LegacyStrategy::optimizePlayback() {
    // No optimizations needed for basic playback
    engine_->setProcessingPriority(AudioEngine::Priority::Normal);
}

// Realtime Strategy - Low-latency playback optimized for live use
bool RealtimeStrategy::initialize(AudioEngine* engine) {
    engine_ = engine;

    // Enable real-time features
    engine->enableFeature(FeatureManager::BasicPlayback);
    engine->enableFeature(FeatureManager::RealtimeMode);
    engine->enableFeature(FeatureManager::AdvancedBuffering);
    engine->enableFeature(FeatureManager::SIMDOptimization);
    engine->enableFeature(FeatureManager::ErrorRecovery);

    // Configure for minimum latency
    engine->setBufferSize(64); // Very small buffer
    engine->setRealTimePriority(true);
    engine->setCPUAffinity(0); // Lock to first CPU core

    // Disable non-realtime features
    engine->disableFeature(FeatureManager::PluginSupport);
    engine->disableFeature(FeatureManager::AudioVisualization);

    std::cout << "Initialized in Realtime mode - Optimized for low latency" << std::endl;
    return true;
}

void RealtimeStrategy::optimizePlayback() {
    // Apply aggressive optimizations
    engine_->setProcessingPriority(AudioEngine::Priority::RealTime);
    engine_->minimizeBuffering();
    engine_->disableGarbageCollection();

    // Configure memory locking to prevent swapping
    engine_->lockMemoryPages();
}

// Production Strategy - Stable playback with all safety features
bool ProductionStrategy::initialize(AudioEngine* engine) {
    engine_ = engine;

    // Enable production features
    engine->enableFeature(FeatureManager::BasicPlayback);
    engine->enableFeature(FeatureManager::MultiFormat);
    engine->enableFeature(FeatureManager::AdvancedBuffering);
    engine->enableFeature(FeatureManager::SIMDOptimization);
    engine->enableFeature(FeatureManager::ErrorRecovery);

    // Optional: Enable plugins if configured
    if (enable_plugins_) {
        engine->enableFeature(FeatureManager::PluginSupport);
    }

    // Configure for stability
    engine->setBufferSize(1024);
    engine->setMaxPreBufferCount(4);
    engine->enableCrashRecovery();

    std::cout << "Initialized in Production mode - Optimized for stability" << std::endl;
    return true;
}

void ProductionStrategy::optimizePlayback() {
    engine_->setProcessingPriority(AudioEngine::Priority::High);
    engine_->enableDetailedLogging();
    engine_->startPerformanceMonitoring();
}
```

## 1. Core Audio Engine Examples

### 1.1 Enhanced Audio Buffer with Resource Pooling

**[重点]** Updated AudioBuffer implementation with RAII and SIMD optimizations:

```cpp
// src/audio/audio_buffer.h
#pragma once

#include <memory>
#include <vector>

/**
 * @brief High-performance audio buffer with SIMD alignment
 *
 * Provides storage for multi-channel audio data with SIMD-aligned
 * memory allocation for optimal performance with vectorized
 * operations. Supports both planar and interleaved formats.
 */
class AudioBuffer {
public:
    /**
     * @brief Constructor
     * @param channels Number of audio channels (1-8)
     * @param frames Number of audio frames
     */
    AudioBuffer(size_t channels, size_t frames);

    /**
     * @brief Constructor with external data
     * @param external_data Pre-allocated audio data
     * @param channels Number of audio channels
     * @param frames Number of audio frames
     * @param copy Create internal copy if true
     */
    AudioBuffer(float* external_data, size_t channels, size_t frames, bool copy = false);

    /**
     * @brief Destructor
     */
    ~AudioBuffer();

    // Copy and move semantics
    AudioBuffer(const AudioBuffer& other);
    AudioBuffer(AudioBuffer&& other) noexcept;
    AudioBuffer& operator=(const AudioBuffer& other);
    AudioBuffer& operator=(AudioBuffer&& other) noexcept;

    /**
     * @brief Get the number of channels
     * @return Number of channels
     */
    size_t getChannels() const noexcept { return channels_; }

    /**
     * @brief Get the number of frames
     * @return Number of frames
     */
    size_t getFrames() const noexcept { return frames_; }

    /**
     * @brief Get total size in samples
     * @return Total size (channels * frames)
     */
    size_t getSize() const noexcept { return channels_ * frames_; }

    /**
     * @brief Get pointer to audio data (interleaved)
     * @return Pointer to first sample
     */
    float* getData() noexcept { return data_; }

    /**
     * @brief Get const pointer to audio data (interleaved)
     * @return Pointer to first sample
     */
    const float* getData() const noexcept { return data_; }

    /**
     * @brief Get channel data (planar)
     * @param channel Channel index (0-based)
     * @return Pointer to channel data
     */
    float* getChannelData(size_t channel) noexcept {
        return data_ + channel * frames_;
    }

    /**
     * @brief Get const channel data (planar)
     * @param channel Channel index (0-based)
     * @return Pointer to channel data
     */
    const float* getChannelData(size_t channel) const noexcept {
        return data_ + channel * frames_;
    }

    /**
     * @brief Resize the buffer
     * @param frames New number of frames
     * @return True if resize was successful
     */
    bool resize(size_t frames);

    /**
     * @brief Clear all audio data to silence
     */
    void clear();

    /**
     * @brief Apply gain to all samples (SIMD optimized)
     * @param gain Gain factor (1.0 = no change)
     */
    void applyGain(float gain);

    /**
     * @brief Mix another buffer into this one (SIMD optimized)
     * @param other Buffer to mix in
     * @param gain Gain factor for mixed buffer
     */
    void mix(const AudioBuffer& other, float gain = 1.0f);

    /**
     * @brief Convert interleaved to planar format
     */
    void toPlanar();

    /**
     * @brief Convert planar to interleaved format
     */
    void toInterleaved();

private:
    float* data_;
    size_t channels_;
    size_t frames_;
    size_t capacity_;
    bool owns_data_;
    static constexpr size_t SIMD_ALIGNMENT = 32;

    void allocateMemory();
    void deallocateMemory();

    // SIMD implementations
    void applyGainAVX2(float* data, size_t samples, float gain);
    void applyGainSSE2(float* data, size_t samples, float gain);
    void mixAVX2(float* dest, const float* src, size_t samples, float gain);
    void mixSSE2(float* dest, const float* src, size_t samples, float gain);
};

/**
 * @brief RAII wrapper for pooled audio buffers
 */
class PooledAudioBuffer : public AudioBuffer {
private:
    class AudioBufferPool* pool_;

public:
    PooledAudioBuffer(float* data, size_t channels, size_t frames,
                     AudioBufferPool* pool);
    ~PooledAudioBuffer();
};
```

### 1.2 Decoder Manager with Lazy Loading (New)

**[重点]** Implementation of lazy loading decoder system:

```cpp
// src/audio/decoder_manager.cpp
#include "decoder_manager.h"
#include "wav_decoder.h"
#include "mp3_decoder.h"
#include "flac_decoder.h"
#include "ogg_decoder.h"
#include <iostream>

DecoderManager::DecoderManager()
    : buffer_pool_(std::make_unique<AudioBufferPool>()) {
    // Register decoder factories
    registerDefaultDecoders();
}

AudioDecoder* DecoderManager::getDecoder(const std::string& format) {
    std::shared_lock lock(decoder_mutex_);

    // Check if decoder is already loaded
    auto it = decoders_.find(format);
    if (it != decoders_.end()) {
        return it->second.get();
    }

    lock.unlock();

    // Need to load the decoder
    std::unique_lock write_lock(decoder_mutex_);

    // Double-check after acquiring write lock (double-checked locking)
    it = decoders_.find(format);
    if (it != decoders_.end()) {
        return it->second.get();
    }

    // Find decoder factory for this format
    auto factory_it = decoder_factories_.find(format);
    if (factory_it == decoder_factories_.end()) {
        std::cerr << "No decoder registered for format: " << format << std::endl;
        return nullptr;
    }

    // Create and initialize decoder
    auto decoder = factory_it->second();
    if (!decoder) {
        std::cerr << "Failed to create decoder for format: " << format << std::endl;
        return nullptr;
    }

    if (!decoder->initialize()) {
        std::cerr << "Failed to initialize decoder for format: " << format << std::endl;
        return nullptr;
    }

    // Store the decoder
    AudioDecoder* decoder_ptr = decoder.get();
    decoders_[format] = std::move(decoder);

    std::cout << "Loaded decoder for format: " << format << std::endl;
    return decoder_ptr;
}

void DecoderManager::preloadDecoder(const std::string& format) {
    // Force loading of specified decoder
    getDecoder(format);
}

void DecoderManager::preloadCommonDecoders() {
    // Preload most common decoders to avoid first-time loading delay
    std::thread([this]() {
        std::cout << "Preloading common decoders..." << std::endl;
        preloadDecoder("wav");  // Most common format
        preloadDecoder("mp3");  // Second most common
        // Don't preload all to avoid startup delay
    }).detach();
}

void DecoderManager::unloadDecoder(const std::string& format) {
    std::unique_lock lock(decoder_mutex_);
    auto it = decoders_.find(format);
    if (it != decoders_.end()) {
        it->second->cleanup();
        decoders_.erase(it);
        std::cout << "Unloaded decoder for format: " << format << std::endl;
    }
}

void DecoderManager::unloadUnusedDecoders(std::chrono::minutes idle_threshold) {
    std::unique_lock lock(decoder_mutex_);

    auto now = std::chrono::steady_clock::now();
    auto it = decoders_.begin();

    while (it != decoders_.end()) {
        // Check if decoder has been idle too long
        auto idle_time = now - it->second->getLastUsedTime();
        if (idle_time > idle_threshold && it->second->getReferenceCount() == 0) {
            std::cout << "Unloading idle decoder: " << it->first << std::endl;
            it->second->cleanup();
            it = decoders_.erase(it);
        } else {
            ++it;
        }
    }
}

std::unique_ptr<AudioBuffer> DecoderManager::decodeFile(
    const std::string& filename,
    size_t max_frames = 0) {

    // Extract format from file extension
    std::string format = filename.substr(filename.find_last_of('.') + 1);
    std::transform(format.begin(), format.end(), format.begin(), ::tolower);

    // Get appropriate decoder
    AudioDecoder* decoder = getDecoder(format);
    if (!decoder) {
        std::cerr << "No decoder available for file: " << filename << std::endl;
        return nullptr;
    }

    // Use buffer pool for output buffer
    auto buffer_handle = buffer_pool_->acquire(2, max_frames > 0 ? max_frames : 8192);
    auto buffer = std::make_unique<AudioBuffer>(
        buffer_handle->getChannelData(0),
        2,
        max_frames > 0 ? max_frames : 8192,
        true
    );

    // Decode file
    if (!decoder->decode(filename, *buffer)) {
        std::cerr << "Failed to decode file: " << filename << std::endl;
        return nullptr;
    }

    return buffer;
}

void DecoderManager::registerDefaultDecoders() {
    // Register built-in decoders
    decoder_factories_["wav"] = []() {
        return std::make_unique<WAVDecoder>();
    };
    decoder_factories_["mp3"] = []() {
        return std::make_unique<MP3Decoder>();
    };
    decoder_factories_["flac"] = []() {
        return std::make_unique<FLACDecoder>();
    };
    decoder_factories_["ogg"] = []() {
        return std::make_unique<OGGDecoder>();
    };

    std::cout << "Registered " << decoder_factories_.size()
              << " decoder factories" << std::endl;
}
```
```

### 1.2 Audio Buffer Implementation

```cpp
// src/audio/audio_buffer.cpp
#include "audio_buffer.h"
#include <algorithm>
#include <cstring>

AudioBuffer::AudioBuffer(size_t channels, size_t frames)
    : channels_(channels), frames_(frames) {
    allocateMemory();
}

AudioBuffer::~AudioBuffer() {
    freeMemory();
}

AudioBuffer::AudioBuffer(const AudioBuffer& other)
    : channels_(other.channels_), frames_(other.frames_) {
    allocateMemory();
    if (data_ && other.data_) {
        std::copy(other.data_.get(), other.data_.get() + getSize(), data_.get());
    }
}

AudioBuffer::AudioBuffer(AudioBuffer&& other) noexcept
    : channels_(other.channels_), frames_(other.frames_), data_(std::move(other.data_)) {
    other.channels_ = 0;
    other.frames_ = 0;
}

AudioBuffer& AudioBuffer::operator=(const AudioBuffer& other) {
    if (this != &other) {
        AudioBuffer temp(other);
        *this = std::move(temp);
    }
    return *this;
}

AudioBuffer& AudioBuffer::operator=(AudioBuffer&& other) noexcept {
    if (this != &other) {
        channels_ = other.channels_;
        frames_ = other.frames_;
        data_ = std::move(other.data_);
        other.channels_ = 0;
        other.frames_ = 0;
    }
    return *this;
}

bool AudioBuffer::resize(size_t frames) {
    if (frames == frames_) {
        return true;
    }

    size_t newSize = channels_ * frames;
    if (newSize > getSize()) {
        // Allocate new buffer
        auto newData = std::make_unique<float[]>(newSize, 0.0f);

        if (data_) {
            // Copy existing data
            size_t copySize = std::min(newSize, getSize());
            std::copy(data_.get(), data_.get() + copySize, newData.get());
        }

        data_ = std::move(newData);
    }

    frames_ = frames;
    return true;
}

void AudioBuffer::clear() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (data_) {
        std::fill(data_.get(), data_.get() + getSize(), 0.0f);
    }
}

void AudioBuffer::allocateMemory() {
    if (channels_ > 0 && frames_ > 0) {
        size_t totalSize = channels_ * frames_;
        data_ = std::make_unique<float[]>(totalSize);

        // Align memory for SIMD operations
        if (reinterpret_cast<uintptr_t>(data_.get()) % ALIGNMENT != 0) {
            data_.reset();
            data_ = static_cast<float*>(aligned_alloc(ALIGNMENT, totalSize * sizeof(float)));
        }
    }
}

void AudioBuffer::freeMemory() {
    if (data_ && reinterpret_cast<uintptr_t>(data_.get()) % ALIGNMENT == 0) {
        free_aligned(data_.release());
    }
}
```

## 2. Sample Rate Converter Examples

### 2.1 Sample Rate Converter Interface

```cpp
// src/audio/sample_rate_converter.h
#pragma once

#include <memory>
#include "audio_buffer.h"

/**
 * @brief Sample rate conversion with multiple quality modes
 *
 * Provides high-quality sample rate conversion between different
 * audio sample rates using various interpolation algorithms.
 */
class SampleRateConverter {
public:
    /**
     * @brief Quality modes for resampling
     */
    enum class Quality {
        Fast,       // Linear interpolation
        Good,       // Cubic interpolation
        Best,       // High-quality sinc interpolation
        High        // Ultra-high quality sinc
    };

    SampleRateConverter() = default;
    ~SampleRateConverter() = default;

    /**
     * @brief Convert audio from one sample rate to another
     * @param input Input audio buffer
     * @param output Output audio buffer (will be resized)
     * @param inputRate Input sample rate
     * @param outputRate Output sample rate
     * @param Quality Resampling quality mode
     * @return True if conversion was successful
     */
    bool resample(const AudioBuffer& input, AudioBuffer& output,
                  double inputRate, double outputRate,
                  Quality quality = Quality::Good);

    /**
     * @brief Get recommended output buffer size
     * @param inputFrames Input frame count
     * @param inputRate Input sample rate
     * @param outputRate Output sample rate
     * @param quality Resampling quality
     * @return Recommended output frame count
     */
    static size_t getOutputFrames(size_t inputFrames,
                                   double inputRate,
                                   double outputRate,
                                   Quality quality);
};
```

### 2.2 SIMD-Optimized Implementation

```cpp
// src/audio/simd_optimizations.cpp
#include "simd_optimizations.h"
#include <immintrin.h>

namespace simd {

// AVX-optimized volume adjustment
void adjust_volume_avx(float* data, size_t frames, float volume) {
    const __m256 volume_vec = _mm256_set1_ps(volume);
    const size_t simd_frames = frames & ~3;

    // Process 8 samples at a time (AVX width)
    for (size_t i = 0; i < simd_frames; i += 8) {
        __m256 samples = _256_load_ps(data + i);
        __m256 result = _256_mul_ps(samples, volume_vec);
        _256_store_ps(data + i, result);
    }

    // Process remaining samples
    for (size_t i = simd_frames; i < frames; ++i) {
        data[i] *= volume;
    }
}

// SSE2-optimized volume adjustment
void adjust_volume_sse2(float* data, size_t frames, float volume) {
    const __m128 volume_vec = _mm_set1_ps(volume);
    const size_t simd_frames = frames & ~3;

    // Process 4 samples at a time (SSE width)
    for (size_t i = 0; i < simd_frames; i += 4) {
        __m128 samples = _mm_load_ps(data + i);
        __m128 result = _mm_mul_ps(samples, volume_vec);
        _mm_store_ps(data + i, result);
    }

    // Process remaining samples
    for (size_t i = simd_frames; i < frames; ++i) {
        data[i] *= volume;
    }
}

// Scalar fallback implementation
void adjust_volume_scalar(float* data, size_t frames, float volume) {
    for (size_t i = 0; i < frames; ++i) {
        data[i] *= volume;
    }
}

// Automatic selection based on CPU capabilities
void adjust_volume(float* data, size_t frames, float volume) {
    if (hasAVX()) {
        adjust_volume_avx(data, frames, volume);
    } else if (hasSSE2()) {
        adjust_volume_sse2(data, frames, volume);
    } else {
        adjust_volume_scalar(data, frames, volume);
    }
}

} // namespace simd
```

### 2.3 Cubic Interpolation Example

```cpp
// src/audio/cubic_resampler.cpp
namespace {

// Cubic interpolation kernel
static inline float cubic_interpolate(float x0, float x1, float x2, float x3, float t) {
    return x1 + 0.5f * t * (x2 - x0) +
           t * t * (0.5f * (x3 - x1) - (x2 - x0));
}

// Resample using cubic interpolation
bool cubic_resample(const float* input, float* output,
                   size_t input_frames, size_t output_frames,
                   double input_rate, double output_rate) {
    const double ratio = input_rate / output_rate;
    const double phase_increment = ratio;

    for (size_t out_frame = 0; out_frame < output_frames; ++out_frame) {
        double position = out_frame * phase_increment;
        size_t input_index = static_cast<size_t>(position);
        double fraction = position - input_index;

        // Get 4 surrounding samples
        float x0, x1, x2, x3;

        x0 = (input_index > 0) ? input[input_index - 1] : 0.0f;
        x1 = input[input_index];
        x2 = (input_index + 1 < input_frames) ? input[input_index + 1] : 0.0f;
        x3 = (input_index + 2 < input_frames) ? input[input_index + 2] : 0.0f;

        // Interpolate for each channel
        for (int ch = 0; ch < channels; ++ch) {
            output[out_frame * channels + ch] = cubic_interpolate(
                x0, x1, x2, x3, fraction
            );
        }
    }

    return true;
}

} // namespace
```

## 3. Plugin System Examples

### 3.1 Plugin Interface Definition

```cpp
// include/plugin_interface.h
#pragma once

#include <string>
#include <vector>
#include <memory>

/**
 * @brief Base interface for all audio plugins
 */
class Plugin {
public:
    virtual ~Plugin() = default;

    /**
     * @brief Get plugin name
     * @return Plugin name
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Get plugin version
     * @return Version string
     */
    virtual std::getstring getVersion() const = 0;

    /**
     * @brief Initialize the plugin
     * @return True if successful
     */
    virtual bool initialize() = 0;

    /**
     * @brief Cleanup the plugin
     */
    virtual void cleanup() = 0;

    /**
     * @brief Get plugin type
     * @return Plugin type string
     */
    virtual std::string getType() const = 0;
};

/**
 * @brief Interface for audio decoder plugins
 */
class AudioDecoder : public Plugin {
public:
    /**
     * @brief Check if file can be decoded
     * @param filename File to check
     * @return True if decodable
     */
    virtual bool canDecode(const std::string& filename) = 0;

    /**
     * @brief Decode audio file
     * @param filename File to decode
     * @param buffer Output buffer
     * @return True if successful
     */
    virtual bool decode(const std::string& filename, AudioBuffer& buffer) = 0;

    /**
     * @brief Get audio format information
     * @param filename File to analyze
     * @param format Output format structure
     * @return True if successful
     */
    virtual bool getFormat(const std::string& filename, AudioFormat& format) = 0;
};
```

### 3.2 Plugin Manager Implementation

```cpp
// src/plugin_manager.cpp
#include "plugin_manager.h"
#include <filesystem>
#include <iostream>

bool PluginManager::loadPlugin(const std::string& path) {
    try {
        // Use platform-specific dynamic loading
#ifdef _WIN32
        HMODULE handle = LoadLibrary(path.c_str());
        if (!handle) {
            log_error("Failed to load plugin: " + path);
            return false;
        }
#else
        void* handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            log_error("Failed to load plugin: " + path);
            return false;
        }
#endif

        // Get plugin create function
        using CreatePluginFunc = Plugin* (*)();
#ifdef _WIN32
        CreatePluginFunc createFunc = reinterpret_cast<CreatePluginFunc>(
            GetProcAddress(handle, "createPlugin"));
#else
        createFunc = reinterpret_cast<CreatePluginFunc>(dlsym(handle, "createPlugin"));
#endif

        if (!createFunc) {
#ifdef _WIN32
            FreeLibrary(handle);
#else
            dlclose(handle);
#endif
            return false;
        }

        // Create plugin instance
        std::unique_ptr<Plugin> plugin(createFunc());
        if (!plugin) {
#ifdef _WIN32
            FreeLibrary(handle);
#else
            dlclose(handle);
#endif
            return false;
        }

        // Initialize plugin
        if (!plugin->initialize()) {
            log_error("Plugin initialization failed: " + plugin->getName());
            return false;
        }

        // Store plugin
        std::string name = plugin->getName();
        plugins_[name] = std::make_pair(std::move(handle), std::move(plugin));

        log_info("Loaded plugin: " + name);
        return true;
    }
    catch (const std::exception& e) {
        log_error("Exception loading plugin: " + std::string(e.what()));
        return false;
    }
}

void PluginManager::unloadPlugin(const std::string& name) {
    auto it = plugins_.find(name);
    if (it != plugins_.end()) {
        auto& [handle, plugin] = it->second;

        // Cleanup plugin
        plugin->cleanup();

        // Free library
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif

        plugins_.erase(it);
        log_info("Unloaded plugin: " + name);
    }
}

std::unique_ptr<AudioDecoder> PluginManager::getDecoder(const std::string& filename) {
    // Try to find a decoder that can handle this file
    for (const auto& [name, plugin_info] : plugins_) {
        if (auto decoder = dynamic_cast<AudioDecoder*>(plugin_info.second.get())) {
            if (decoder->canDecode(filename)) {
                // Create a copy for thread safety
                return std::make_unique<FoobarDecoderAdapter>(
                    filename, decoder);
            }
        }
    }
    return nullptr;
}
```

## 4. Qt GUI Examples

### 4.1 Main Window Implementation

```cpp
// qt_gui/mainwindow.cpp
#include "mainwindow.h"
#include "playerwidget.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , playerWidget_(nullptr)
    , playlistView_(nullptr)
    , mediaPlayer_(new QMediaPlayer(this))
    , playlist_(new QMediaPlaylist(this))
{
    setupUI();
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    connectSignals();

    mediaPlayer_->setPlaylist(playlist_);
}

void MainWindow::setupUI() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create player widget
    playerWidget_ = new PlayerWidget(this);
    mainLayout->addWidget(playerWidget_);

    // Create playlist view
    playlistView_ = new QListView(this);
    playlistView_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    mainLayout->addWidget(playlistView_);

    // Set window properties
    setWindowTitle("coreMusicPlayer");
    resize(800, 600);
}

void MainWindow::onOpenFile() {
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        "Open Audio Files",
        QStandardPaths::writableLocation(QStandardPaths::MusicLocation),
        tr("Audio Files (*.mp3 *.wav *.flac *.ogg);;All Files (*.*)")
    );

    if (!files.isEmpty()) {
        for (const QString& file : files) {
            playlist_->addMedia(QUrl::fromLocalFile(file));
        }

        // If playlist was empty, start playing first track
        if (playlist_->mediaCount() == files.size()) {
            mediaPlayer_->play();
        }
    }
}

void MainWindow::onPlayPause() {
    if (mediaPlayer_->state() == QMediaPlayer::PlayingState) {
        mediaPlayer_->pause();
    } else {
        mediaPlayer_->play();
    }
}
```

### 4.2 Player Widget Implementation

```cpp
// qt_gui/playerwidget.cpp
#include "playerwidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>

PlayerWidget::PlayerWidget(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

void PlayerWidget::setupUI() {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    // Control buttons
    QVBoxLayout *controlLayout = new QVBoxLayout();

    prevButton_ = createButton("Previous", SP_MediaSkipBackward);
    playButton_ = createButton("Play", SP_MediaPlay);
    stopButton_ = createButton("Stop", SP_MediaStop);
    nextButton_ = createButton("Next", SP_MediaSkipForward);

    controlLayout->addWidget(prevButton_);
    controlLayout->addWidget(playButton_);
    controlLayout->addWidget(stopButton_);
    controlLayout->addWidget(nextButton_);

    // Progress slider
    QHBoxLayout *progressLayout = new QHBoxLayout();
    positionSlider_ = new QSlider(Qt::Horizontal);
    positionSlider_->setRange(0, 0);
    positionSlider_->setEnabled(false);

    timeLabel_ = new QLabel("0:00 / 0:00");
    progressLayout->addWidget(timeLabel_);
    progressLayout->addWidget(positionSlider_);

    // Volume control
    QHBoxLayout *volumeLayout = new QHBoxLayout();
    volumeSlider_ = new QSlider(Qt::Horizontal);
    volumeSlider_->setRange(0, 100);
    volumeSlider_->setValue(50);

    QLabel *volumeIcon = new QLabel();
    volumeIcon->setPixmap(style()->standardIcon(
        QStyle::SP_MediaVolume).pixmap(16, 16));

    volumeLayout->addWidget(volumeIcon);
    volumeLayout->addWidget(volumeSlider_);

    // Main layout
    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(progressLayout);
    mainLayout->addLayout(volumeLayout);

    // Connect signals
    connect(playButton_, &QPushButton::clicked,
            this, &PlayerWidget::onPlayPauseClicked);
    connect(stopButton_, &QPushButton::clicked,
            this, &PlayerWidget::onStopClicked);
    connect(prevButton_, &QPushButton::clicked,
            this, &PlayerWidget::onPreviousClicked);
    connect(nextButton_, &QPushButton::clicked,
            this, &PlayerWidget::onNextClicked);
    connect(positionSlider_, &QSlider::sliderMoved,
            this, &PlayerWidget::onPositionSliderMoved);
    connect(volumeSlider_, &QSlider::valueChanged,
            this, &PlayerWidget::onVolumeChanged);
}

QPushButton* PlayerWidget::createButton(const QString& tooltip, QStyle::StandardPixmap icon) {
    QPushButton *button = new QPushButton();
    button->setIcon(style()->standardIcon(icon));
    button->setToolTip(tooltip);
    button->setFixedSize(32, 32);
    return button;
}

void PlayerWidget::onPlayPauseClicked() {
    emit playPauseRequested();

    // Update button icon
    if (playButton_->icon().name() == "SP_MediaPlay") {
        playButton_->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    } else {
        playButton_->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}
```

### 4.3 Playlist Model Implementation

```cpp
// qt_gui/playlistmodel.cpp
#include "playlistmodel.h"
#include <QFileInfo>

PlaylistModel::PlaylistModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PlaylistModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return tracks_.size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid() || index.row() >= tracks_.size())
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return QFileInfo(tracks_[index.row()]).fileName();
    case Qt::ToolTipRole:
        return QFileInfo(tracks_[index.row()]).absoluteFilePath();
    case Qt::UserRole:
        return tracks_[index.row()];
    default:
        return QVariant();
    }
}

bool PlaylistModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!index.isValid() || index.row() >= tracks_.size() || role != Qt::EditRole)
        return false;

    tracks_[index.row()] = value.toString();
    emit dataChanged(index, index);
    return true;
}

void PlaylistModel::addTrack(const QString &filePath) {
    beginInsertRows(QModelIndex(), tracks_.size(), tracks_.size());
    tracks_.append(filePath);
    endInsertRows();
}

void PlaylistModel::removeTrack(int index) {
    if (index < 0 || index >= tracks_.size())
        return;

    beginRemoveRows(QModelIndex(), index, index);
    tracks_.removeAt(index);
    endRemoveRows();
}

void PlaylistModel::clear() {
    if (tracks_.isEmpty())
        return;

    beginResetModel();
    tracks_.clear();
    endResetModel();
}

void PlaylistModel::moveTrack(int from, int to) {
    if (from < 0 || from >= tracks_.size() || to < 0 || to >= tracks_.size() || from == to)
        return;

    beginMoveRows(QModelIndex(), from, from, QModelIndex(), to <= from ? to : to + 1);
    std::swap(tracks_[from], tracks_[to]);
    endMoveRows();
}
```

## 5. Configuration System Examples

### 5.1 Configuration Manager Implementation

```cpp
// config/config_manager.cpp
#include "config_manager.h"
#include <fstream>
#include <filesystem>

ConfigManager::ConfigManager(const std::string& filename)
    : filename_(filename) {
    load();
}

ConfigManager::~ConfigManager() {
    save();
}

template<typename T>
T ConfigManager::get(const std::string& key, const T& defaultValue) {
    if (!config_.contains(key)) {
        return defaultValue;
    }

    try {
        return config_[key].get<T>();
    } catch (const nlohmann::json::exception& e) {
        log_error("Config error: " + std::string(e.what()));
        return defaultValue;
    }
}

template<typename T>
void ConfigManager::set(const std::string& key, const T& value) {
    config_[key] = value;
    modified_ = true;
}

void ConfigManager::load() {
    if (!std::filesystem::exists(filename_)) {
        return;
    }

    try {
        std::ifstream file(filename_);
        file >> config_;

        // Validate configuration
        if (!config_.is_object()) {
            config_ = nlohmann::json::object();
        }
    }
    catch (const std::exception& e) {
        log_error("Failed to load config: " + std::string(e.what()));
        config_ = nlohmann::json::object();
    }
}

void ConfigManager::save() {
    if (!modified_) {
        return;
    }

    try {
        std::ofstream file(filename_);
        file << config_ << std::endl;
        modified_ = false;
    }
    catch (const std::exception& e) {
        log_error("Failed to save config: " + std::string(e.what()));
    }
}

// Explicit instantiations for common types
template std::string ConfigManager::get<std::string>(const std::string& key, const std::string& defaultValue);
template int ConfigManager::get<int>(const std::string& key, int defaultValue);
template double ConfigManager::get<double>(const std::string& key, double defaultValue);
template bool ConfigManager::get<bool>(const std::string& key, bool defaultValue);

template void ConfigManager::set<std::string>(const std::string& key, const std::string& value);
template void ConfigManager::set<int>(const std::string& key, int value);
template void ConfigManager::set<double>(const std::string& key, double value);
template void ConfigManager::set<bool>(const std::string& key, bool value);
```

## 6. Platform-Specific Examples

### 6.1 WASAPI Implementation (Windows)

```cpp
// src/audio/wasapi_output.cpp
#include "wasapi_output.h"
#include <audioclient.h>
#include <mmdeviceapi.h>

WASAPIOutput::WASAPIOutput()
    : audioClient_(nullptr)
    , renderClient_(nullptr)
    , device_(nullptr)
    , bufferFrames_(0)
    , latency_(0.0)
{
}

WASAPIOutput::~WASAPIOutput() {
    cleanup();
}

bool WASAPIOutput::initialize(const AudioFormat& format) {
    HRESULT hr;

    // Get default audio device
    hr = deviceEnumerator_->GetDefaultAudioEndpoint(
        eRender, eConsole, &device_);
    if (FAILED(hr)) {
        log_error("Failed to get default audio endpoint");
        return false;
    }

    // Activate device
    hr = device_->Activate(
        __uuidof(IAudioClient),
        CLSCTX_ALL,
        nullptr,
        nullptr,
        (void**)&audioClient_
    );
    if (FAILED(hr)) {
        log_error("Failed to activate audio device");
        return false;
    }

    // Get device format
    WAVEFORMATEX *deviceFormat;
    hr = audioClient_->GetMixFormat(&deviceFormat);
    if (FAILED(hr)) {
        log_error("Failed to get device format");
        return false;
    }

    // Check if format is supported
    if (!isFormatSupported(*deviceFormat, format)) {
        log_error("Audio format not supported by device");
        return false;
    }

    // Convert to our format
    WAVEFORMATEX desiredFormat = {};
    desiredFormat.wFormatTag = WAVE_FORMAT_PCM;
    desiredFormat.nChannels = format.channels;
    desiredFormat.nSamplesPerSec = format.sampleRate;
    desiredFormat.wBitsPerSample = 32;
    desiredFormat.nBlockAlign = (desiredFormat.nChannels * desiredFormat.wBitsPerSample) / 8;
    desiredFormat.nAvgBytesPerSec = format.sampleRate * format.channels *
        desiredFormat.wBitsPerSample / 8;

    // Negotiate format
    hr = audioClient_->IsFormatSupported(
        AUDCL_SHAREMODE_SHARED,
        &desiredFormat, deviceFormat);
    if (hr == AUDCL_E_UNSUPPORTED_FORMAT) {
        log_warning("Device doesn't support format, trying exclusive mode");
        hr = audioClient->IsFormatSupported(
            AUDCL_SHAREMODE_EXCLUSIVE,
            &desiredFormat, deviceFormat);
    }

    if (FAILED(hr)) {
        log_error("Audio format not supported");
        return false;
    }

    // Initialize audio client
    hr = audioClient_->Initialize(
        AUDCL_SHAREMODE_SHARED,
        AUDCL_STREAM_PLAYBACK,
        &desiredFormat,
        0,
        nullptr
    );
    if (FAILED(hr)) {
        log_error("Failed to initialize audio client");
        return false;
    }

    // Get render client
    hr = audioClient_->GetService(__uuidof(IAudioRenderClient), &renderClient_);
    if (FAILED(hr)) {
        log_error("Failed to get render client");
        return false;
    }

    // Get buffer frame count
    hr = audioClient_->GetBufferSize(&bufferFrames_);
    if (FAILED(hr)) {
        log_error("Failed to get buffer size");
        return false;
    }

    // Get device period
    REFERENCE_TIME defaultPeriod, minimumPeriod;
    hr = audioClient_->GetDevicePeriod(&defaultPeriod, &minimumPeriod);
    if (SUCCEEDED(hr)) {
        latency_ = static_cast<double>(defaultPeriod) / 10000000.0;
    }

    // Success
    format_ = format;
    return true;
}

bool WASAPIOutput::write(const AudioBuffer& buffer) {
    if (!renderClient_ || !audioClient_) {
        return false;
    }

    // Wait for buffer
    UINT numFramesPadding = bufferFrames_ - buffer.getFrames();
    HRESULT hr = audioClient_->GetCurrentPadding(&numFramesPadding);
    if (FAILED(hr)) {
        return false;
    }

    UINT framesAvailable = buffer.getFrames() + numFramesPadding;
    BYTE *bufferData = nullptr;

    hr = renderClient_->GetBuffer(&bufferData, framesAvailable * format_.getFrameSize(),
        0, nullptr);
    if (FAILED(hr)) {
        return false;
    }

    // Copy audio data
    std::memcpy(bufferData, buffer.getData(),
                buffer.getSize() * sizeof(float));

    // Release buffer
    hr = renderClient_->ReleaseBuffer(framesAvailable);

    return SUCCEEDED(hr);
}
```

### 6.2 ALSA Implementation (Linux)

```cpp
// src/audio/alsa_output.cpp
#include "alsa_output.h"
#include <alsa/asoundlib.h>
#include <stdexcept>

ALSAOutput::ALSAOutput()
    : pcm_(nullptr)
    , hwParams_(nullptr)
    , swParams_(nullptr)
{
}

ALSAOutput::~ALSAOutput() {
    cleanup();
}

bool ALSAOutput::initialize(const AudioFormat& format) {
    int err;

    // Open PCM device
    err = snd_pcm_open(&pcm_, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        log_error("Cannot open PCM device: " + std::string(snd_strerror(err)));
        return false;
    }

    // Allocate hardware parameters object
    err = snd_pcm_hw_params_alloca(&hwParams_);
    if (err < 0) {
        log_error("Cannot allocate hardware parameters: " + std::string(snd_strerror(err)));
        cleanup();
        return false;
    }

    // Fill hardware parameters
    err = snd_pcm_hw_params_any(pcm_, hwParams_);
    if (err < 0) {
        log_error("Cannot initialize hardware parameters: " + std::string(snd_strerror(err)));
        snd_pcm_hw_params_free(hwParams_);
        cleanup();
        return false;
    }

    // Set access type
    err = snd_pcm_hw_params_set_access(pcm_, hwParams_, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (err < 0) {
        log_error("Cannot set access type: " + std::string(snd_strerror(err)));
        snd_pcm_hw_params_free(hwParams_);
        cleanup();
        return false;
    }

    // Set sample format
    err = snd_pcm_hw_params_set_format(pcm_, hwParams_, SND_PCM_FORMAT_S16_LE);
    if (err < 0) {
        log_error("Cannot set sample format: " + std::string(snd_strerror(err)));
        snd_pcm_hw_params_free(hwParams_);
        cleanup();
        return false;
    }

    // Set channels
    err = snd_pcm_hw_params_set_channels(pcm_, hwParams_, format.channels);
    if (err < 0) {
        log_error("Cannot set channel count: " + std::string(snd_strerror(err)));
        snd_pcm_hw_params_free(hwParams_);
        cleanup();
        return false;
    }

    // Set sample rate
    err = snd_pcm_hw_params_set_rate_near(pcm_, hwParams_,
                                      format.sampleRate, 0);
    if (err < 0) {
        log_error("Cannot set sample rate: " + std::string(snd_strerror(err)));
        snd_pcm_hw_params_free(hwParams_);
        cleanup();
        return false;
    }

    // Set hardware parameters
    err = snd_pcm_hw_params(pcm_, hwParams_);
    if (err < 0) {
        log_error("Cannot set hardware parameters: " + std::string(snd_strerror(err)));
        return false;
    }

    // Allocate software parameters object
    err = snd_pcm_sw_params_alloca(&swParams_);
    if (err < 0) {
        log_error("Cannot allocate software parameters: " + std::string(snd_strerror(err)));
        cleanup();
        return false;
    }

    // Start threading
    err = snd_pcm_sw_params_start_thread(pcm_, swParams_);
    if (err < 0) {
        log_warning("Cannot start threading: " + std::string(snd_strerror(err)));
        // Continue without threading
    }

    // Prepare device
    err = snd_pcm_prepare(pcm_);
    if (err < 0) {
        log_error("Cannot prepare device: " + std::string(snd_strerror(err)));
        snd_pcm_sw_params_free(swParams_);
        cleanup();
        return false;
    }

    format_ = format;
    return true;
}

bool ALSAOutput::write(const AudioBuffer& buffer) {
    if (!pcm_) {
        return false;
    }

    snd_pcm_sframes_t frames = buffer.getFrames();
    const float* data = buffer.getData();

    int result = snd_pcm_writei(pcm_, data, frames);
    if (result < 0) {
        if (result == -EPIPE) {
            log_warning("Audio buffer underrun");
        } else {
            log_error("ALSA write error: " + std::string(snd_strerror(result)));
        }
        return false;
    }

    return result == static_cast<snd_pcm_sframes_t>(frames);
}
```

## 7. Error Handling and Thread Safety Examples (New)

### 7.1 Result-Based Error Handling

```cpp
// src/core/audio_engine.cpp
#include "audio_engine.h"
#include "result.h"
#include "error.h"

VoidResult AudioEngine::initialize(const AudioConfig& config) {
    // Initialize with explicit error handling
    auto deviceResult = initializeAudioDevice(config);
    if (deviceResult.isError()) {
        return VoidResult::error(
            deviceResult.getError()
                .addContext("function", "AudioEngine::initialize")
                .addContext("device", config.deviceName)
        );
    }

    auto bufferResult = setupAudioBuffers(config);
    if (bufferResult.isError()) {
        // Cleanup on error
        cleanupAudioDevice();
        return VoidResult::error(bufferResult.getError());
    }

    auto threadResult = startAudioThread();
    if (threadResult.isError()) {
        cleanupAudioBuffers();
        cleanupAudioDevice();
        return VoidResult::error(threadResult.getError());
    }

    return VoidResult::success();
}

// Chained operations with error propagation
Result<std::unique_ptr<AudioBuffer>> AudioEngine::loadAndDecodeFile(
    const std::string& filename) {

    return openFile(filename)
        .flatMap([this](const FileHandle& file) {
            return detectFileFormat(file);
        })
        .flatMap([this](const AudioFormat& format) {
            return createDecoder(format);
        })
        .flatMap([this, filename](std::unique_ptr<AudioDecoder> decoder) {
            return decodeAudioFile(*decoder, filename);
        })
        .mapError([](const Error& err) {
            // Add additional context
            return Error(err.getCategory(),
                       err.getMessage() + " (during loadAndDecodeFile)",
                       err.getSeverity())
                .addContext("operation", "load_and_decode");
        });
}
```

### 7.2 Thread-Safe Resource Management

```cpp
// src/audio/thread_safe_buffer_pool.cpp
#include "thread_safe_buffer_pool.h"
#include <atomic>
#include <mutex>

class ThreadSafeBufferPool {
private:
    struct BufferSlot {
        alignas(64) std::atomic<AudioBuffer*> buffer{nullptr};
        alignas(64) std::atomic<bool> in_use{false};
    };

    std::vector<BufferSlot> slots_;
    mutable std::shared_mutex pool_mutex_;
    std::atomic<size_t> next_slot_{0};

public:
    std::unique_ptr<PooledBufferHandle> acquire(size_t channels, size_t frames) {
        // Fast path - try lock-free acquisition
        for (size_t i = 0; i < slots_.size(); ++i) {
            size_t idx = (next_slot_.fetch_add(1, std::memory_order_relaxed) % slots_.size());

            bool expected = false;
            if (slots_[idx].in_use.compare_exchange_strong(
                    expected, true, std::memory_order_acquire)) {

                AudioBuffer* buffer = slots_[idx].buffer.load(std::memory_order_acquire);
                if (buffer && buffer->getChannels() >= channels &&
                    buffer->getFrames() >= frames) {
                    return std::make_unique<PooledBufferHandle>(buffer, this);
                }

                // Buffer not suitable, release and continue
                slots_[idx].in_use.store(false, std::memory_order_release);
            }
        }

        // Slow path - need to allocate new buffer
        std::unique_lock lock(pool_mutex_);
        return allocateNewBuffer(channels, frames);
    }

    void release(AudioBuffer* buffer) {
        if (!buffer) return;

        // Find the slot containing this buffer
        for (auto& slot : slots_) {
            if (slot.buffer.load(std::memory_order_acquire) == buffer) {
                slot.in_use.store(false, std::memory_order_release);
                return;
            }
        }

        // Buffer not found in pool, delete it
        delete buffer;
    }

private:
    std::unique_ptr<PooledBufferHandle> allocateNewBuffer(size_t channels, size_t frames) {
        auto* buffer = new AudioBuffer(channels, frames);

        // Find an empty slot
        for (auto& slot : slots_) {
            if (slot.buffer.load(std::memory_order_acquire) == nullptr) {
                slot.buffer.store(buffer, std::memory_order_release);
                slot.in_use.store(true, std::memory_order_release);
                return std::make_unique<PooledBufferHandle>(buffer, this);
            }
        }

        // No empty slots, append new slot
        slots_.push_back({buffer, true});
        return std::make_unique<PooledBufferHandle>(buffer, this);
    }
};
```

### 7.3 Lock-Free Audio Processing Pipeline

```cpp
// src/audio/lock_free_pipeline.cpp
#include "lock_free_pipeline.h"
#include "concurrent_queue.h"

class LockFreeAudioPipeline {
private:
    struct AudioPacket {
        std::unique_ptr<AudioBuffer> buffer;
        std::chrono::high_resolution_clock::time_point timestamp;
        uint32_t sequence_number;
    };

    // Lock-free queues for pipeline stages
    LockFreeQueue<AudioPacket> input_queue_{256};
    LockFreeQueue<AudioPacket> processing_queue_{256};
    LockFreeQueue<AudioPacket> output_queue_{256};

    // Atomic state
    std::atomic<bool> is_running_{false};
    std::atomic<uint32_t> sequence_counter_{0};

    // Processing threads
    std::thread decoder_thread_;
    std::thread processor_thread_;

public:
    VoidResult start() {
        bool expected = false;
        if (!is_running_.compare_exchange_strong(expected, true)) {
            return VoidResult::error(
                Error(ErrorCategory::InvalidState,
                      "Pipeline already running"));
        }

        try {
            decoder_thread_ = std::thread(&LockFreeAudioPipeline::decoderLoop, this);
            processor_thread_ = std::thread(&LockFreeAudioPipeline::processorLoop, this);

            return VoidResult::success();
        } catch (const std::exception& e) {
            is_running_.store(false);
            return VoidResult::error(Error::fromCurrentException("Failed to start pipeline"));
        }
    }

    void stop() {
        is_running_.store(false);

        if (decoder_thread_.joinable()) {
            decoder_thread_.join();
        }
        if (processor_thread_.joinable()) {
            processor_thread_.join();
        }
    }

    void audioCallback(float* output, size_t frames) noexcept {
        // Fast path - no allocations or locks
        AudioPacket packet;
        if (output_queue_.dequeue(packet)) {
            // Copy to output
            auto& buffer = packet.buffer;
            std::copy(buffer->data(),
                     buffer->data() + std::min(frames, buffer->getFrames()),
                     output);
        } else {
            // No data available, output silence
            std::fill_n(output, frames, 0.0f);
        }
    }

    Result<uint32_t> enqueueFile(const std::string& filename) {
        if (!is_running_.load(std::memory_order_acquire)) {
            return Result<uint32_t>::error(
                Error(ErrorCategory::InvalidState, "Pipeline not running"));
        }

        uint32_t sequence = sequence_counter_.fetch_add(1, std::memory_order_relaxed);

        AudioPacket packet{
            nullptr, // Buffer will be filled by decoder
            std::chrono::high_resolution_clock::now(),
            sequence
        };

        if (!input_queue_.enqueue(packet)) {
            return Result<uint32_t>::error(
                Error(ErrorCategory::BufferOverrun, "Input queue full"));
        }

        return Result<uint32_t>::success(sequence);
    }

private:
    void decoderLoop() {
        while (is_running_.load(std::memory_order_acquire)) {
            AudioPacket packet;
            if (input_queue_.dequeue(packet)) {
                // Decode file (omitted for brevity)
                auto bufferResult = decodeAudioFile(packet.sequence_number);
                if (bufferResult.isSuccess()) {
                    packet.buffer = std::move(bufferResult.getValue());
                    processing_queue_.enqueue(packet);
                }
            } else {
                // No work, yield briefly
                std::this_thread::yield();
            }
        }
    }

    void processorLoop() {
        while (is_running_.load(std::memory_order_acquire)) {
            AudioPacket packet;
            if (processing_queue_.dequeue(packet)) {
                // Apply DSP processing
                applyDSP(*packet.buffer);
                output_queue_.enqueue(packet);
            } else {
                std::this_thread::yield();
            }
        }
    }
};
```

### 7.4 Error Recovery and Resilience

```cpp
// src/audio/resilient_audio_output.cpp
#include "resilient_audio_output.h"
#include <chrono>

class ResilientAudioOutput {
private:
    std::unique_ptr<AudioOutput> primary_output_;
    std::vector<std::unique_ptr<AudioOutput>> fallback_outputs_;
    std::atomic<bool> is_fallback_active_{false};
    std::atomic<size_t> consecutive_failures_{0};
    std::chrono::steady_clock::time_point last_success_;

public:
    VoidResult initialize(const AudioConfig& config) {
        // Initialize primary output
        primary_output_ = createOutput(config.outputType);
        auto initResult = primary_output_->initialize(config);

        if (initResult.isSuccess()) {
            last_success_ = std::chrono::steady_clock::now();
            consecutive_failures_.store(0);
            return VoidResult::success();
        }

        // Primary failed, try fallbacks
        for (auto& fallback : fallback_outputs_) {
            auto fallbackResult = fallback->initialize(config);
            if (fallbackResult.isSuccess()) {
                logWarning("Primary output failed, using fallback: " +
                          fallback->getDeviceName());
                primary_output_ = std::move(fallback);
                is_fallback_active_.store(true);
                return VoidResult::success();
            }
        }

        return VoidResult::error(
            Error(ErrorCategory::AudioDeviceNotFound,
                  "No working audio outputs available",
                  ErrorSeverity::Critical)
                .addContext("primary_error", initResult.getError().getMessage()));
    }

    VoidResult writeAudio(const AudioBuffer& buffer) {
        auto result = primary_output_->write(buffer);

        if (result.isSuccess()) {
            // Success - reset failure count
            consecutive_failures_.store(0);
            last_success_ = std::chrono::steady_clock::now();

            // Try to switch back to primary if using fallback
            if (is_fallback_active_.load()) {
                trySwitchToPrimary();
            }

            return VoidResult::success();
        }

        // Write failed - handle error
        auto failures = consecutive_failures_.fetch_add(1) + 1;

        if (failures >= MAX_CONSECUTIVE_FAILURES) {
            // Too many failures, try to recover
            return handleCriticalFailure(result.getError());
        }

        // Try fallback device
        if (is_fallback_active_.load()) {
            // Already using fallback, just report error
            return VoidResult::error(result.getError());
        }

        return switchToFallback(result.getError());
    }

private:
    VoidResult switchToFallback(const Error& primaryError) {
        for (auto& fallback : fallback_outputs_) {
            auto fallbackResult = fallback->write(last_buffer_);
            if (fallbackResult.isSuccess()) {
                logWarning("Switched to fallback audio output due to: " +
                          primaryError.getMessage());
                primary_output_ = std::move(fallback);
                is_fallback_active_.store(true);
                consecutive_failures_.store(0);
                return VoidResult::success();
            }
        }

        // All fallbacks failed too
        return VoidResult::error(
            Error(ErrorCategory::AudioDeviceNotFound,
                  "Primary and all fallback outputs failed",
                  ErrorSeverity::Critical));
    }

    VoidResult handleCriticalFailure(const Error& error) {
        // Attempt complete reinitialization
        logError("Critical audio output failure, attempting recovery: " +
                error.getMessage());

        // Cleanup
        primary_output_.reset();
        is_fallback_active_.store(false);

        // Wait briefly before retry
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Reinitialize with default config
        AudioConfig defaultConfig = getDefaultAudioConfig();
        return initialize(defaultConfig);
    }
};
```

### 7.5 Thread-Safe Configuration Management

```cpp
// src/config/thread_safe_config.cpp
#include "thread_safe_config.h"
#include <shared_mutex>
#include <atomic>

class ThreadSafeConfigManager {
private:
    mutable std::shared_mutex config_mutex_;
    nlohmann::json config_;
    std::atomic<uint64_t> config_version_{0};
    std::string config_file_;

    // Cache for frequently accessed values
    struct CacheEntry {
        std::string value;
        uint64_t version;
        std::chrono::steady_clock::time_point last_update;
    };

    mutable std::mutex cache_mutex_;
    std::unordered_map<std::string, CacheEntry> value_cache_;

public:
    template<typename T>
    Result<T> getValue(const std::string& key, const T& defaultValue = T{}) const {
        // Check cache first
        {
            std::lock_guard cache_lock(cache_mutex_);
            auto it = value_cache_.find(key);
            if (it != value_cache_.end() &&
                it->second.version == config_version_.load()) {

                // Cached value is valid
                T value;
                try {
                    value = nlohmann::json::parse(it->second.value).get<T>();
                    return Result<T>::success(value);
                } catch (...) {
                    // Cache corrupted, continue to read from file
                }
            }
        }

        // Read from config with shared lock
        std::shared_lock config_lock(config_mutex_);
        auto it = config_.find(key);
        if (it != config_.end()) {
            try {
                T value = it->get<T>();

                // Update cache
                {
                    std::lock_guard cache_lock(cache_mutex_);
                    value_cache_[key] = {
                        it->dump(),
                        config_version_.load(),
                        std::chrono::steady_clock::now()
                    };
                }

                return Result<T>::success(value);
            } catch (const nlohmann::json::exception& e) {
                return Result<T>::error(
                    Error(ErrorCategory::ConfigParseError,
                          "Failed to parse config value for key: " + key)
                        .addContext("key", key)
                        .addContext("error", e.what()));
            }
        }

        return Result<T>::success(defaultValue);
    }

    template<typename T>
    VoidResult setValue(const std::string& key, const T& value) {
        // Write with exclusive lock
        std::unique_lock config_lock(config_mutex_);

        try {
            config_[key] = value;
            config_version_.fetch_add(1);

            // Invalidate cache
            {
                std::lock_guard cache_lock(cache_mutex_);
                value_cache_.erase(key);
            }

            // Trigger save asynchronously
            scheduleConfigSave();

            return VoidResult::success();
        } catch (const std::exception& e) {
            return VoidResult::error(
                Error(ErrorCategory::ConfigWriteFailed,
                      "Failed to set config value")
                    .addContext("key", key)
                    .addContext("error", e.what()));
        }
    }

    VoidResult reloadConfig() {
        std::unique_lock config_lock(config_mutex_);

        try {
            std::ifstream file(config_file_);
            if (!file) {
                return VoidResult::error(
                    Error(ErrorCategory::FileNotFound,
                          "Cannot open config file")
                        .addContext("file", config_file_));
            }

            nlohmann::json new_config;
            file >> new_config;

            // Validate config
            auto validationResult = validateConfig(new_config);
            if (validationResult.isError()) {
                return validationResult;
            }

            config_ = std::move(new_config);
            config_version_.fetch_add(1);

            // Clear cache
            {
                std::lock_guard cache_lock(cache_mutex_);
                value_cache_.clear();
            }

            return VoidResult::success();
        } catch (const std::exception& e) {
            return VoidResult::error(
                Error::fromCurrentException("Failed to reload config")
                    .addContext("file", config_file_));
        }
    }

private:
    VoidResult validateConfig(const nlohmann::json& config) {
        // Validate required fields
        if (!config.contains("audio")) {
            return VoidResult::error(
                Error(ErrorCategory::ConfigValidationFailed,
                      "Missing 'audio' section in config"));
        }

        const auto& audio = config["audio"];
        if (!audio.contains("sample_rate") || !audio.contains("buffer_size")) {
            return VoidResult::error(
                Error(ErrorCategory::ConfigValidationFailed,
                      "Missing required audio configuration"));
        }

        // Validate value ranges
        int sample_rate = audio["sample_rate"];
        if (sample_rate < 8000 || sample_rate > 192000) {
            return VoidResult::error(
                Error(ErrorCategory::ConfigValidationFailed,
                      "Invalid sample rate")
                    .addContext("sample_rate", sample_rate));
        }

        return VoidResult::success();
    }
};
```

These examples showcase the core implementation patterns used throughout the coreMusicPlayer project, demonstrating proper C++ practices, design patterns, thread safety, error handling, and platform-specific considerations. Each example includes comprehensive error handling, resource management using RAII, and clear documentation.