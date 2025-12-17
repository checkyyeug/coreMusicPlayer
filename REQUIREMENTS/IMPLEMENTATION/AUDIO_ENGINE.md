# coreMusicPlayer Audio Engine Documentation

## 1. Overview

The coreMusicPlayer Audio Engine is a high-performance, cross-platform audio processing system designed for low-latency playback and high-quality audio processing. It supports multiple audio formats, sample rate conversion, and platform-specific audio output APIs. **[重点]** The engine has been redesigned to support a unified player architecture with mode-based feature selection and lazy loading of components.

## 1.1 Key Architecture Changes

- **Unified Player**: Single engine supporting 5 different player modes
- **Lazy Loading**: Decoders and plugins loaded only when needed
- **Resource Pooling**: Efficient memory management with buffer pools
- **Feature Gating**: Fine-grained control over enabled features
- **Improved Thread Safety**: Better separation of audio, I/O, and UI threads

## 2. Architecture

### 2.1 Core Components

```
Unified Audio Engine
├── Player Mode Management
│   ├── Strategy Pattern (Legacy/Realtime/Production/etc.)
│   ├── Feature Manager (bitset-based gating)
│   └── Mode Switcher (runtime mode changes)
├── Audio Buffer Management
│   ├── AudioBuffer (SIMD-optimized, aligned)
│   ├── BufferPool (RAII, thread-safe)
│   ├── RingBuffer (lock-free for audio thread)
│   └── Memory Manager (aligned allocations)
├── Decoder Manager
│   ├── Lazy Loading System
│   ├── Format Registry
│   ├── WAV Decoder (dr_wag)
│   ├── MP3 Decoder (minimp3)
│   ├── FLAC Decoder (builtin)
│   └── OGG Decoder (stb_vorbis)
├── Audio Processing Pipeline
│   ├── Sample Rate Converter (SIMD-optimized)
│   ├── DSP Chain (plugin-based)
│   ├── Volume Control (atomic updates)
│   └── Channel Mixer (configurable)
├── Audio Output System
│   ├── WASAPI Output (Windows, low-latency)
│   ├── ALSA Output (Linux, real-time)
│   ├── CoreAudio Output (macOS, planned)
│   └── Device Manager (hot-plug support)
└── Threading Architecture
    ├── Audio Thread (real-time priority)
    ├── Decoder Thread (background pool)
    ├── I/O Thread (file operations)
    └── UI Thread (event callbacks)
```

### 2.2 Data Flow

```
Audio File
    ↓
File Loader
    ↓
Audio Decoder
    ↓
Sample Rate Converter
    ↓
Audio Buffer
    ↓
DSP Processing
    ↓
Audio Output
    ↓
Hardware Device
```

## 3. Audio Buffer Management

### 3.1 Enhanced AudioBuffer Class (Updated)

**[重点]** The AudioBuffer has been redesigned for better performance and memory management:

```cpp
class AudioBuffer {
public:
    // Constructors
    AudioBuffer(int channels, size_t frames);
    AudioBuffer(float* external_data, int channels, size_t frames, bool copy = false);
    ~AudioBuffer();

    // Data access
    float* getChannel(int channel);
    const float* getChannel(int channel) const;
    float* getInterleaved();
    const float* getInterleaved() const;

    // Buffer operations
    bool resize(size_t newFrames);
    void clear();
    void copyFrom(const AudioBuffer& other);
    void toPlanar();      // Convert interleaved to planar
    void toInterleaved(); // Convert planar to interleaved

    // Properties
    int getChannels() const { return channels_; }
    size_t getFrames() const { return frames_; }
    size_t getSamples() const { return channels_ * frames_; }

    // SIMD operations (auto-detected at runtime)
    void applyGain(float gain);
    void mixWith(const AudioBuffer& other, float gain = 1.0f);
    void applyFadeIn(size_t frames);
    void applyFadeOut(size_t frames);

private:
    float* data_;
    int channels_;
    size_t frames_;
    size_t capacity_;
    bool owns_data_;
    static constexpr size_t SIMD_ALIGNMENT = 32;

    // Memory management
    void allocateMemory();
    void deallocateMemory();

    // SIMD implementations
    void applyGainAVX2(float* data, size_t samples, float gain);
    void applyGainSSE2(float* data, size_t samples, float gain);
    void mixAVX2(float* dest, const float* src, size_t samples, float gain);
    void mixSSE2(float* dest, const float* src, size_t samples, float gain);
};
```

### 3.2 Memory Alignment

Audio buffers are aligned for SIMD operations:

```cpp
class AudioBuffer {
private:
    static constexpr size_t SIMD_ALIGNMENT = 32; // AVX requires 32-byte alignment

    void* allocate_aligned(size_t size) {
#ifdef _WIN32
        return _aligned_malloc(size, SIMD_ALIGNMENT);
#else
        void* ptr;
        if (posix_memalign(&ptr, SIMD_ALIGNMENT, size) != 0) {
            return nullptr;
        }
        return ptr;
#endif
    }
};
```

### 3.3 Advanced Buffer Pool (Updated)

