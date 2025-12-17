# coreMusicPlayer Code Standards

## 1. Overview

This document defines the coding standards and conventions for the coreMusicPlayer project. Adherence to these standards ensures code readability, maintainability, and consistency across the codebase.

## 2. General Guidelines

### 2.1 Philosophy
- **Clarity over cleverness**: Code should be easy to understand
- **Simplicity over complexity**: Choose simple solutions
- **Explicit over implicit**: Make intentions clear
- **Consistency over personal preference**: Follow established patterns

### 2.2 Principles
1. **Readability First**: Write code that others can understand
2. **Maintainability**: Code should be easy to modify and extend
3. **Performance**: Optimize when necessary, measure before optimizing
4. **Portability**: Write code that works across platforms
5. **Safety**: Consider edge cases and error conditions

## 3. C++ Standards

### 3.1 Language Version
- **C++17**: Use C++17 features where appropriate
- **Compiler Compatibility**: MSVC 2019+, GCC 9+, Clang 10+

### 3.2 Naming Conventions

#### 3.2.1 Classes and Types
```cpp
class AudioBuffer {
public:
    // ...
};

struct AudioFormat {
    int sample_rate;
    int channels;
    AudioFormatType format;
};

enum class PlaybackState {
    Stopped,
    Playing,
    Paused
};

// Type aliases
using AudioFrame = std::vector<float>;
```

#### 3.2.2 Functions and Methods
```cpp
// Regular functions
AudioBuffer* create_buffer(int channels, int frames);

// Class methods
void AudioBuffer::resize(size_t frames);

// Const member functions
size_t AudioBuffer::size() const;

// Static methods
static AudioFormat detect_format(const std::string& filename);
```

#### 3.2.3 Variables
```cpp
// Member variables
class AudioBuffer {
private:
    size_t size_;
    std::unique_ptr<float[]> data_;
    int channels_;
};

// Local variables
int sample_rate = 44100;
auto buffer = std::make_unique<float[]>(frames);

// Constants
const int MAX_CHANNELS = 8;
constexpr double PI = 3.14159265359;
```

### 3.3 Code Style

#### 3.3.1 Indentation
- **Spaces**: Use 4 spaces, no tabs
- **Alignment**: Align braces vertically

#### 3.3.2 Braces
```cpp
// 1TBS (One True Brace Style)
if (condition) {
    do_something();
} else {
    do_something_else();
}

// Functions
void function() {
    do_something();
}
```

#### 3.3.3 Spacing
```cpp
// Operators with spaces
int result = a + b * c;

// Control structures
if (condition) {
    // ...
}

// Function parameters
void function(int param1, int param2);

// Templates
template<typename T>
T function() {
    // ...
}
```

### 3.4 Comments

#### 3.4.1 File Header
```cpp
/**
 * @file audio_buffer.h
 * @brief Audio buffer management for coreMusicPlayer
 *
 * This file contains the AudioBuffer class which provides
 * a container for audio data with support for multiple channels
 * and various audio formats.
 *
 * @author coreMusicPlayer Team
 * @date 2024
 */
```

#### 3.4.2 Class Documentation
```cpp
/**
 * @brief Thread-safe audio buffer container
 *
 * The AudioBuffer class manages audio data with support for multiple
 * channels and real-time resizing. It provides SIMD-aligned memory
 * for optimal performance with vectorized operations.
 *
 * @example
 * AudioBuffer buffer(2, 1024); // 2 channels, 1024 frames
 * buffer.resize(2048); // Resize to 2048 frames
 */
class AudioBuffer {
    // ...
};
```

#### 3.4.3 Method Documentation
```cpp
/**
 * @brief Resize the buffer to hold a specific number of frames
 *
 * @param frames Number of frames to allocate
 * @return True if resize was successful
 *
 * This method may trigger reallocation if the new size requires
 * more memory than currently allocated. The existing data is
 * preserved up to the minimum of old and new sizes.
 */
bool resize(size_t frames);
```

#### 3.4.4 Implementation Comments
```cpp
// Clear the buffer
std::fill(data_.get(), data_.get() + size(), 0.0f);

// Handle special case for zero frames
if (frames == 0) {
    return false;
}
```

## 4. Design Patterns

### 4.1 Singleton
```cpp
class ConfigManager {
public:
    static ConfigManager& getInstance() {
        static ConfigManager instance;
        return instance;
    }

    // Delete copy constructor and assignment operator
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

private:
    ConfigManager() = default;
};
```

