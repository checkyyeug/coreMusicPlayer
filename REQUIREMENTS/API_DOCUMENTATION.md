# coreMusicPlayer API Documentation

## Table of Contents
1. [Overview](#1-overview)
2. [Core Classes](#2-core-classes)
3. [Audio Processing APIs](#3-audio-processing-apis)
4. [Plugin System APIs](#4-plugin-system-apis)
5. [Configuration APIs](#5-configuration-apis)
6. [Platform APIs](#6-platform-apis)
7. [Utility APIs](#7-utility-apis)
8. [Examples](#8-examples)

## 1. Overview

coreMusicPlayer provides a C++ API for audio playback, processing, and plugin management. The API is designed to be simple yet powerful, supporting both basic playback and advanced audio processing scenarios.

### Key Concepts

- **AudioBuffer**: Container for audio data samples
- **AudioFormat**: Describes audio format (sample rate, channels, bit depth)
- **AudioOutput**: Platform-specific audio output interface
- **MusicPlayer**: High-level player interface
- **PluginManager**: Manages Foobar2000-compatible plugins

### Headers Structure

```cpp
// Core headers
#include "core/music_player.h"
#include "core/config_manager.h"
#include "core/foobar_plugin_manager.h"

// Audio processing
#include "audio/audio_buffer.h"
#include "audio/sample_rate_converter.h"
#include "audio/wav_parser.h"

// Platform abstraction
#include "platform/audio_output.h"
```

## 2. Core Classes

### 2.1 MusicPlayer

The main class for audio playback functionality.

```cpp
class MusicPlayer {
public:
    // Constructor/Destructor
    MusicPlayer();
    ~MusicPlayer();

    // Initialization
    bool initialize(const AudioFormat& format = AudioFormat::getDefault());
    void shutdown();

    // File operations
    bool loadFile(const std::string& filename);
    bool closeFile();
    bool isFileLoaded() const;

    // Playback control
    bool play();
    bool pause();
    bool stop();
    bool seek(double seconds);
    bool seekPercent(double percent);

    // State information
    enum State {
        STOPPED,
        PLAYING,
        PAUSED,
        ERROR
    };

    State getState() const;
    double getPosition() const;     // Current position in seconds
    double getDuration() const;     // Total duration in seconds
    float getVolume() const;

    // Volume control
    void setVolume(float volume);   // 0.0 to 1.0

    // Audio format
    AudioFormat getFormat() const;

    // Callbacks
    typedef std::function<void(State)> StateChangeCallback;
    typedef std::function<void(double)> PositionCallback;

    void setStateChangeCallback(StateChangeCallback callback);
    void setPositionCallback(PositionCallback callback);

private:
    // Implementation details...
};
```

#### Example Usage

```cpp
#include "core/music_player.h"

int main() {
    MusicPlayer player;

    // Initialize with default format
    if (!player.initialize()) {
        std::cerr << "Failed to initialize player" << std::endl;
        return 1;
    }

    // Load and play a file
    if (player.loadFile("music.wav")) {
        player.setVolume(0.8f);  // 80% volume
        player.play();

        // Wait for playback to complete
        while (player.getState() == MusicPlayer::PLAYING) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    return 0;
}
```

### 2.2 AudioBuffer

Container for audio sample data with SIMD optimization support.

```cpp
class AudioBuffer {
public:
    // Constructors
    AudioBuffer();
    AudioBuffer(int channels, size_t frames);
    AudioBuffer(const AudioFormat& format, size_t frames);

    // Copy operations
    AudioBuffer(const AudioBuffer& other);
    AudioBuffer& operator=(const AudioBuffer& other);

    // Move operations
    AudioBuffer(AudioBuffer&& other) noexcept;
    AudioBuffer& operator=(AudioBuffer&& other) noexcept;

    // Basic properties
    int getChannels() const;
    size_t getFrames() const;
    size_t getSize() const;  // Total number of samples
    AudioFormat getFormat() const;

    // Data access
    float* getData();
    const float* getData() const;
    float& operator()(size_t frame, int channel);
    const float& operator()(size_t frame, int channel) const;
    float getSample(int channel, size_t frame) const;
    void setSample(int channel, size_t frame, float value);

    // Buffer operations
    void clear();
    void fill(float value);
    void resize(size_t frames);
    void resize(int channels, size_t frames);

    // Channel operations
    float* getChannelData(int channel);
    const float* getChannelData(int channel) const;

    // Copy operations
    void copyFrom(const AudioBuffer& other);
    void copyFrom(const float* data, size_t frames);

    // Interleaving
    void interleave(float* output) const;
    void deinterleave(const float* input);

    // Processing
    void applyGain(float gain);
    void applyGain(float gain, int startFrame, size_t numFrames);

    // Format conversion
    void toPCM16(int16_t* output) const;
    void toPCM24(int32_t* output) const;
    void toFloat32(float* output) const;

    // Utility
    bool isEmpty() const;
    size_t getMemoryUsage() const;
};
```

#### Example Usage

```cpp
// Create a stereo buffer for 1 second of 44.1kHz audio
AudioBuffer buffer(2, 44100);

// Fill with a sine wave
for (size_t i = 0; i < buffer.getFrames(); ++i) {
    float sample = 0.5f * sinf(2.0f * M_PI * 440.0f * i / 44100.0f);
    buffer.setSample(0, i, sample);  // Left channel
    buffer.setSample(1, i, sample);  // Right channel
}

// Apply gain
buffer.applyGain(0.8f);

// Convert to 16-bit PCM
std::vector<int16_t> pcm(buffer.getSize());
buffer.toPCM16(pcm.data());
```

### 2.3 AudioFormat

Describes audio format specifications.

```cpp
class AudioFormat {
public:
    // Bit depth constants
    enum BitDepth {
        INT16 = 16,
        INT24 = 24,
        INT32 = 32,
        FLOAT32 = 32
    };

    // Constructors
    AudioFormat();
    AudioFormat(int sampleRate, int channels, BitDepth bitDepth);
    AudioFormat(int sampleRate, int channels, int bitsPerSample, bool isFloat = false);

    // Properties
    int sampleRate;
    int channels;
    int bitsPerSample;
    bool isFloat;

    // Utility
    int getBytesPerSample() const;
    int getFrameSize() const;  // Bytes per frame
    bool isValid() const;

    // Comparison
    bool operator==(const AudioFormat& other) const;
    bool operator!=(const AudioFormat& other) const;

    // Static methods
    static AudioFormat getDefault();
    static AudioFormat getCDQuality();
    static AudioFormat getHighQuality();
};

// Stream operator for logging
std::ostream& operator<<(std::ostream& os, const AudioFormat& format);
```

## 3. Audio Processing APIs

### 3.1 SampleRateConverter

Handles sample rate conversion with multiple quality modes.

```cpp
class SampleRateConverter {
public:
    // Quality modes
    enum Quality {
        FAST,       // Linear interpolation
        GOOD,       // Cubic interpolation
        BEST,       // Sinc interpolation
        HIGH        // High-quality with pre-computed tables
    };

    // Constructor
    SampleRateConverter(Quality quality = GOOD);

    // Conversion
    bool convert(const AudioBuffer& input, AudioBuffer& output,
                 double inputRate, double outputRate);

    // In-place conversion
    bool convertInPlace(AudioBuffer& buffer, double targetRate);

    // Utility
    static bool isSupported(double sampleRate);
    static std::vector<double> getSupportedSampleRates();
};
```

#### Example

```cpp
// Convert 44.1kHz to 48kHz
AudioBuffer input(2, 44100);  // Input buffer
AudioBuffer output(2, 48000); // Output buffer

SampleRateConverter src(SampleRateConverter::BEST);
if (src.convert(input, output, 44100.0, 48000.0)) {
    std::cout << "Conversion successful" << std::endl;
}
```

### 3.2 WAVParser

Parses WAV file headers and loads audio data.

```cpp
class WAVParser {
public:
    // WAV header structure
    struct Header {
        char riff[4];           // "RIFF"
        uint32_t fileSize;      // File size - 8
        char wave[4];           // "WAVE"
        char fmt[4];            // "fmt "
        uint32_t fmtSize;       // Format chunk size
        uint16_t audioFormat;   // Audio format (1 = PCM)
        uint16_t channels;      // Number of channels
        uint32_t sampleRate;    // Sample rate
        uint32_t byteRate;      // Byte rate
        uint16_t blockAlign;    // Block align
        uint16_t bitsPerSample; // Bits per sample
        char data[4];           // "data"
        uint32_t dataSize;      // Data size
    };

    // Static methods
    static bool parseFile(const std::string& filename, Header& header);
    static bool loadFile(const std::string& filename, AudioBuffer& buffer);
    static bool isSupported(const std::string& filename);
    static AudioFormat getFormat(const std::string& filename);
};
```

#### Example

```cpp
// Check if file is a valid WAV
if (WAVParser::isSupported("music.wav")) {
    // Get format information
    WAVParser::Header header;
    if (WAVParser::parseFile("music.wav", header)) {
        std::cout << "Sample rate: " << header.sampleRate << std::endl;
        std::cout << "Channels: " << header.channels << std::endl;
    }

    // Load audio data
    AudioBuffer buffer;
    if (WAVParser::loadFile("music.wav", buffer)) {
        std::cout << "Loaded " << buffer.getFrames() << " frames" << std::endl;
    }
}
```

## 4. Plugin System APIs

### 4.1 PluginManager

Manages loading and configuration of Foobar2000-compatible plugins.

```cpp
class PluginManager {
public:
    // Plugin parameter value
    using PluginParamValue = std::variant<bool, int, float, std::string>;

    // Constructor
    PluginManager();
    ~PluginManager();

    // Initialization
    bool initialize(const std::string& configPath = "");
    void shutdown();

    // Plugin management
    bool loadPlugin(const std::string& pluginPath);
    void unloadPlugin(const std::string& pluginName);
    bool isPluginLoaded(const std::string& pluginName) const;

    // Plugin information
    std::vector<std::string> getLoadedPlugins() const;
    std::string getPluginInfo(const std::string& pluginName) const;

    // Plugin parameters
    void setPluginParameter(const std::string& pluginName,
                           const std::string& paramName,
                           const PluginParamValue& value);

    PluginParamValue getPluginParameter(const std::string& pluginName,
                                       const std::string& paramName,
                                       const PluginParamValue& defaultValue = {}) const;

    // Plugin enable/disable
    void setPluginEnabled(const std::string& pluginName, bool enabled);
    bool isPluginEnabled(const std::string& pluginName) const;

    // Audio processing
    bool processAudio(AudioBuffer& buffer, const std::string& pluginName = "");

    // Configuration
    bool saveConfiguration();
    bool loadConfiguration();

private:
    // Implementation details...
};
```

#### Example

```cpp
// Initialize plugin manager
PluginManager pluginMgr;
if (!pluginMgr.initialize()) {
    std::cerr << "Failed to initialize plugin manager" << std::endl;
}

// Load a plugin
if (pluginMgr.loadPlugin("plugins/foo_input_mp3.dll")) {
    // Configure plugin
    pluginMgr.setPluginParameter("foo_input_mp3", "quality", 5);
    pluginMgr.setPluginEnabled("foo_input_mp3", true);
}

// Save configuration
pluginMgr.saveConfiguration();
```

## 5. Configuration APIs

### 5.1 ConfigManager

Manages application configuration with support for multiple data types.

```cpp
class ConfigManager {
public:
    // Constructor
    ConfigManager(const std::string& filename = "config.ini");

    // File operations
    bool load();
    bool save();
    bool reload();

    // Get methods with default values
    std::string getString(const std::string& key,
                         const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    double getDouble(const std::string& key, double defaultValue = 0.0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;

    // Set methods
    void setString(const std::string& key, const std::string& value);
    void setInt(const std::string& key, int value);
    void setDouble(const std::string& key, double value);
    void setBool(const std::string& key, bool value);

    // Array/Map support
    std::vector<std::string> getArray(const std::string& key) const;
    void setArray(const std::string& key, const std::vector<std::string>& values);

    // Section operations
    bool hasSection(const std::string& section) const;
    std::vector<std::string> getSections() const;
    std::vector<std::string> getKeys(const std::string& section = "") const;

    // Remove operations
    void removeKey(const std::string& key);
    void removeSection(const std::string& section);
    void clear();

    // Utility
    bool isModified() const;
    std::string getFilename() const;
};
```

#### Example

```cpp
// Create config manager
ConfigManager config("player_config.ini");

// Load existing configuration
config.load();

// Get configuration values
int volume = config.getInt("audio.volume", 80);
std::string device = config.getString("audio.device", "default");
bool autoPlay = config.getBool("playback.auto_play", false);

// Set configuration values
config.setInt("audio.volume", 90);
config.setString("audio.device", "headphones");
config.setBool("playback.auto_play", true);

// Save configuration
config.save();
```

## 6. Platform APIs

### 6.1 AudioOutput

Platform-agnostic audio output interface.

```cpp
class AudioOutput {
public:
    // Platform-specific creation
    static std::unique_ptr<AudioOutput> create();

    virtual ~AudioOutput() = default;

    // Initialization
    virtual bool initialize(const AudioFormat& format) = 0;
    virtual void shutdown() = 0;
    virtual bool isInitialized() const = 0;

    // Playback control
    virtual bool start() = 0;
    virtual bool stop() = 0;
    virtual bool pause() = 0;
    virtual bool resume() = 0;

    // Audio output
    virtual bool write(const float* buffer, size_t frames) = 0;
    virtual size_t getBufferSpace() const = 0;

    // Device information
    virtual int getLatency() const = 0;
    virtual double getSampleRate() const = 0;
    virtual int getChannels() const = 0;

    // Device enumeration
    static std::vector<DeviceInfo> getAvailableDevices();
    static bool setDefaultDevice(const std::string& deviceId);

protected:
    struct DeviceInfo {
        std::string id;
        std::string name;
        std::string driver;
        int minChannels;
        int maxChannels;
        double minSampleRate;
        double maxSampleRate;
    };
};
```

#### Example

```cpp
// Create platform-specific audio output
auto audio = AudioOutput::create();
if (!audio) {
    std::cerr << "Failed to create audio output" << std::endl;
    return 1;
}

// Initialize with desired format
AudioFormat format(44100, 2, AudioFormat::FLOAT32);
if (!audio->initialize(format)) {
    std::cerr << "Failed to initialize audio" << std::endl;
    return 1;
}

// Start playback
if (audio->start()) {
    // Write audio data
    std::vector<float> buffer(1024);
    // ... fill buffer with audio data ...
    audio->write(buffer.data(), buffer.size() / 2);
}
```

## 7. Utility APIs

### 7.1 Logger

Simple logging utility.

```cpp
enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL
};

class Logger {
public:
    static void setLevel(LogLevel level);
    static void setFile(const std::string& filename);

    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);
    static void fatal(const std::string& message);

    template<typename... Args>
    static void debugf(const std::string& format, Args... args);

    template<typename... Args>
    static void infof(const std::string& format, Args... args);

    template<typename... Args>
    static void warningf(const std::string& format, Args... args);

    template<typename... Args>
    static void errorf(const std::string& format, Args... args);

    template<typename... Args>
    static void fatalf(const std::string& format, Args... args);
};

// Convenience macros
#define LOG_DEBUG(msg) Logger::debug(msg)
#define LOG_INFO(msg) Logger::info(msg)
#define LOG_WARNING(msg) Logger::warning(msg)
#define LOG_ERROR(msg) Logger::error(msg)
#define LOG_FATAL(msg) Logger::fatal(msg)
```

### 7.2 ThreadPool

Simple thread pool for parallel processing.

```cpp
class ThreadPool {
public:
    ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
    ~ThreadPool();

    // Submit tasks
    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))>;

    // Wait for all tasks
    void waitForAll();

    // Status
    size_t getThreadCount() const;
    size_t getActiveThreadCount() const;
    size_t getQueuedTaskCount() const;
};
```

## 8. Examples

### Example 1: Basic WAV Player

```cpp
#include "core/music_player.h"
#include "audio/wav_parser.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <wav_file>" << std::endl;
        return 1;
    }

    MusicPlayer player;

    // Initialize player
    if (!player.initialize()) {
        std::cerr << "Failed to initialize player" << std::endl;
        return 1;
    }

    // Load WAV file
    if (!player.loadFile(argv[1])) {
        std::cerr << "Failed to load file: " << argv[1] << std::endl;
        return 1;
    }

    // Set volume
    player.setVolume(0.8f);

    // Play
    player.play();

    // Print status
    std::cout << "Playing: " << argv[1] << std::endl;
    std::cout << "Duration: " << player.getDuration() << " seconds" << std::endl;

    // Wait for playback to complete
    while (player.getState() == MusicPlayer::PLAYING) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        // Show progress
        double pos = player.getPosition();
        double dur = player.getDuration();
        std::cout << "\rProgress: " << pos << " / " << dur
                  << " (" << (100.0 * pos / dur) << "%)" << std::flush;
    }

    std::cout << "\nPlayback complete" << std::endl;

    return 0;
}
```

### Example 2: Audio Processing with Plugins

```cpp
#include "core/foobar_plugin_manager.h"
#include "audio/audio_buffer.h"
#include "audio/sample_rate_converter.h"

int main() {
    // Initialize plugin manager
    PluginManager pluginMgr;
    if (!pluginMgr.initialize("plugins.ini")) {
        std::cerr << "Failed to initialize plugin manager" << std::endl;
        return 1;
    }

    // Load plugins
    pluginMgr.loadPlugin("plugins/foo_dsp_eq.dll");

    // Configure equalizer
    pluginMgr.setPluginParameter("foo_dsp_eq", "enabled", true);
    pluginMgr.setPluginParameter("foo_dsp_eq", "preamp", 0.0);
    pluginMgr.setPluginParameter("foo_dsp_eq", "band1", 2.0);  // 60Hz
    pluginMgr.setPluginParameter("foo_dsp_eq", "band2", 1.5);  // 230Hz

    // Create audio buffer (1 second at 44.1kHz)
    AudioBuffer buffer(2, 44100);

    // Generate test signal (sine wave)
    for (size_t i = 0; i < buffer.getFrames(); ++i) {
        float sample = 0.5f * sinf(2.0f * M_PI * 440.0f * i / 44100.0f);
        buffer.setSample(0, i, sample);
        buffer.setSample(1, i, sample);
    }

    // Process with equalizer
    if (pluginMgr.processAudio(buffer)) {
        std::cout << "Audio processed successfully" << std::endl;

        // Save processed audio
        WAVParser::saveToFile("output.wav", buffer);
    }

    return 0;
}
```

### Example 3: Real-time Audio Processing

```cpp
#include "core/music_player.h"
#include "audio/sample_rate_converter.h"

class RealtimeProcessor {
private:
    MusicPlayer player_;
    SampleRateConverter src_;
    AudioBuffer inputBuffer_;
    AudioBuffer outputBuffer_;
    std::thread processingThread_;
    std::atomic<bool> running_;

public:
    RealtimeProcessor() : running_(false) {}

    ~RealtimeProcessor() {
        stop();
    }

    bool start(const std::string& filename) {
        // Initialize player
        if (!player_.initialize()) {
            return false;
        }

        // Load file
        if (!player_.loadFile(filename)) {
            return false;
        }

        // Setup buffers
        AudioFormat format = player_.getFormat();
        inputBuffer_.resize(format.channels, 1024);
        outputBuffer_.resize(format.channels, 1024);

        // Initialize SRC for 48kHz output
        src_.initialize(SampleRateConverter::GOOD);

        // Start processing thread
        running_ = true;
        processingThread_ = std::thread(&RealtimeProcessor::processLoop, this);

        return player_.play();
    }

    void stop() {
        running_ = false;
        if (processingThread_.joinable()) {
            processingThread_.join();
        }
        player_.stop();
    }

private:
    void processLoop() {
        while (running_) {
            // Get audio data from player
            if (player_.getState() == MusicPlayer::PLAYING) {
                // Read audio data (simplified)
                // ...

                // Apply sample rate conversion
                src_.convert(inputBuffer_, outputBuffer_,
                           player_.getFormat().sampleRate, 48000.0);

                // Send to output device
                // ...
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
};
```

### Example 4: Configuration Management

```cpp
#include "core/config_manager.h"

class PlayerSettings {
private:
    ConfigManager config_;

public:
    PlayerSettings(const std::string& filename) : config_(filename) {}

    bool load() {
        return config_.load();
    }

    bool save() {
        return config_.save();
    }

    // Audio settings
    int getVolume() const {
        return config_.getInt("audio.volume", 80);
    }

    void setVolume(int volume) {
        config_.setInt("audio.volume", std::max(0, std::min(100, volume)));
    }

    std::string getDevice() const {
        return config_.getString("audio.device", "default");
    }

    void setDevice(const std::string& device) {
        config_.setString("audio.device", device);
    }

    // Playback settings
    bool getAutoPlay() const {
        return config_.getBool("playback.auto_play", false);
    }

    void setAutoPlay(bool autoPlay) {
        config_.setBool("playback.auto_play", autoPlay);
    }

    // Recent files
    std::vector<std::string> getRecentFiles() const {
        return config_.getArray("ui.recent_files");
    }

    void addRecentFile(const std::string& filename) {
        auto recent = getRecentFiles();

        // Remove if already exists
        recent.erase(std::remove(recent.begin(), recent.end(), filename),
                    recent.end());

        // Add to front
        recent.insert(recent.begin(), filename);

        // Keep only last 10
        if (recent.size() > 10) {
            recent.resize(10);
        }

        config_.setArray("ui.recent_files", recent);
    }
};

// Usage
int main() {
    PlayerSettings settings("player_config.ini");

    // Load settings
    settings.load();

    // Use settings
    std::cout << "Volume: " << settings.getVolume() << std::endl;
    std::cout << "Device: " << settings.getDevice() << std::endl;

    // Add recent file
    settings.addRecentFile("music.mp3");

    // Save settings
    settings.save();

    return 0;
}
```

## Best Practices

1. **Error Handling**: Always check return values from API calls
2. **Resource Management**: Use RAII patterns, let destructors clean up
3. **Thread Safety**: Use mutexes when sharing data between threads
4. **Performance**: Pre-allocate buffers, avoid frequent allocations
5. **Plugin Safety**: Validate plugins before loading, handle exceptions

## Thread Safety

- Most classes are **not thread-safe** by default
- Use external synchronization for shared access
- Audio processing callbacks should not block
- Use thread-safe variants when available (e.g., `AudioBuffer` copy operations)

## Version Information

Current API version: 1.0.0

Compatibility:
- C++17 or later
- Windows 7+ / Linux kernel 3.10+
- Requires audio drivers (WASAPI/ALSA)

For more information and updates, visit:
https://github.com/checkyyeug/coreMusicPlayer