**[重点]** Improved buffer pool with automatic resource management:

```cpp
class AudioBufferPool {
private:
    struct BufferInfo {
        std::unique_ptr<float[]> data;
        size_t channels;
        size_t frames;
        bool in_use;
        std::chrono::steady_clock::time_point last_used;
    };

    std::vector<BufferInfo> buffers_;
    mutable std::shared_mutex pool_mutex_;
    size_t alignment_ = 32;
    std::atomic<size_t> total_allocated_{0};
    std::atomic<size_t> max_allocation_{1024 * 1024 * 1024}; // 1GB limit

public:
    // RAII buffer handle
    class BufferHandle {
    private:
        AudioBuffer* buffer_;
        AudioBufferPool* pool_;

    public:
        BufferHandle(AudioBuffer* buffer, AudioBufferPool* pool)
            : buffer_(buffer), pool_(pool) {}

        ~BufferHandle() {
            if (pool_ && buffer_) {
                pool_->release(buffer_);
            }
        }

        AudioBuffer* get() const { return buffer_; }
        AudioBuffer* operator->() const { return buffer_; }
    };

    // Acquire buffer with automatic return
    std::unique_ptr<BufferHandle> acquire(size_t channels, size_t frames) {
        std::unique_lock lock(pool_mutex_);

        // Try to reuse existing buffer
        for (auto& buffer : buffers_) {
            if (!buffer.in_use && buffer.channels >= channels &&
                buffer.frames >= frames) {
                buffer.in_use = true;
                buffer.last_used = std::chrono::steady_clock::now();
                lock.unlock();

                auto audio_buffer = std::make_unique<AudioBuffer>(
                    buffer.data.get(), channels, frames);
                return std::make_unique<BufferHandle>(audio_buffer.get(), this);
            }
        }

        // Check memory limit
        size_t needed = channels * frames * sizeof(float);
        if (total_allocated_ + needed > max_allocation_) {
            pruneUnusedBuffers();
        }

        // Allocate new buffer
        float* data = allocateAlignedMemory(needed);
        buffers_.push_back({
            std::unique_ptr<float[]>(data),
            channels,
            frames,
            true,
            std::chrono::steady_clock::now()
        });

        total_allocated_ += needed;

        lock.unlock();

        auto audio_buffer = std::make_unique<AudioBuffer>(
            data, channels, frames);
        return std::make_unique<BufferHandle>(audio_buffer.get(), this);
    }

    // Release buffer back to pool
    void release(AudioBuffer* buffer) {
        if (!buffer) return;

        std::lock_guard lock(pool_mutex_);

        for (auto& buf : buffers_) {
            if (buf.data.get() == buffer->data()) {
                buf.in_use = false;
                buf.last_used = std::chrono::steady_clock::now();
                break;
            }
        }
    }

    // Clean up old unused buffers
    void pruneUnusedBuffers(std::chrono::minutes max_age = std::chrono::minutes(5)) {
        std::lock_guard lock(pool_mutex_);

        auto now = std::chrono::steady_clock::now();
        auto it = std::remove_if(buffers_.begin(), buffers_.end(),
            [this, &now, max_age](const BufferInfo& buffer) {
                if (!buffer.in_use &&
                    (now - buffer.last_used) > max_age) {
                    total_allocated_ -= buffer.channels * buffer.frames * sizeof(float);
                    return true;
                }
                return false;
            });

        buffers_.erase(it, buffers_.end());
    }

private:
    float* allocateAlignedMemory(size_t size) {
#ifdef _WIN32
        return static_cast<float*>(_aligned_malloc(size, alignment_));
#else
        void* ptr = nullptr;
        if (posix_memalign(&ptr, alignment_, size) != 0) {
            throw std::bad_alloc();
        }
        return static_cast<float*>(ptr);
#endif
    }
};
```

## 4. Audio Decoders

### 4.1 Decoder Manager (New)

**[重点]** The DecoderManager provides lazy loading and caching of audio decoders:

```cpp
class DecoderManager {
private:
    std::unordered_map<std::string, std::unique_ptr<AudioDecoder>> decoders_;
    std::map<std::string, std::function<std::unique_ptr<AudioDecoder>()>> decoder_factories_;
    mutable std::shared_mutex decoder_mutex_;
    std::unique_ptr<AudioBufferPool> buffer_pool_;

public:
    DecoderManager() : buffer_pool_(std::make_unique<AudioBufferPool>()) {
        // Register decoder factories
        registerDefaultDecoders();
    }

    // Get decoder for format (lazy loading)
    AudioDecoder* getDecoder(const std::string& format) {
        std::shared_lock lock(decoder_mutex_);

        auto it = decoders_.find(format);
        if (it != decoders_.end()) {
            return it->second.get();
        }

        lock.unlock();

        // Need to load the decoder
        std::unique_lock write_lock(decoder_mutex_);

        // Double-check after acquiring write lock
        it = decoders_.find(format);
        if (it != decoders_.end()) {
            return it->second.get();
        }

        auto factory_it = decoder_factories_.find(format);
        if (factory_it == decoder_factories_.end()) {
            return nullptr;
        }

        auto decoder = factory_it->second();
        if (!decoder->initialize()) {
            return nullptr;
        }

        AudioDecoder* decoder_ptr = decoder.get();
        decoders_[format] = std::move(decoder);

        return decoder_ptr;
    }

    // Preload decoder (useful for startup optimization)
    void preloadDecoder(const std::string& format) {
        getDecoder(format);
    }

    // Unload decoder to free memory
    void unloadDecoder(const std::string& format) {
        std::unique_lock lock(decoder_mutex_);
        decoders_.erase(format);
    }

    // Get buffer pool for audio operations
    AudioBufferPool* getBufferPool() const {
        return buffer_pool_.get();
    }

private:
    void registerDefaultDecoders() {
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
    }
};
```

