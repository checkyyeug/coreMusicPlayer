# coreMusicPlayer Architecture Design

## Table of Contents
1. [Overview](#1-overview)
2. [Unified Player Architecture](#2-unified-player-architecture)
3. [Component Architecture](#3-component-architecture)
4. [Threading Model](#4-threading-model)
5. [Memory Management](#5-memory-management)
6. [Plugin Architecture](#6-plugin-architecture)
7. [Feature Management System](#7-feature-management-system)
8. [Platform Abstraction](#8-platform-abstraction)
9. [Performance Optimization](#9-performance-optimization)
10. [Error Handling Strategy](#10-error-handling-strategy)

## 1. Overview

coreMusicPlayer uses a unified architecture that consolidates five separate player implementations into a single, mode-switchable system. The architecture is designed around the strategy pattern, allowing runtime switching between different playback modes while maintaining backward compatibility.

### Design Principles
- **Single Source of Truth**: One codebase, one executable
- **Mode Switching**: Runtime strategy pattern for different player modes
- **Resource Efficiency**: Lazy loading and resource pooling
- **Thread Safety**: Lock-free structures for real-time paths
- **Extensibility**: Plugin system for format and feature expansion

## 2. Unified Player Architecture

### 2.1 Strategy Pattern Implementation

```cpp
// Core strategy interface
class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;

    virtual VoidResult initialize(AudioEngine* engine) = 0;
    virtual void optimizePlayback() = 0;
    virtual Result<bool> loadFile(const std::string& filename) = 0;
    virtual PlaybackState getPlaybackState() = 0;
};

// Strategy implementations
class LegacyStrategy : public PlayerStrategy {
public:
    VoidResult initialize(AudioEngine* engine) override {
        // Enable only basic features
        engine->enableFeature(FeatureManager::BasicPlayback);
        engine->setBufferSize(2048);
        engine->setMaxPreBufferCount(2);
        return VoidResult::success();
    }
};

class RealtimeStrategy : public PlayerStrategy {
public:
    VoidResult initialize(AudioEngine* engine) override {
        // Configure for minimum latency
        engine->enableFeature(FeatureManager::RealtimeMode);
        engine->setBufferSize(64); // Very small buffer
        engine->setRealTimePriority(true);
        return VoidResult::success();
    }
};
```

### 2.2 Unified Player Manager

```cpp
class UnifiedMusicPlayer {
private:
    std::unique_ptr<PlayerStrategy> strategy_;
    std::unique_ptr<AudioEngine> audioEngine_;
    std::unique_ptr<DecoderManager> decoderManager_;
    std::unique_ptr<FeatureManager> featureManager_;
    PlayerMode currentMode_;

public:
    VoidResult setMode(PlayerMode mode) {
        // Validate mode switch
        if (!isModeSwitchValid(currentMode_, mode)) {
            return VoidResult::error(
                Error(ErrorCategory::InvalidState,
                      "Cannot switch from " + modeToString(currentMode_) +
                      " to " + modeToString(mode) + " while playing"));
        }

        // Create new strategy
        auto newStrategy = createStrategy(mode);
        auto result = newStrategy->initialize(audioEngine_.get());
        if (result.isError()) {
            return result;
        }

        // Switch strategies atomically
        strategy_ = std::move(newStrategy);
        currentMode_ = mode;

        return VoidResult::success();
    }
};
```

## 3. Component Architecture

### 3.1 Component Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                   UnifiedMusicPlayer                         │
├─────────────────────────────────────────────────────────────┤
│  PlayerMode  │  PlayerStrategy  │  FeatureManager            │
├─────────────────┼─────────────────┼───────────────────────────┤
│  AudioEngine    │  DecoderManager │  PluginManager              │
├─────────────────┼─────────────────┼───────────────────────────┤
│  AudioBufferPool│  ConfigManager  │  EventBus                  │
└─────────────────┴─────────────────┴───────────────────────────┘
```

### 3.2 Component Interactions

```cpp
class AudioEngine {
private:
    FeatureManager* featureManager_;
    std::unique_ptr<DecoderManager> decoderManager_;
    std::unique_ptr<AudioBufferPool> bufferPool_;

public:
    Result<std::unique_ptr<AudioBuffer>> processFile(const std::string& filename) {
        // Check if decoding is enabled
        if (!featureManager_->isEnabled(FeatureManager::BasicPlayback)) {
            return Result<std::unique_ptr<AudioBuffer>>::error(
                Error(ErrorCategory::InvalidState, "Decoding not enabled"));
        }

        // Get appropriate decoder
        auto decoder = decoderManager_->getDecoder(filename);
        if (!decoder) {
            return Result<std::unique_ptr<AudioBuffer>>::error(
                Error(ErrorCategory::UnsupportedFormat, "No decoder available"));
        }

        // Get buffer from pool
        auto buffer = bufferPool_->acquire();
        if (!buffer) {
            return Result<std::unique_ptr<AudioBuffer>>::error(
                Error(ErrorCategory::OutOfMemory, "Failed to acquire buffer"));
        }

        // Decode audio
        auto result = decoder->decode(filename, *buffer);
        if (result.isError()) {
            bufferPool_->release(std::move(buffer));
            return result;
        }

        return Result<std::unique_ptr<AudioBuffer>>::success(std::move(buffer));
    }
};
```

## 4. Threading Model

### 4.1 Thread Architecture

```
Main Thread (GUI & Event Dispatch)
    │
    ├─── Audio Thread (Real-time processing)
    │   ├─── Lock-free audio queue
    │   ├─── SIMD-optimized processing
    │   └─── No allocations in hot path
    │
    ├─── Decoder Thread Pool (File decoding)
    │   ├─── Thread-local decoders
    │   └─── Lazy loading management
    │
    ├─── I/O Thread (Asynchronous operations)
    │   └─── File operations without blocking
    │
    └─── Worker Thread Pool (Background tasks)
        ├─── Plugin processing
        ├─── Metadata extraction
        └─── Configuration I/O
```

### 4.2 Synchronization Strategy

```cpp
class ThreadSafeAudioEngine {
private:
    // Audio data protection - No mutex for real-time path
    alignas(64) std::atomic<float> volume_{1.0f};
    alignas(64) std::atomic<PlaybackState> state_{PlaybackState::Stopped};

    // Configuration protection - RW lock for read-heavy operations
    mutable std::shared_mutex configMutex_;
    AudioFormat currentFormat_;

    // Cross-thread communication - Lock-free queues
    moodycamel::ConcurrentQueue<AudioCommand> commandQueue_;
    moodycamel::ConcurrentQueue<AudioEvent> eventQueue_;

public:
    // Real-time audio callback (no locking!)
    void audioCallback(float* output, size_t frames) noexcept {
        if (state_.load(std::memory_order_acquire) == PlaybackState::Playing) {
            const float vol = volume_.load(std::memory_order_relaxed);
            processAudioWithSIMD(output, frames, vol);
        }
    }

    // Configuration update (write lock)
    VoidResult updateFormat(const AudioFormat& format) {
        std::unique_lock lock(configMutex_);
        currentFormat_ = format;
        return notifyFormatChange(format);
    }
};
```

### 4.3 Lock-Free Data Structures

```cpp
template<typename T, size_t Size>
class LockFreeQueue {
private:
    static constexpr size_t BUFFER_SIZE = Size + 1;
    alignas(64) std::atomic<size_t> writeIndex_{0};
    alignas(64) std::atomic<size_t> readIndex_{0};
    std::array<T, BUFFER_SIZE> buffer_;

public:
    bool enqueue(const T& item) noexcept {
        const size_t currentWrite = writeIndex_.load(std::memory_order_relaxed);
        const size_t nextWrite = (currentWrite + 1) % BUFFER_SIZE;

        if (nextWrite == readIndex_.load(std::memory_order_acquire)) {
            return false; // Queue full
        }

        buffer_[currentWrite] = item;
        writeIndex_.store(nextWrite, std::memory_order_release);
        return true;
    }
};
```

## 5. Memory Management

### 5.1 Audio Buffer Pool

```cpp
class AudioBufferPool {
private:
    struct BufferSlot {
        alignas(64) std::atomic<AudioBuffer*> buffer{nullptr};
        alignas(64) std::atomic<bool> inUse{false};
    };

    std::vector<BufferSlot> slots_;
    std::atomic<size_t> nextSlot_{0};

public:
    std::unique_ptr<PooledBufferHandle> acquire(size_t channels, size_t frames) {
        // Fast path - try lock-free acquisition
        for (size_t i = 0; i < slots_.size(); ++i) {
            size_t idx = nextSlot_.fetch_add(1) % slots_.size();

            bool expected = false;
            if (slots_[idx].inUse.compare_exchange_strong(
                    expected, true, std::memory_order_acquire)) {

                AudioBuffer* buffer = slots_[idx].buffer.load();
                if (buffer && buffer->matches(channels, frames)) {
                    return std::make_unique<PooledBufferHandle>(buffer, this);
                }
                slots_[idx].inUse.store(false, std::memory_order_release);
            }
        }

        // Slow path - allocate new buffer
        return allocateNewBuffer(channels, frames);
    }
};
```

### 5.2 RAII Resource Management

```cpp
class PooledBufferHandle {
private:
    AudioBuffer* buffer_;
    AudioBufferPool* pool_;

public:
    PooledBufferHandle(AudioBuffer* buffer, AudioBufferPool* pool)
        : buffer_(buffer), pool_(pool) {}

    ~PooledBufferHandle() {
        if (buffer_ && pool_) {
            pool_->release(buffer_);
        }
    }

    // Move semantics
    PooledBufferHandle(PooledBufferHandle&& other) noexcept
        : buffer_(other.buffer_), pool_(other.pool_) {
        other.buffer_ = nullptr;
        other.pool_ = nullptr;
    }
};
```

## 6. Plugin Architecture

### 6.1 Plugin Interface

```cpp
// Base plugin interface
class IPlugin {
public:
    virtual ~IPlugin() = default;

    virtual PluginInfo getInfo() const = 0;
    virtual VoidResult initialize() = 0;
    virtual void cleanup() = 0;
};

// Decoder plugin interface
class IDecoderPlugin : public IPlugin {
public:
    virtual bool canDecode(const std::string& filename) = 0;
    virtual Result<AudioFormat> getFormat(const std::string& filename) = 0;
    virtual Result<std::unique_ptr<AudioBuffer>> decode(const std::string& filename) = 0;
};
```

### 6.2 Plugin Manager

```cpp
class PluginManager {
private:
    std::unordered_map<std::string, std::unique_ptr<IPlugin>> plugins_;
    std::shared_mutex pluginsMutex_;

public:
    VoidResult loadPlugin(const std::string& path) {
        // Load dynamic library
        auto handle = dlopen(path.c_str(), RTLD_LAZY);
        if (!handle) {
            return VoidResult::error(
                Error(ErrorCategory::PluginLoadFailed,
                      "Failed to load plugin: " + path));
        }

        // Get create function
        auto createFunc = reinterpret_cast<IPlugin* (*)()>(dlsym(handle, "createPlugin"));
        if (!createFunc) {
            dlclose(handle);
            return VoidResult::error(
                Error(ErrorCategory::PluginLoadFailed,
                      "Plugin missing createPlugin function"));
        }

        // Create and initialize plugin
        auto plugin = std::unique_ptr<IPlugin>(createFunc());
        auto result = plugin->initialize();
        if (result.isError()) {
            return result;
        }

        // Store plugin
        std::unique_lock lock(pluginsMutex_);
        plugins_[plugin->getInfo().id] = std::move(plugin);

        return VoidResult::success();
    }
};
```

## 7. Feature Management System

### 7.1 Feature Definition

```cpp
class FeatureManager {
public:
    enum class Feature : uint32_t {
        // Audio processing features
        BasicPlayback = 1 << 0,
        VolumeControl = 1 << 1,
        SampleRateConversion = 1 << 2,
        SIMDOptimization = 1 << 3,
        AdvancedDSP = 1 << 4,

        // Format support
        WAVSupport = 1 << 10,
        MP3Support = 1 << 11,
        FLACSupport = 1 << 12,
        OGGSupport = 1 << 13,
        MultiFormat = 1 << 14,

        // System features
        PluginSupport = 1 << 20,
        RealtimeMode = 1 << 21,
        LowLatency = 1 << 22,
        ErrorRecovery = 1 << 23,
        AudioVisualization = 1 << 24,

        // Advanced features
        AdvancedBuffering = 1 << 30,
        HotPluginReload = 1 << 31
    };

private:
    std::atomic<uint64_t> enabledFeatures_{0};
    std::unordered_map<Feature, FeatureInfo> featureInfo_;

    struct FeatureInfo {
        std::string name;
        std::string description;
        std::vector<Feature> dependencies;
        std::vector<Feature> conflicts;
        std::function<VoidResult()> initializer;
        std::function<void()> cleanup;
    };
};
```

### 7.2 Feature Dependencies

```cpp
VoidResult FeatureManager::enableFeature(Feature feature) {
    // Check conflicts
    auto conflicts = getConflicts(feature);
    for (auto conflict : conflicts) {
        if (isEnabled(conflict)) {
            return VoidResult::error(
                Error(ErrorCategory::InvalidState,
                      "Feature conflicts with " + getFeatureInfo(conflict).name));
        }
    }

    // Enable dependencies recursively
    auto dependencies = getDependencies(feature);
    for (auto dep : dependencies) {
        if (!isEnabled(dep)) {
            auto result = enableFeature(dep);
            if (result.isError()) {
                return result;
            }
        }
    }

    // Initialize feature if not already enabled
    if (!isEnabled(feature)) {
        auto& info = getFeatureInfo(feature);
        if (info.initializer) {
            auto result = info.initializer();
            if (result.isError()) {
                return result;
            }
        }

        enabledFeatures_.fetch_or(static_cast<uint64_t>(feature));
    }

    return VoidResult::success();
}
```

### 7.3 Feature Presets

```cpp
class FeaturePresets {
public:
    static const std::unordered_map<std::string, uint64_t> PRESETS = {
        {"minimal", static_cast<uint64_t>(
            FeatureManager::BasicPlayback | FeatureManager::WAVSupport)},

        {"standard", static_cast<uint64_t>(
            FeatureManager::BasicPlayback |
            FeatureManager::VolumeControl |
            FeatureManager::SampleRateConversion |
            FeatureManager::MultiFormat)},

        {"professional", static_cast<uint64_t>(
            FeatureManager::BasicPlayback |
            FeatureManager::VolumeControl |
            FeatureManager::SampleRateConversion |
            FeatureManager::SIMDOptimization |
            FeatureManager::AdvancedDSP |
            FeatureManager::MultiFormat |
            FeatureManager::PluginSupport)},

        {"realtime", static_cast<uint64_t>(
            FeatureManager::BasicPlayback |
            FeatureManager::VolumeControl |
            FeatureManager::RealtimeMode |
            FeatureManager::LowLatency |
            FeatureManager::SIMDOptimization)}
    };
};
```

## 8. Platform Abstraction

### 8.1 Audio Output Interface

```cpp
class IAudioOutput {
public:
    virtual ~IAudioOutput() = default;

    virtual VoidResult initialize(const AudioFormat& format) = 0;
    virtual VoidResult shutdown() = 0;
    virtual VoidResult write(const AudioBuffer& buffer) = 0;

    virtual Result<std::vector<DeviceInfo>> enumerateDevices() = 0;
    virtual VoidResult setDevice(const std::string& deviceId) = 0;

    virtual double getLatency() const = 0;
    virtual AudioFormat getCurrentFormat() const = 0;
};
```

### 8.2 Platform-Specific Implementations

```cpp
// Windows WASAPI
class WASAPIOutput : public IAudioOutput {
private:
    IAudioClient* audioClient_;
    IAudioRenderClient* renderClient_;
    HANDLE audioEvent_;

public:
    VoidResult initialize(const AudioFormat& format) override {
        // Initialize WASAPI
        HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr)) {
            return VoidResult::error(
                Error(ErrorCategory::AudioInitialization,
                      "Failed to initialize COM"));
        }

        // Create device enumerator
        IMMDeviceEnumerator* enumerator = nullptr;
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                            CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
                            reinterpret_cast<void**>(&enumerator));

        if (FAILED(hr)) {
            return VoidResult::error(
                Error(ErrorCategory::AudioInitialization,
                      "Failed to create device enumerator"));
        }

        // Get default device
        IMMDevice* device = nullptr;
        hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);

        if (FAILED(hr)) {
            enumerator->Release();
            return VoidResult::error(
                Error(ErrorCategory::AudioDeviceNotFound,
                      "No default audio device"));
        }

        // Initialize audio client
        // ... (implementation details)

        return VoidResult::success();
    }
};

// Linux ALSA
class ALSAOutput : public IAudioOutput {
private:
    snd_pcm_t* pcmHandle_;
    snd_pcm_hw_params_t* hwParams_;

public:
    VoidResult initialize(const AudioFormat& format) override {
        // Open PCM device
        int err = snd_pcm_open(&pcmHandle_, "default",
                              SND_PCM_STREAM_PLAYBACK, 0);
        if (err < 0) {
            return VoidResult::error(
                Error(ErrorCategory::AudioInitialization,
                      "Cannot open PCM device"));
        }

        // Set hardware parameters
        err = snd_pcm_hw_params_alloca(&hwParams_);
        if (err < 0) {
            snd_pcm_close(pcmHandle_);
            return VoidResult::error(
                Error(ErrorCategory::AudioInitialization,
                      "Cannot allocate hardware parameter structure"));
        }

        // Configure parameters
        // ... (implementation details)

        return VoidResult::success();
    }
};
```

## 9. Performance Optimization

### 9.1 SIMD Optimizations

```cpp
namespace simd {

// Runtime SIMD detection
class SIMD {
public:
    enum class Level {
        None,
        SSE2,
        AVX2,
        AVX512
    };

    static Level detectMaxLevel() {
        if (hasAVX512()) return Level::AVX512;
        if (hasAVX2()) return Level::AVX2;
        if (hasSSE2()) return Level::SSE2;
        return Level::None;
    }

    static bool hasSSE2();
    static bool hasAVX2();
    static bool hasAVX512();
};

// Optimized volume adjustment
void adjustVolume(float* data, size_t samples, float volume) {
    switch (SIMD::detectMaxLevel()) {
        case SIMD::Level::AVX2:
            adjustVolumeAVX2(data, samples, volume);
            break;
        case SIMD::Level::SSE2:
            adjustVolumeSSE2(data, samples, volume);
            break;
        default:
            adjustVolumeScalar(data, samples, volume);
            break;
    }
}

// AVX2 implementation
void adjustVolumeAVX2(float* data, size_t samples, float volume) {
    const __m256 volumeVec = _mm256_set1_ps(volume);
    const size_t simdSamples = samples & ~7; // Process 8 at a time

    for (size_t i = 0; i < simdSamples; i += 8) {
        __m256 samplesVec = _mm256_load_ps(data + i);
        __m256 result = _mm256_mul_ps(samplesVec, volumeVec);
        _mm256_store_ps(data + i, result);
    }

    // Process remaining samples
    for (size_t i = simdSamples; i < samples; ++i) {
        data[i] *= volume;
    }
}

} // namespace simd
```

### 9.2 Cache Optimization

```cpp
class CacheOptimizedAudioBuffer {
private:
    // Separate buffers for each cache line
    alignas(64) std::vector<float> leftChannel_;
    alignas(64) std::vector<float> rightChannel_;

    // Pad to prevent false sharing
    char padding_[64 - sizeof(std::vector<float>) * 2 % 64];

public:
    void process(float* output, size_t frames) {
        // Process in cache-friendly chunks
        constexpr size_t CHUNK_SIZE = 1024;

        for (size_t chunk = 0; chunk < frames; chunk += CHUNK_SIZE) {
            size_t chunkFrames = std::min(CHUNK_SIZE, frames - chunk);

            // Process chunk - fits in L1 cache
            for (size_t i = 0; i < chunkFrames; ++i) {
                output[(chunk + i) * 2] = leftChannel_[chunk + i];
                output[(chunk + i) * 2 + 1] = rightChannel_[chunk + i];
            }
        }
    }
};
```

## 10. Error Handling Strategy

### 10.1 Result-Based Error Handling

```cpp
// All API functions return Result<T>
template<typename T>
class Result {
private:
    std::variant<T, Error> value_;

public:
    static Result<T> success(T value) {
        return Result<T>(std::move(value));
    }

    static Result<T> error(Error error) {
        return Result<T>(std::move(error));
    }

    bool isSuccess() const noexcept {
        return std::holds_alternative<T>(value_);
    }

    bool isError() const noexcept {
        return std::holds_alternative<Error>(value_);
    }

    const T& getValue() const {
        if (isError()) {
            throw std::runtime_error("Attempted to get value from error result");
        }
        return std::get<T>(value_);
    }

    const Error& getError() const {
        if (isSuccess()) {
            throw std::runtime_error("Attempted to get error from success result");
        }
        return std::get<Error>(value_);
    }
};
```

### 10.2 Error Recovery

```cpp
class AudioOutput {
private:
    std::unique_ptr<IAudioOutput> primaryOutput_;
    std::vector<std::unique_ptr<IAudioOutput>> fallbackOutputs_;

public:
    VoidResult writeAudio(const AudioBuffer& buffer) {
        auto result = primaryOutput_->write(buffer);

        if (result.isSuccess()) {
            return VoidResult::success();
        }

        // Try fallback devices
        for (auto& fallback : fallbackOutputs_) {
            result = fallback->write(buffer);
            if (result.isSuccess()) {
                // Switch to fallback
                primaryOutput_ = std::move(fallback);
                logWarning("Switched to fallback audio output");
                return VoidResult::success();
            }
        }

        // All outputs failed
        return VoidResult::error(
            Error(ErrorCategory::AudioDeviceNotFound,
                  "All audio outputs failed"));
    }
};
```

### 10.3 Error Context

```cpp
class Error {
private:
    ErrorCategory category_;
    std::string message_;
    ErrorSeverity severity_;
    std::unordered_map<std::string, std::string> context_;

public:
    template<typename T>
    Error& addContext(const std::string& key, const T& value) {
        context_[key] = std::to_string(value);
        return *this;
    }

    std::string toString() const {
        std::string result;
        result += "[" + severityToString(severity_) + "] ";
        result += categoryToString(category_) + ": ";
        result += message_;

        if (!context_.empty()) {
            result += "\nContext:\n";
            for (const auto& [k, v] : context_) {
                result += "  " + k + " = " + v + "\n";
            }
        }

        return result;
    }
};
```

## Summary

The coreMusicPlayer architecture provides:

1. **Unified Design**: One codebase supporting multiple playback modes
2. **Performance**: Optimized for both real-time and general use
3. **Extensibility**: Plugin system for formats and features
4. **Reliability**: Comprehensive error handling and recovery
5. **Maintainability**: Clear separation of concerns and modular design

This architecture enables coreMusicPlayer to scale from simple WAV playback to professional audio processing while maintaining high performance and reliability.