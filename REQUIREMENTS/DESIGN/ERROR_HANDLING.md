# coreMusicPlayer Error Handling Strategy

## Overview

coreMusicPlayer uses a unified error handling strategy based on `Result<T>` types instead of exceptions. This approach provides explicit error handling, better performance, and clearer code flow.

## Design Principles

1. **Explicit Error Handling**: All functions return `Result<T>` - no exceptions
2. **Rich Error Context**: Errors include detailed context for debugging
3. **Graceful Degradation**: Automatic recovery from common errors
4. **Performance Awareness**: Zero overhead in success path

## Core Error Types

### Error Categories

```cpp
enum class ErrorCategory : int {
    // Audio errors (1000-1999)
    AudioInitialization = 1000,
    AudioDeviceNotFound = 1001,
    AudioFormatUnsupported = 1002,
    AudioBufferUnderrun = 1003,
    AudioBufferOverrun = 1004,

    // File errors (2000-2999)
    FileNotFound = 2000,
    FileAccessDenied = 2001,
    FileCorrupted = 2002,
    UnsupportedFormat = 2003,

    // Plugin errors (3000-3999)
    PluginLoadFailed = 3000,
    PluginInitializationFailed = 3001,
    PluginRuntimeError = 3002,
    PluginSecurityViolation = 3003,

    // Configuration errors (4000-4999)
    ConfigParseError = 4000,
    ConfigValidationFailed = 4001,
    ConfigWriteFailed = 4002,

    // Memory errors (5000-5999)
    OutOfMemory = 5000,
    MemoryLeak = 5001,
    InvalidMemoryAccess = 5002,

    // Thread errors (6000-6999)
    ThreadCreationFailed = 6000,
    ThreadDeadlock = 6001,
    ThreadTimeout = 6002,

    // General errors (9000-9999)
    Unknown = 9000,
    NotImplemented = 9001,
    InvalidArgument = 9002,
    OperationFailed = 9003
};
```

### Error Severity Levels

```cpp
enum class ErrorSeverity {
    Debug,      // Debug information only
    Info,       // Informational message
    Warning,    // Recoverable error
    Error,      // Standard error
    Critical    // Unrecoverable error
};
```

## Result Type Implementation

### Basic Result Structure

```cpp
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

    T takeValue() {
        if (isError()) {
            throw std::runtime_error("Attempted to take value from error result");
        }
        return std::move(std::get<T>(value_));
    }

    const Error& getError() const {
        if (isSuccess()) {
            throw std::runtime_error("Attempted to get error from success result");
        }
        return std::get<Error>(value_);
    }
};
```

### Monadic Operations

```cpp
// Map operation - transform success value
template<typename F>
auto map(F&& func) -> Result<decltype(func(getValue()))> {
    if (isError()) {
        return Result<decltype(func(getValue()))>::error(getError());
    }

    try {
        return Result<decltype(func(getValue()))>::success(func(getValue()));
    } catch (...) {
        return Result<decltype(func(getValue()))>::error(
            Error::fromCurrentException());
    }
}

// FlatMap - chain operations that return Result
template<typename F>
auto flatMap(F&& func) -> decltype(func(getValue())) {
    if (isError()) {
        return decltype(func(getValue()))::error(getError());
    }
    return func(getValue());
}
```

## Error Recovery Strategies

### Automatic Recovery

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

        // Try recovery based on error type
        if (auto recoveryResult = handleWriteError(result.getError());
            recoveryResult.isSuccess()) {
            return recoveryResult;
        }

        // Try fallback devices
        return tryFallbackOutputs(buffer);
    }

private:
    VoidResult handleWriteError(const Error& error) {
        switch (error.getCategory()) {
            case ErrorCategory::AudioBufferUnderrun:
                return handleBufferUnderrun();

            case ErrorCategory::AudioDeviceNotFound:
                return reconnectDevice();

            default:
                return VoidResult::error(error);
        }
    }
};
```

### Graceful Degradation

```cpp
class SampleRateConverter {
public:
    Result<AudioBuffer> convert(const AudioBuffer& input,
                               int targetRate,
                               Quality desiredQuality) {
        // Try desired quality first
        auto result = convertWithQuality(input, targetRate, desiredQuality);
        if (result.isSuccess()) {
            return result;
        }

        // Fall back progressively
        if (desiredQuality > Quality::Good) {
            logWarning("Falling back to Good quality");
            return convertWithQuality(input, targetRate, Quality::Good);
        }

        if (desiredQuality > Quality::Fast) {
            logWarning("Falling back to Fast quality");
            return convertWithQuality(input, targetRate, Quality::Fast);
        }

        return Result<AudioBuffer>::error(
            Error(ErrorCategory::UnsupportedFormat,
                  "All conversion quality levels failed"));
    }
};
```

## Error Context and Debugging

### Rich Error Context

```cpp
class Error {
private:
    std::unordered_map<std::string, std::string> context_;

public:
    template<typename T>
    Error& addContext(const std::string& key, const T& value) {
        context_[key] = std::to_string(value);
        return *this;
    }