### 4.2 Enhanced Decoder Interface (Updated)

```cpp
class AudioDecoder {
public:
    virtual ~AudioDecoder() = default;

    // Format detection
    virtual bool canDecode(const std::string& filename) = 0;

    // File operations
    virtual bool open(const std::string& filename) = 0;
    virtual void close() = 0;

    // Audio information
    virtual int getSampleRate() const = 0;
    virtual int getChannels() const = 0;
    virtual int getBitDepth() const = 0;
    virtual size_t getFrames() const = 0;

    // Decoding
    virtual size_t decode(float* buffer, size_t frames) = 0;
    virtual bool seek(size_t frame) = 0;

    // Metadata
    virtual std::map<std::string, std::string> getMetadata() const = 0;
};
```

### 4.2 WAV Decoder (dr_wav)

```cpp
class WAVDecoder : public AudioDecoder {
public:
    WAVDecoder() : wav_(nullptr) {}

    bool canDecode(const std::string& filename) override {
        return filename.find(".wav") != std::string::npos ||
               filename.find(".WAV") != std::string::npos;
    }

    bool open(const std::string& filename) override {
        wav_ = drwav_open_file(filename.c_str());
        return wav_ != nullptr;
    }

    size_t decode(float* buffer, size_t frames) override {
        if (!wav_) return 0;

        // Convert samples to float
        drwav_int32* temp = new drwav_int32[frames * wav_->channels];
        size_t read = drwav_read_pcm_frames_s32(wav_, frames, temp);

        // Convert to float and interleave
        for (size_t i = 0; i < read; ++i) {
            for (int c = 0; c < wav_->channels; ++c) {
                buffer[i * wav_->channels + c] =
                    temp[i * wav_->channels + c] / 2147483648.0f;
            }
        }

        delete[] temp;
        return read;
    }

private:
    drwav* wav_;
};
```

### 4.3 MP3 Decoder (minimp3)

```cpp
class MP3Decoder : public AudioDecoder {
public:
    MP3Decoder() : file_(nullptr), file_size_(0) {}

    bool open(const std::string& filename) override {
        file_ = fopen(filename.c_str(), "rb");
        if (!file_) return false;

        // Get file size
        fseek(file_, 0, SEEK_END);
        file_size_ = ftell(file_);
        fseek(file_, 0, SEEK_SET);

        // Read entire file
        file_data_.resize(file_size_);
        fread(file_data_.data(), 1, file_size_, file_);

        return true;
    }

    size_t decode(float* buffer, size_t frames) override {
        mp3dec_frame_info_t info;
        size_t samples = mp3dec_decode_f32(
            &mp3d_,
            file_data_.data() + data_offset_,
            file_size_ - data_offset_,
            buffer,
            frames * channels_,
            &info
        );

        data_offset_ += info.frame_bytes;
        return samples / channels_;
    }

private:
    FILE* file_;
    size_t file_size_;
    size_t data_offset_;
    std::vector<uint8_t> file_data_;
    mp3dec_t mp3d_;
};
```

## 5. Sample Rate Conversion

### 5.1 Converter Interface

```cpp
enum class Quality {
    Fast,      // Linear interpolation
    Good,      // Cubic interpolation
    Best,      // Sinc interpolation
    High       // High-quality sinc with pre-computed tables
};

class SampleRateConverter {
public:
    SampleRateConverter(int inputRate, int outputRate, Quality quality = Quality::Good);
    ~SampleRateConverter();

    // Conversion
    size_t convert(const float* input, size_t inputFrames,
                   float* output, size_t maxOutputFrames);

    // Configuration
    void setQuality(Quality quality);
    double getRatio() const { return outputRate_ / inputRate_; }

    // State
    void reset();
    size_t getLatency() const;

private:
    int inputRate_;
    int outputRate_;
    Quality quality_;
    void* state_;  // Implementation-specific state
};
```

### 5.2 Linear Interpolation (Fast)