### 4.2 Factory Pattern
```cpp
class AudioDecoderFactory {
public:
    static std::unique_ptr<AudioDecoder> create(const std::string& format) {
        if (format == "wav") {
            return std::make_unique<WAVDecoder>();
        } else if (format == "mp3") {
            return std::make_unique<MP3Decoder>();
        }
        return nullptr;
    }
};
```

### 4.3 RAII
```cpp
class AudioDevice {
public:
    AudioDevice() {
        initialize();
    }

    ~AudioDevice() {
        cleanup();
    }

private:
    void initialize() {
        // Acquire resource
    }

    void cleanup() {
        // Release resource
    }
};
```

### 4.4 Smart Pointers
```cpp
// Use unique_ptr for exclusive ownership
std::unique_ptr<AudioBuffer> buffer = std::make_unique<AudioBuffer>();

// Use shared_ptr for shared ownership
std::shared_ptr<PluginManager> plugin_manager =
    std::make_shared<PluginManager>();

// Use weak_ptr to avoid cycles
std::weak_ptr<Plugin> plugin_ref;
```

## 5. Error Handling

### 5.1 Exceptions
```cpp
class AudioException : public std::exception {
public:
    AudioException(ErrorCode code, const std::string& message)
        : error_code_(code), message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

    ErrorCode getErrorCode() const { return error_code_; }

private:
    ErrorCode error_code_;
    std::string message_;
};
```

### 5.2 Error Codes
```cpp
enum class ErrorCode {
    Success = 0,
    FileNotFound = 1,
    UnsupportedFormat = 2,
    DeviceError = 3,
    OutOfMemory = 4,
    InvalidArgument = 5
};
```

### 5.3 Return Value Pattern
```cpp
// For functions that can fail
bool decode_file(const std::string& filename, AudioBuffer& buffer) {
    try {
        // Implementation
        return true;
    } catch (const AudioException& e) {
        log_error("Failed to decode file: " + std::string(e.what()));
        return false;
    }
}
```

## 6. Memory Management

### 6.1 Raw Pointers
- **Avoid**: Prefer smart pointers
- **When to use**: Legacy API compatibility, performance-critical code
- **Documentation**: Document ownership and lifetime

```cpp
// Bad (raw pointer without clear owner)
AudioBuffer* create_buffer() {
    return new AudioBuffer(); // Who deletes this?
}

// Good (unique_ptr with clear ownership)
std::unique_ptr<AudioBuffer> create_buffer() {
    return std::make_unique<AudioBuffer>();
}
```

### 6.2 Memory Alignment
```cpp
// Aligned allocation for SIMD
class AudioBuffer {
private:
    static constexpr size_t ALIGNMENT = 32; // AVX requires 32-byte alignment

    // Aligned allocation
    void* allocate_aligned(size_t size) {
        return aligned_alloc(ALIGNMENT, size);
    }
};
```

### 6.3 Resource Pools
```cpp
template<typename T>
class ResourcePool {
public:
    std::unique_ptr<T> acquire() {
        if (pool_.empty()) {
            return std::make_unique<T>();
        }
        auto resource = std::move(pool_.back());
        pool_.pop_back();
        return resource;
    }

    void release(std::unique_ptr<T> resource) {
        pool_.push_back(std::move(resource));
    }

private:
    std::vector<std::unique_ptr<T>> pool_;
};
```

## 7. Concurrency

### 7.1 Threads
```cpp
// Use std::thread for thread management
class AudioProcessor {
public:
    void start() {
        worker_thread_ = std::thread(&AudioProcessor::process_loop, this);
    }

    void stop() {
        running_ = false;
        worker_thread_.join();
    }

private:
    void process_loop() {
        while (running_) {
            // Process audio
        }
    }

    std::thread worker_thread_;
    std::atomic<bool> running_{true};
};
```

### 7.2 Mutexes
```cpp
class AudioBuffer {
public:
    void write(const float* data, size_t frames) {
        std::lock_guard<std::mutex> lock(mutex_);
        // Write data
    }

private:
    std::mutex mutex_;
};
```

### 7.3 Atomic Operations
```cpp
class PlaybackController {
public:
    void set_volume(double volume) {
        volume_.store(volume);
    }

    double get_volume() const {
        return volume_.load();
    }

private:
    std::atomic<double> volume_{1.0};
};
```

### 7.4 Lock-Free Data Structures
```cpp
class LockFreeQueue {
public:
    bool push(const AudioFrame& frame) {
        // Lock-free push implementation
    }

    bool pop(AudioFrame& frame) {
        // Lock-free pop implementation
    }

private:
    // Implementation details...
};
```

## 8. Platform-Specific Code