    std::string getContext(const std::string& key) const {
        auto it = context_.find(key);
        return it != context_.end() ? it->second : std::string{};
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

### Usage Example

```cpp
AudioDecoder* DecoderManager::getDecoder(const std::string& format) {
    // Try to get existing decoder
    auto it = decoders_.find(format);
    if (it != decoders_.end()) {
        return it->second.get();
    }

    // Need to load decoder
    auto factory = decoderFactories_.find(format);
    if (factory == decoderFactories_.end()) {
        lastError_ = Error(ErrorCategory::UnsupportedFormat,
                          "No decoder registered for format")
                        .addContext("format", format)
                        .addContext("available_formats",
                                   std::to_string(decoderFactories_.size()));
        return nullptr;
    }

    // Create and initialize decoder
    auto decoder = factory->second();
    if (!decoder->initialize()) {
        lastError_ = Error(ErrorCategory::PluginInitializationFailed,
                          "Failed to initialize decoder")
                        .addContext("format", format);
        return nullptr;
    }

    decoders_[format] = std::move(decoder);
    return decoders_[format].get();
}
```

## Thread-Safe Error Handling

### Atomic Error Storage

```cpp
class ThreadSafeAudioEngine {
private:
    std::atomic<Error> lastError_;
    std::atomic<bool> hasError_{false};

public:
    void setLastError(Error error) {
        lastError_.store(std::move(error));
        hasError_.store(true);
    }

    std::optional<Error> getLastError() const {
        if (hasError_.load()) {
            return lastError_.load();
        }
        return std::nullopt;
    }

    void clearError() {
        hasError_.store(false);
    }
};
```

### Error Reporting in Real-time Path

```cpp
void audioCallback(float* output, size_t frames) noexcept {
    // Fast path - no error checking
    if (processingState.load() == ProcessingState::Error) {
        // Clear buffer to silence
        std::fill_n(output, frames * channels, 0.0f);
        return;
    }

    // Try to get audio data
    if (!audioQueue_.dequeue(buffer)) {
        // Report error without blocking
        setAtomicError(Error(ErrorCategory::AudioBufferUnderrun,
                           "Audio buffer underrun in callback"));

        // Clear output
        std::fill_n(output, frames * channels, 0.0f);
        return;
    }

    // Process audio normally
    processAudio(buffer, output, frames);
}
```

## Error Handling Guidelines

### Do's

1. **Always check Result types**
   ```cpp
   auto result = loadFile("music.mp3");
   if (result.isError()) {
       handleError(result.getError());
       return;
   }
   ```

2. **Provide rich context**
   ```cpp
   return Result<void>::error(
       Error(ErrorCategory::FileNotFound, "Cannot open audio file")
           .addContext("filename", filename)
           .addContext("working_directory", getCurrentDirectory())
           .addContext("errno", errno));
   ```

3. **Use monadic operations for chaining**
   ```cpp
   return loadFile(filename)
       .flatMap([this](auto file) { return parseHeader(file); })
       .flatMap([this](auto header) { return allocateBuffer(header); });
   ```

### Don'ts

1. **Don't throw exceptions in error path**
   ```cpp
   // Bad
   if (error) {
       throw std::runtime_error(error.getMessage());
   }

   // Good
   if (error) {
       return Result<T>::error(error);
   }
   ```

2. **Don't lose error context**
   ```cpp
   // Bad
   if (decoder->initialize()) {
       return success;
   } else {
       return error("Failed to initialize");
   }

   // Good
   if (decoder->initialize()) {
       return success;
   } else {
       return error("Failed to initialize decoder: " + decoder->getLastError().getMessage());
   }
   ```

3. **Don't ignore errors**
   ```cpp
   // Bad
   loadFile(filename);  // Ignoring Result

   // Good
   auto result = loadFile(filename);
   if (result.isError()) {
       logError(result.getError());
       return;
   }
   ```

## Integration with Existing Code

### Migration from Exceptions

```cpp
// Before (exceptions)
void AudioEngine::initialize() {
    if (!initDevice()) {
        throw AudioException("Failed to initialize device");
    }

    if (!setupBuffers()) {
        throw AudioException("Failed to setup buffers");
    }
}

// After (Result)
VoidResult AudioEngine::initialize() {
    auto deviceResult = initDevice();
    if (deviceResult.isError()) {
        return deviceResult;
    }

    auto bufferResult = setupBuffers();
    if (bufferResult.isError()) {
        cleanupDevice();
        return bufferResult;
    }

    return VoidResult::success();
}
```

### Backward Compatibility

```cpp
// Legacy API that still throws for compatibility
class LegacyAudioEngine {
public:
    void initialize() {
        auto result = modernEngine_.initialize();
        if (result.isError()) {
            throw AudioException(result.getError().getMessage());
        }
    }
};
```

## Testing Error Handling

### Unit Tests

```cpp
TEST(AudioDecoderTest, HandlesCorruptedFile) {
    auto decoder = std::make_unique<WAVDecoder>();

    auto result = decoder->decode("corrupted.wav");

    EXPECT_TRUE(result.isError());
    EXPECT_EQ(result.getError().getCategory(),
              ErrorCategory::FileCorrupted);
    EXPECT_TRUE(result.getError().isRecoverable());
}

TEST(ErrorTest, PreservesContext) {
    auto error = Error(ErrorCategory::FileNotFound, "File not found")
        .addContext("filename", "test.wav")
        .addContext("path", "/music/");

    EXPECT_EQ(error.getContext("filename"), "test.wav");
    EXPECT_EQ(error.getContext("path"), "/music/");

    auto str = error.toString();
    EXPECT_NE(str.find("filename: test.wav"), std::string::npos);
}
```

## Summary

The coreMusicPlayer error handling strategy provides:

1. **Explicit error handling** through `Result<T>` types
2. **Rich error context** for effective debugging
3. **Automatic recovery** from common errors
4. **Thread-safe error reporting**
5. **Zero overhead** in success paths
6. **Clear migration path** from exception-based code

This approach results in more reliable, maintainable, and performant code while providing excellent debugging capabilities.