```cpp
class LinearSRC {
public:
    size_t convert(const float* input, size_t inputFrames,
                   float* output, size_t maxOutputFrames) {
        double ratio = outputRate_ / inputRate_;
        size_t outputFrames = 0;

        for (size_t i = 0; i < inputFrames - 1 && outputFrames < maxOutputFrames; ++i) {
            double pos = i * ratio;
            size_t index = static_cast<size_t>(pos);
            double frac = pos - index;

            if (index + 1 < inputFrames) {
                float sample = input[index] * (1.0f - frac) +
                              input[index + 1] * frac;
                output[outputFrames++] = sample;
            }
        }

        return outputFrames;
    }
};
```

### 5.3 Cubic Interpolation (Good)

```cpp
class CubicSRC {
public:
    float cubicInterpolate(float y0, float y1, float y2, float y3, float t) {
        float a0 = y3 - y2 - y0 + y1;
        float a1 = y0 - y1 - a0;
        float a2 = y2 - y0;
        float a3 = y1;

        return a0 * t * t * t + a1 * t * t + a2 * t + a3;
    }

    size_t convert(const float* input, size_t inputFrames,
                   float* output, size_t maxOutputFrames) {
        double ratio = outputRate_ / inputRate_;
        size_t outputFrames = 0;

        for (size_t i = 0; i < inputFrames - 3 && outputFrames < maxOutputFrames; ++i) {
            double pos = i * ratio;
            size_t index = static_cast<size_t>(pos);
            double frac = pos - index;

            if (index + 3 < inputFrames) {
                float sample = cubicInterpolate(
                    input[index],
                    input[index + 1],
                    input[index + 2],
                    input[index + 3],
                    static_cast<float>(frac)
                );
                output[outputFrames++] = sample;
            }
        }

        return outputFrames;
    }
};
```

### 5.4 SIMD Optimized Processing

```cpp
#ifdef __SSE2__
void processAudioSSE2(float* buffer, size_t frames, float gain) {
    __m128 gainVec = _mm_set1_ps(gain);

    for (size_t i = 0; i < frames; i += 4) {
        __m128 samples = _mm_load_ps(buffer + i);
        samples = _mm_mul_ps(samples, gainVec);
        _mm_store_ps(buffer + i, samples);
    }
}
#endif

#ifdef __AVX__
void processAudioAVX(float* buffer, size_t frames, float gain) {
    __m256 gainVec = _mm256_set1_ps(gain);

    for (size_t i = 0; i < frames; i += 8) {
        __m256 samples = _mm256_load_ps(buffer + i);
        samples = _mm256_mul_ps(samples, gainVec);
        _mm256_store_ps(buffer + i, samples);
    }
}
#endif
```

## 6. Audio Output

### 6.1 AudioOutput Interface

```cpp
class AudioOutput {
public:
    virtual ~AudioOutput() = default;

    // Device management
    virtual bool initialize(const AudioFormat& format) = 0;
    virtual void cleanup() = 0;
    virtual std::vector<std::string> getDevices() = 0;
    virtual bool setDevice(const std::string& deviceId) = 0;

    // Stream control
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual bool pause() = 0;

    // Buffer management
    virtual size_t getBufferSize() const = 0;
    virtual void setBufferSize(size_t frames) = 0;
    virtual double getLatency() const = 0;

    // Callback
    virtual void setCallback(std::function<float*(size_t frames)> callback) = 0;

protected:
    AudioFormat format_;
    std::atomic<State> state_{State::Stopped};
};
```

### 6.2 WASAPI Output (Windows)

```cpp
class WASAPIOutput : public AudioOutput {
public:
    bool initialize(const AudioFormat& format) override {
        HRESULT hr;

        // Initialize COM
        hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
        if (FAILED(hr)) return false;

        // Create audio client
        hr = device_->Activate(
            __uuidof(IAudioClient),
            CLSCTX_ALL,
            nullptr,
            reinterpret_cast<void**>(&audioClient_)
        );

        if (FAILED(hr)) return false;

        // Get mix format
        WAVEFORMATEX* mixFormat;
        hr = audioClient_->GetMixFormat(&mixFormat);
        if (FAILED(hr)) return false;

        // Configure for low latency
        hr = audioClient_->Initialize(
            AUDCLNT_SHAREMODE_SHARED,
            AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
            0,  // Buffer duration
            0,  // Periodicity
            mixFormat,
            nullptr
        );

        // Set event handle for notifications
        hr = audioClient_->SetEventHandle(audioEvent_);

        return SUCCEEDED(hr);
    }

    bool start() override {
        HRESULT hr = audioClient_->Start();
        if (SUCCEEDED(hr)) {
            state_ = State::Playing;
            startAudioThread();
        }
        return SUCCEEDED(hr);
    }

private:
    void audioThread() {
        while (state_ == State::Playing) {
            // Wait for buffer ready
            WaitForSingleObject(audioEvent_, 200);

            // Get buffer
            UINT32 padding;
            audioClient_->GetCurrentPadding(&padding);

            UINT32 framesAvailable;
            audioClient_->GetBufferSize(&framesAvailable);
            framesAvailable -= padding;

            if (framesAvailable > 0) {
                BYTE* data;
                HRESULT hr = renderClient_->GetBuffer(
                    framesAvailable, &data);

                if (SUCCEEDED(hr)) {
                    // Fill buffer with audio data
                    fillBuffer(reinterpret_cast<float*>(data), framesAvailable);

                    renderClient_->ReleaseBuffer(framesAvailable, 0);
                }
            }
        }
    }

    IAudioClient* audioClient_;
    IAudioRenderClient* renderClient_;
    HANDLE audioEvent_;
    std::thread audioThread_;
};
```