### 8.1 Preprocessor Directives
```cpp
#ifdef _WIN32
    // Windows-specific code
    #include <windows.h>
#elif defined(__linux__)
    // Linux-specific code
    #include <alsa/asoundlib.h>
#endif
```

### 8.2 Platform Abstraction
```cpp
// Abstract base class
class AudioOutput {
public:
    virtual ~AudioOutput() = default;
    virtual bool initialize(const AudioFormat& format) = 0;
    virtual void cleanup() = 0;
    virtual void write(const AudioBuffer& buffer) = 0;
};

// Platform implementations
class WASAPIOutput : public AudioOutput { /* ... */ };
class ALSAOutput : public AudioOutput { /* ... */ };
```

## 9. Performance Guidelines

### 9.1 Optimizations
```cpp
// Pre-allocate
AudioBuffer buffer(2, 4096);

// Move semantics
std::vector<AudioBuffer> buffers;
buffers.push_back(std::move(buffer));

// Reserve capacity
std::vector<float> samples;
samples.reserve(1024);
```

### 9.2 SIMD Optimizations
```cpp
#ifdef __SSE2__
void process_audio_sse2(float* data, size_t frames) {
    __m128 volume = _mm_set1_ps(volume);

    for (size_t i = 0; i < frames; i += 4) {
        __m128 samples = _mm_load_ps(data + i);
        samples = _mm_mul_ps(samples, volume);
        _mm_store_ps(data + i, samples);
    }
}
#endif
```

### 9.3 Avoid Unnecessary Copies
```cpp
// Bad: copies buffer
std::vector<float> process(std::vector<float> input) {
    std::vector<float> output;
    // Process input
    return output; // Copy returned
}

// Good: references
void process(const std::vector<float>& input, std::vector<float>& output) {
    output.resize(input.size());
    // Process input
}
```

## 10. Testing Standards

### 10.1 Unit Test Structure
```cpp
TEST(AudioBufferTest, BasicOperations) {
    // Arrange
    AudioBuffer buffer(2, 1024);

    // Act
    buffer.resize(2048);

    // Assert
    EXPECT_EQ(buffer.getFrames(), 2048);
    EXPECT_EQ(buffer.getChannels(), 2);
}
```

### 10.2 Test Naming
- **Descriptive**: Test what the code does
- **Inverted**: Test scenarios
- **TestDouble**: Setup-Act-Assert pattern

### 10.3 Fixtures
```cpp
class AudioBufferTest : public ::testing::Test {
protected:
    void SetUp() override {
        buffer_ = std::make_unique<AudioBuffer>(2, 1024);
    }

    void TearDown() override {
        buffer_.reset();
    }

protected:
    std::unique_ptr<AudioBuffer> buffer_;
};
```

## 11. Documentation Standards

### 11.1 Header Documentation
- Every header has a file comment block
- Every public class is documented
- Every public method is documented
- Use Doxygen syntax

### 12. Version Control

### 12.1 Commit Messages
```
type(scope): brief description

More detailed explanation if needed.

Issue #123

Co-authored-by: Developer Name <email@example.com>
```

### 12.2 Branches
- `main`: Stable release branch
- `develop`: Development branch
- `feature/*`: Feature branches
- `hotfix/*`: Quick fixes

### 12.3 Tags
- Format: `v1.0.0` (semantic versioning)
- Annotated: `v1.0.0-beta`, `v1.0.0-rc1`

## 13. Security Considerations

### 13.1 Input Validation
- Validate all external inputs
- Check buffer sizes
- Verify file paths

### 13.2 Buffer Security
- Use safe string functions
- Check buffer boundaries
- Initialize all variables

### 13.3 Integer Overflows
```cpp
// Bad: Potential overflow
int sample_count = file_size / (channels * sizeof(float));

// Good: Safe arithmetic
if (channels > 0 && sizeof(float) > 0) {
    int sample_count = file_size / (channels * sizeof(float));
}
```

## 14. Review Checklist

Before submitting code, verify:

### 14.1 Code Quality
- [ ] Code compiles without warnings
- [ ] All tests pass
- [ ] Code is properly formatted
- [ ] Documentation is complete

### 14.2 Functionality
- [ ] Implementation meets requirements
- [ ] Error handling is appropriate
- [] Edge cases are covered
- [ ] Performance is acceptable

### 14.3 Security
- [ ] No buffer overflows
- [ ] Input is validated
- [ ] Resources are released
- [ ] No memory leaks

These standards ensure consistent, maintainable, and high-quality code throughout the coreMusicPlayer project.