### 6.3 ALSA Output (Linux)

```cpp
class ALSAOutput : public AudioOutput {
public:
    bool initialize(const AudioFormat& format) override {
        int err;

        // Open PCM device
        err = snd_pcm_open(&pcm_, "default", SND_PCM_STREAM_PLAYBACK, 0);
        if (err < 0) return false;

        // Set hardware parameters
        snd_pcm_hw_params_t* hwParams;
        snd_pcm_hw_params_alloca(&hwParams);

        snd_pcm_hw_params_any(pcm_, hwParams);
        snd_pcm_hw_params_set_access(pcm_, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED);
        snd_pcm_hw_params_set_format(pcm_, hwParams, SND_PCM_FORMAT_FLOAT_LE);
        snd_pcm_hw_params_set_channels(pcm_, hwParams, format.channels);
        snd_pcm_hw_params_set_rate(pcm_, hwParams, format.sampleRate, 0);

        // Set buffer size
        snd_pcm_hw_params_set_buffer_size(pcm_, hwParams, 4096);
        snd_pcm_hw_params_set_period_size(pcm_, hwParams, 1024, 0);

        err = snd_pcm_hw_params(pcm_, hwParams);
        if (err < 0) return false;

        return true;
    }

    bool start() override {
        state_ = State::Playing;
        startAudioThread();
        return true;
    }

private:
    void audioThread() {
        float* buffer = new float[4096 * format_.channels];

        while (state_ == State::Playing) {
            // Get audio data
            size_t frames = getAudioData(buffer, 4096);

            if (frames > 0) {
                snd_pcm_sframes_t framesWritten = snd_pcm_writei(
                    pcm_, buffer, frames);

                if (framesWritten < 0) {
                    // Handle underrun
                    snd_pcm_prepare(pcm_);
                }
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        }

        delete[] buffer;
    }

    snd_pcm_t* pcm_;
    std::thread audioThread_;
};
```

## 7. DSP Processing

### 7.1 Volume Control

```cpp
class VolumeControl {
public:
    VolumeControl() : volume_(1.0f) {}

    void process(float* buffer, size_t frames, int channels) {
        if (volume_ == 1.0f) return;  // No processing needed

#ifdef __SSE2__
        processSSE2(buffer, frames * channels, volume_);
#else
        for (size_t i = 0; i < frames * channels; ++i) {
            buffer[i] *= volume_;
        }
#endif
    }

    void setVolume(float volume) {
        volume_ = std::max(0.0f, std::min(1.0f, volume));
    }

    float getVolume() const { return volume_; }

private:
    std::atomic<float> volume_;
};
```

### 7.2 Equalizer

```cpp
class Equalizer {
public:
    Equalizer(int sampleRate) : sampleRate_(sampleRate) {
        bands_.resize(10);
        for (auto& band : bands_) {
            band.frequency = 0.0f;
            band.gain = 0.0f;
            band.q = 1.0f;
        }
    }

    void process(float* buffer, size_t frames, int channels) {
        // Process each channel
        for (int ch = 0; ch < channels; ++ch) {
            for (size_t i = 0; i < frames; ++i) {
                float sample = buffer[i * channels + ch];

                // Apply each band
                for (auto& band : bands_) {
                    if (band.frequency > 0.0f) {
                        sample = applyBand(sample, band, sampleRate_);
                    }
                }

                buffer[i * channels + ch] = sample;
            }
        }
    }

private:
    struct Band {
        float frequency;
        float gain;  // in dB
        float q;
    };

    std::vector<Band> bands_;
    int sampleRate_;

    float applyBand(float sample, const Band& band, int sampleRate) {
        // Biquad filter implementation
        // Simplified for example
        return sample * std::pow(10.0f, band.gain / 20.0f);
    }
};
```

## 8. Performance Optimization

### 8.1 Real-Time Considerations

1. **Avoid Memory Allocations in Audio Thread**
   ```cpp
   class AudioProcessor {
   public:
       AudioProcessor() {
           // Pre-allocate all buffers
           bufferPool_.initialize(10, 4096, 2);
       }

       void processAudio(float* input, float* output, size_t frames) {
           // Use pre-allocated buffers
           auto buffer = bufferPool_.acquire();

           // Process without allocations
           process(input, buffer->data(), frames);

           bufferPool_.release(std::move(buffer));
       }
   };
   ```

2. **Lock-Free Data Structures**
   ```cpp
   template<typename T>
   class LockFreeQueue {
   public:
       bool push(const T& item) {
           // Implement lock-free push
           return queue_.push(item);
       }

       bool pop(T& item) {
           // Implement lock-free pop
           return queue_.pop(item);
       }

   private:
       moodycamel::ConcurrentQueue<T> queue_;
   };
   ```

3. **CPU Affinity and Priority**
   ```cpp
   void setAudioThreadPriority() {
#ifdef _WIN32
       SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
       SetThreadAffinityMask(GetCurrentThread(), 0x01);  // CPU 0
#else
       struct sched_param param;
       param.sched_priority = 99;
       pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);

       cpu_set_t cpuset;
       CPU_ZERO(&cpuset);
       CPU_SET(0, &cpuset);
       pthread_setaffinity_np(pthread_self(), sizeof(cpuset), &cpuset);
#endif
   }
   ```

### 8.2 SIMD Optimizations

```cpp
class AudioProcessorSIMD {
public:
    void processStereo(float* left, float* right, size_t frames, float gain) {
        // Check CPU features at runtime
        if (hasAVX2_) {
            processAVX2(left, right, frames, gain);
        } else if (hasSSE2_) {
            processSSE2(left, right, frames, gain);
        } else {
            processScalar(left, right, frames, gain);
        }
    }

private:
#ifdef __AVX2__
    void processAVX2(float* left, float* right, size_t frames, float gain) {
        __m256 gainVec = _mm256_set1_ps(gain);

        for (size_t i = 0; i < frames; i += 8) {
            // Load 8 samples from each channel
            __m256 leftVec = _mm256_load_ps(left + i);
            __m256 rightVec = _mm256_load_ps(right + i);

            // Apply gain
            leftVec = _mm256_mul_ps(leftVec, gainVec);
            rightVec = _mm256_mul_ps(rightVec, gainVec);

            // Store back
            _mm256_store_ps(left + i, leftVec);
            _mm256_store_ps(right + i, rightVec);
        }
    }
#endif

    bool hasAVX2_ = false;
    bool hasSSE2_ = false;
};
```

## 9. Error Handling

### 9.1 Error Types

```cpp
enum class AudioError {
    NoError = 0,
    DeviceNotFound,
    DeviceInUse,
    InvalidFormat,
    BufferUnderrun,
    BufferOverrun,
    InitializationFailed,
    PermissionDenied,
    FileNotFound,
    CorruptFile,
    UnsupportedFormat
};

class AudioException : public std::exception {
public:
    AudioException(AudioError error, const std::string& message)
        : error_(error), message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

    AudioError getError() const { return error_; }

private:
    AudioError error_;
    std::string message_;
};
```

### 9.2 Error Recovery

```cpp
class AudioOutput {
public:
    bool start() {
        try {
            if (!initializeDevice()) {
                throw AudioException(AudioError::InitializationFailed,
                                   "Failed to initialize audio device");
            }

            startAudioThread();
            state_ = State::Playing;
            return true;

    }
}
```

## 10. Error Handling and Recovery (Updated)

### 10.1 Unified Error Handling

Using the Result<T> type for thread-safe error propagation:

```cpp
// Thread-safe audio output with Result-based error handling
class AudioOutput {
private:
    std::atomic<State> state_{State::Stopped};
    std::atomic<Error> last_error_;
    moodycamel::ConcurrentQueue<AudioCommand> command_queue_;

public:
    // Thread-safe start with Result
    VoidResult start() {
        // Validate state
        State expected = State::Stopped;
        if (!state_.compare_exchange_strong(expected, State::Starting)) {
            return VoidResult::error(
                Error(ErrorCategory::InvalidState,
                      "Cannot start: device already running or transitioning")
                    .addContext("current_state", static_cast<int>(expected)));
        }

        auto initResult = initializeDevice();
        if (initResult.isError()) {
            state_.store(State::Stopped);
            last_error_.store(initResult.getError());
            return initResult;
        }

        auto threadResult = startAudioThread();
        if (threadResult.isError()) {
            state_.store(State::Stopped);
            last_error_.store(threadResult.getError());
            cleanupDevice();
            return threadResult;
        }

        state_.store(State::Playing);
        return VoidResult::success();
    }

    // Real-time audio callback with error handling
    void audioCallback(float* output, size_t frames) noexcept {
        // Fast path - no error checking in hot path
        if (state_.load(std::memory_order_acquire) != State::Playing) {
            // Clear buffer to silence
            std::fill_n(output, frames * format_.channels, 0.0f);
            return;
        }

        // Try to get audio data
        auto dataResult = audioBufferPool_.tryDequeue(frames);
        if (dataResult.isSuccess()) {
            auto& buffer = dataResult.getValue();
            std::copy(buffer.data(), buffer.data() + buffer.size(), output);
        } else {
            // Handle underrun
            std::fill_n(output, frames * format_.channels, 0.0f);

            // Report error without blocking
            Error underrunError(ErrorCategory::AudioBufferUnderrun,
                              "Audio buffer underrun detected",
                              ErrorSeverity::Warning);
            last_error_.store(underrunError);
            reportErrorAsync(underrunError);

            // Try to recover
            handleBufferUnderrun();
        }
    }

private:
    void handleBufferUnderrun() {
        // Increase buffer size gradually
        if (buffer_size_ < MAX_BUFFER_SIZE) {
            buffer_size_ = std::min(buffer_size_ * 2, MAX_BUFFER_SIZE);

            // Queue command to reconfigure buffers
            AudioCommand cmd{AudioCommand::ReconfigureBuffers};
            cmd.buffer_size = buffer_size_;
            command_queue_.enqueue(cmd);
        }
    }
};
```

### 10.2 Error Recovery Mechanisms

```cpp
class ErrorRecoveryManager {
private:
    std::unordered_map<ErrorCategory, std::function<BoolResult(const Error&)>> recovery_strategies_;

public:
    ErrorRecoveryManager() {
        // Register recovery strategies
        recovery_strategies_[ErrorCategory::AudioDeviceNotFound] =
            [this](const Error& err) { return recoverFromDeviceError(err); };
        recovery_strategies_[ErrorCategory::AudioBufferUnderrun] =
            [this](const Error& err) { return recoverFromBufferUnderrun(err); };
        recovery_strategies_[ErrorCategory::PluginLoadFailed] =
            [this](const Error& err) { return recoverFromPluginError(err); };
    }

    BoolResult attemptRecovery(const Error& error) {
        auto it = recovery_strategies_.find(error.getCategory());
        if (it != recovery_strategies_.end()) {
            return it->second(error);
        }

        // No recovery strategy available
        return BoolResult::error(error);
    }

private:
    BoolResult recoverFromDeviceError(const Error& error) {
        // Try to find alternative device
        auto devices = AudioDeviceManager::enumerateDevices();
        for (const auto& device : devices) {
            if (device.id != error.getContext("failed_device")) {
                auto initResult = AudioDeviceManager::initializeDevice(device);
                if (initResult.isSuccess()) {
                    logInfo("Recovered from device error using: " + device.name);
                    return BoolResult::success(true);
                }
            }
        }

        // All devices failed
        return BoolResult::error(
            Error(ErrorCategory::AudioDeviceNotFound,
                  "No working audio devices available",
                  ErrorSeverity::Critical));
    }

    BoolResult recoverFromBufferUnderrun(const Error& error) {
        // Increase buffer size
        size_t currentSize = std::stoull(error.getContext("buffer_size"));
        size_t newSize = std::min(currentSize * 2, MAX_BUFFER_SIZE);

        auto reconfigureResult = AudioBufferManager::reconfigureBuffers(newSize);
        if (reconfigureResult.isSuccess()) {
            logWarning("Recovered from buffer underrun by increasing buffer to " +
                      std::to_string(newSize));
            return BoolResult::success(true);
        }

        return BoolResult::error(
            Error(ErrorCategory::AudioBufferOverrun,
                  "Failed to recover from buffer underrun"));
    }
};
```

## 11. Thread Safety Guidelines (New)

### 11.1 Thread-Safe API Design

```cpp
// Thread-safe player interface
class ThreadSafeAudioPlayer {
private:
    mutable std::shared_mutex state_mutex_;
    std::atomic<PlayerState> atomic_state_{PlayerState::Stopped};
    std::atomic<float> volume_{1.0f};

    // Use atomic flags for simple state
    std::atomic<bool> is_seeking_{false};
    std::atomic<uint64_t> seek_position_{0};

    // Thread-safe queue for commands
    moodycamel::ConcurrentQueue<PlayerCommand> command_queue_;

public:
    // Thread-safe play/pause
    VoidResult play() {
        PlayerState expected = PlayerState::Stopped;
        if (atomic_state_.compare_exchange_strong(expected, PlayerState::Playing)) {
            // Notify audio thread
            command_queue_.enqueue({PlayerCommand::StartPlayback});
            return VoidResult::success();
        }
        return VoidResult::error(
            Error(ErrorCategory::InvalidState, "Already playing"));
    }

    VoidResult pause() {
        PlayerState expected = PlayerState::Playing;
        if (atomic_state_.compare_exchange_strong(expected, PlayerState::Paused)) {
            command_queue_.enqueue({PlayerCommand::PausePlayback});
            return VoidResult::success();
        }
        return VoidResult::error(
            Error(ErrorCategory::InvalidState, "Not playing"));
    }

    // Thread-safe volume control
    void setVolume(float newVolume) {
        volume_.store(std::clamp(newVolume, 0.0f, 1.0f),
                      std::memory_order_relaxed);
    }

    float getVolume() const {
        return volume_.load(std::memory_order_relaxed);
    }

    // Thread-safe seek with atomic operations
    VoidResult seek(double position) {
        // Set seek flag and position atomically
        seek_position_.store(static_cast<uint64_t>(position * getSampleRate()),
                           std::memory_order_release);
        is_seeking_.store(true, std::memory_order_release);

        // Command audio thread
        command_queue_.enqueue({PlayerCommand::Seek});
        return VoidResult::success();
    }

    // Audio thread checks for seek requests
    bool checkSeekRequest(uint64_t& outPosition) {
        if (is_seeking_.load(std::memory_order_acquire)) {
            outPosition = seek_position_.load(std::memory_order_acquire);
            is_seeking_.store(false, std::memory_order_release);
            return true;
        }
        return false;
    }

    // Thread-safe state query
    PlayerState getState() const {
        // Read atomic value for fast path
        return atomic_state_.load(std::memory_order_acquire);
    }

    // For complex operations, use shared mutex
    TrackInfo getCurrentTrack() const {
        std::shared_lock lock(state_mutex_);
        return current_track_;
    }
};
```

### 11.2 Lock-Free Data Structures

```cpp
// Lock-free audio buffer queue for real-time audio
template<typename T, size_t Size>
class LockFreeQueue {
private:
    static constexpr size_t BUFFER_SIZE = Size + 1; // One extra slot
    std::array<T, BUFFER_SIZE> buffer_;
    alignas(64) std::atomic<size_t> write_index_{0};
    alignas(64) std::atomic<size_t> read_index_{0};

public:
    bool enqueue(const T& item) {
        const size_t current_write = write_index_.load(std::memory_order_relaxed);
        const size_t next_write = (current_write + 1) % BUFFER_SIZE;

        if (next_write == read_index_.load(std::memory_order_acquire)) {
            // Queue is full
            return false;
        }

        buffer_[current_write] = item;
        write_index_.store(next_write, std::memory_order_release);
        return true;
    }

    bool dequeue(T& out_item) {
        const size_t current_read = read_index_.load(std::memory_order_relaxed);

        if (current_read == write_index_.load(std::memory_order_acquire)) {
            // Queue is empty
            return false;
        }

        out_item = buffer_[current_read];
        read_index_.store((current_read + 1) % BUFFER_SIZE,
                         std::memory_order_release);
        return true;
    }

    size_t size() const {
        const size_t write = write_index_.load(std::memory_order_acquire);
        const size_t read = read_index_.load(std::memory_order_acquire);

        if (write >= read) {
            return write - read;
        } else {
            return BUFFER_SIZE + write - read;
        }
    }

    bool empty() const {
        return write_index_.load(std::memory_order_acquire) ==
               read_index_.load(std::memory_order_acquire);
    }
};

// Usage in audio engine
class RealTimeAudioEngine {
private:
    LockFreeQueue<AudioBuffer, 64> audio_queue_;
    std::atomic<AudioProcessingState> state_{AudioProcessingState::Idle};

public:
    void audioCallback(float* output, size_t frames) noexcept {
        // Fast path - check state atomically
        if (state_.load(std::memory_order_acquire) != AudioProcessingState::Running) {
            std::fill_n(output, frames * channels_, 0.0f);
            return;
        }

        AudioBuffer buffer;
        if (audio_queue_.dequeue(buffer)) {
            // Process buffer
            std::copy(buffer.data(), buffer.data() + buffer.size(), output);
        } else {
            // Buffer underrun
            std::fill_n(output, frames * channels_, 0.0f);
        }
    }
};
```

## 12. Testing

### 12.1 Unit Tests

```cpp
TEST(AudioBufferTest, BasicOperations) {
    AudioBuffer buffer(2, 1024);  // 2 channels, 1024 frames

    EXPECT_EQ(buffer.getChannels(), 2);
    EXPECT_EQ(buffer.getFrames(), 1024);
    EXPECT_EQ(buffer.getSamples(), 2048);

    // Test resize
    EXPECT_TRUE(buffer.resize(2048));
    EXPECT_EQ(buffer.getFrames(), 2048);

    // Test channel access
    float* left = buffer.getChannel(0);
    float* right = buffer.getChannel(1);
    EXPECT_NE(left, nullptr);
    EXPECT_NE(right, nullptr);
    EXPECT_NE(left, right);
}

TEST(SampleRateConverterTest, Conversion) {
    SampleRateConverter src(44100, 48000, Quality::Good);

    // Create test sine wave at 44100 Hz
    std::vector<float> input(4410);
    for (size_t i = 0; i < input.size(); ++i) {
        input[i] = sin(2.0 * M_PI * 440.0 * i / 44100.0);
    }

    // Convert to 48000 Hz
    std::vector<float> output(4800);
    size_t outputFrames = src.convert(input.data(), input.size(),
                                     output.data(), output.size());

    // Verify output length
    EXPECT_NEAR(outputFrames, 4800, 10);
}
```

### 10.2 Performance Tests

```cpp
BENCHMARK(AudioProcessing_Baseline) {
    AudioBuffer buffer(2, 4096);

    for (auto _ : state) {
        // Apply gain
        for (size_t i = 0; i < buffer.getSamples(); ++i) {
            buffer.getChannel(0)[i] *= 0.5f;
        }
    }
}

BENCHMARK(AudioProcessing_SIMD) {
    AudioBuffer buffer(2, 4096);

    for (auto _ : state) {
        // Apply gain with SIMD
        applyGainSIMD(buffer.getChannel(0), buffer.getSamples(), 0.5f);
    }
}
```

This audio engine documentation provides comprehensive details about the core audio processing system in coreMusicPlayer, enabling developers to understand and extend the audio functionality effectively.