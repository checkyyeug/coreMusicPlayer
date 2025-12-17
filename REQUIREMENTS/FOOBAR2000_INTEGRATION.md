# coreMusicPlayer Foobar2000 SDK Integration Documentation

## 1. Overview

coreMusicPlayer integrates with the Foobar2000 SDK to provide compatibility with existing Foobar2000 components, decoders, and DSP plugins. This integration allows users to leverage the extensive ecosystem of Foobar2000 plugins while maintaining coreMusicPlayer's native capabilities.

## 2. SDK Architecture

### 2.1 Component Structure

```
Foobar2000 SDK Integration
├── SDK Compatibility Layer
│   ├── Input Plugin Interface
│   ├── Output Plugin Interface
│   ├── DSP Plugin Interface
│   └── Visualization Plugin Interface
├── Component Manager
│   ├── Plugin Loader
│   ├── Service Registry
│   └── Lifecycle Manager
├── API Adapters
│   ├── AudioInputAdapter
│   ├── AudioOutputAdapter
│   ├── MetadataAdapter
│   └── ConfigurationAdapter
└── Bridge Components
    ├── Component Bridge
    ├── Event Bridge
    └── Configuration Bridge
```

### 2.2 Integration Points

1. **Input Plugins**: Access to additional audio format decoders
2. **DSP Plugins**: Real-time audio processing effects
3. **Visualization Plugins**: Audio visualization components
4. **Metadata Plugins**: Extended metadata reading/writing
5. **Output Plugins**: Alternative audio output methods

## 3. SDK Setup

### 3.1 Directory Structure

```
foobar2000_sdk/
├── SDK/
│   ├── audio_stream_decoder.h
│   ├── audio_stream_encoder.h
│   ├── dsp.h
│   ├── file_info.h
│   ├── input.h
│   ├── metadb.h
│   ├── output.h
│   ├── playlist_loader.h
│   └── component_client.h
├── pfc/
│   ├── string.h
│   ├── array.h
│   ├── list.h
│   ├── other.h
│   └── templates.h
└── libPPUI/
    ├── CListCtrl.h
    ├── CWindow.h
    └── other UI components
```

### 3.2 CMake Configuration

```cmake
# CMakeLists.txt - Foobar2000 SDK Integration
set(FOOBAR2000_SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/foobar2000_sdk")

# Include paths
include_directories(
    ${FOOBAR2000_SDK_ROOT}
    ${FOOBAR2000_SDK_ROOT}/SDK
    ${FOOBAR2000_SDK_ROOT}/pfc
)

# Source files
set(FOOBAR_SOURCES
    src/foobar_plugin_manager.cpp
    src/foobar/foobar_input_adapter.cpp
    src/foobar/foobar_output_adapter.cpp
    src/foobar/foobar_dsp_adapter.cpp
    src/foobar/foobar_metadata_adapter.cpp
    src/foobar/foobar_service_registry.cpp
    src/foobar/foobar_component_bridge.cpp
)

# Create Foobar2000 integration library
add_library(xpumusic_foobar STATIC ${FOOBAR_SOURCES})

# Link with core audio engine
target_link_libraries(xpumusic_foobar xpumusic_core)

# Windows-specific linking
if(WIN32)
    target_link_libraries(xpumusic_foobar
        ole32
        uuid
        kernel32
        user32
    )
endif()

# Enable Foobar2000 support if SDK is found
option(ENABLE_FOOBAR2000 "Enable Foobar2000 plugin support" ON)
if(ENABLE_FOOBAR2000 AND EXISTS ${FOOBAR2000_SDK_ROOT})
    target_compile_definitions(xpumusic_core PRIVATE WITH_FOOBAR2000)
endif()
```

## 4. Component Manager

### 4.1 Plugin Manager Interface

```cpp
// foobar_plugin_manager.h
class FoobarPluginManager {
public:
    static FoobarPluginManager& instance();

    // Plugin lifecycle
    bool initialize(const std::string& pluginPath);
    void shutdown();

    // Plugin discovery
    std::vector<std::string> discoverPlugins(const std::string& path);
    bool loadPlugin(const std::string& filename);
    void unloadPlugin(const std::string& filename);
    void reloadAllPlugins();

    // Component access
    std::shared_ptr<input_decoder> getInputDecoder(const std::string& extension);
    std::shared_ptr<dsp_chain_entry> getDSPProcessor(const std::string& name);
    std::shared_ptr<output> getOutputPlugin(const std::string& name);

    // Configuration
    void setPluginPath(const std::string& path);
    std::string getPluginPath() const;

    // Events
    using PluginEventCallback = std::function<void(const std::string&)>;
    void onPluginLoaded(PluginEventCallback callback);
    void onPluginUnloaded(PluginEventCallback callback);

private:
    FoobarPluginManager() = default;
    void scanPlugins();
    void validatePlugin(const std::string& path);

    std::map<std::string, HMODULE> loadedPlugins_;
    std::map<std::string, std::shared_ptr<void>> components_;
    std::string pluginPath_;
    std::vector<PluginEventCallback> loadedCallbacks_;
    std::vector<PluginEventCallback> unloadedCallbacks_;
};
```

### 4.2 Plugin Manager Implementation

```cpp
// foobar_plugin_manager.cpp
FoobarPluginManager& FoobarPluginManager::instance() {
    static FoobarPluginManager instance;
    return instance;
}

bool FoobarPluginManager::initialize(const std::string& pluginPath) {
    pluginPath_ = pluginPath;

    // Initialize Foobar2000 core services
    if (!initializeCoreServices()) {
        return false;
    }

    // Scan for plugins
    scanPlugins();

    return true;
}

std::vector<std::string> FoobarPluginManager::discoverPlugins(const std::string& path) {
    std::vector<std::string> plugins;

    // Search for DLL files
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA((path + "\\*.dll").c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string filename = findData.cFileName;

            // Check if it's a valid Foobar2000 plugin
            if (isValidPlugin(path + "\\" + filename)) {
                plugins.push_back(filename);
            }
        } while (FindNextFileA(hFind, &findData));

        FindClose(hFind);
    }

    return plugins;
}

bool FoobarPluginManager::loadPlugin(const std::string& filename) {
    std::string fullPath = pluginPath_ + "\\" + filename;

    // Load the DLL
    HMODULE hModule = LoadLibraryA(fullPath.c_str());
    if (!hModule) {
        return false;
    }

    // Get the plugin entry point
    typedef foobar2000_component_api* (*get_component_proc_t)();
    get_component_proc_t getComponent =
        reinterpret_cast<get_component_proc_t>(GetProcAddress(hModule, "get_component"));

    if (!getComponent) {
        FreeLibrary(hModule);
        return false;
    }

    // Initialize the component
    foobar2000_component_api* component = getComponent();
    if (!component) {
        FreeLibrary(hModule);
        return false;
    }

    // Register the component
    component->component_services->register_init_callback([](init_callback_t callback) {
        callback();
    });

    // Store the plugin
    loadedPlugins_[filename] = hModule;

    // Notify callbacks
    for (auto& callback : loadedCallbacks_) {
        callback(filename);
    }

    return true;
}

void FoobarPluginManager::scanPlugins() {
    auto plugins = discoverPlugins(pluginPath_);

    for (const auto& plugin : plugins) {
        loadPlugin(plugin);
    }
}
```

## 5. Input Plugin Integration

### 5.1 Input Decoder Adapter

```cpp
// foobar_input_adapter.h
class FoobarInputAdapter : public AudioDecoder {
public:
    FoobarInputAdapter(std::shared_ptr<input_decoder> decoder);
    ~FoobarInputAdapter() override;

    // AudioDecoder interface
    bool canDecode(const std::string& filename) override;
    bool open(const std::string& filename) override;
    void close() override;

    // Audio information
    int getSampleRate() const override;
    int getChannels() const override;
    int getBitDepth() const override;
    size_t getFrames() const override;

    // Decoding
    size_t decode(float* buffer, size_t frames) override;
    bool seek(size_t frame) override;

    // Metadata
    std::map<std::string, std::string> getMetadata() const override;

private:
    void initializeDecoder();
    file_info_impl createFileInfo();

    std::shared_ptr<input_decoder> decoder_;
    std::string currentFile_;
    audio_chunk::ptr chunk_;
    file_info_impl fileInfo_;
    size_t currentFrame_;
    bool initialized_;
};
```

### 5.2 Input Adapter Implementation

```cpp
// foobar_input_adapter.cpp
FoobarInputAdapter::FoobarInputAdapter(std::shared_ptr<input_decoder> decoder)
    : decoder_(decoder)
    , currentFrame_(0)
    , initialized_(false) {
}

bool FoobarInputAdapter::open(const std::string& filename) {
    currentFile_ = filename;
    currentFrame_ = 0;
    initialized_ = false;

    try {
        // Initialize the decoder for this file
        service_ptr_t<file> fileHandle;
        filesystem::g_open_read(fileHandle, filename.c_str(), filesystem::open_mode_read);

        // Get file info
        decoder_->open(fileHandle, fileInfo_, abort_callback_dummy());

        // Initialize decoding
        initializeDecoder();

        initialized_ = true;
        return true;

    } catch (const std::exception& e) {
        return false;
    }
}

size_t FoobarInputAdapter::decode(float* buffer, size_t frames) {
    if (!initialized_ || !decoder_) {
        return 0;
    }

    size_t framesDecoded = 0;

    try {
        while (framesDecoded < frames) {
            // Get next chunk if needed
            if (!chunk_ || chunk_->get_sample_count() == 0) {
                decoder_->decode(chunk_, abort_callback_dummy());

                if (!chunk_ || chunk_->get_sample_count() == 0) {
                    break; // End of file
                }
            }

            // Convert chunk data to float buffer
            const audio_sample* source = chunk_->get_data();
            size_t chunkFrames = chunk_->get_sample_count();
            size_t framesToCopy = std::min(chunkFrames, frames - framesDecoded);

            // Interleave channels
            for (size_t i = 0; i < framesToCopy; ++i) {
                for (int c = 0; c < chunk_->get_channel_count(); ++c) {
                    buffer[(framesDecoded + i) * chunk_->get_channel_count() + c] =
                        static_cast<float>(source[i * chunk_->get_channel_count() + c]);
                }
            }

            framesDecoded += framesToCopy;
            currentFrame_ += framesToCopy;

            // Update chunk
            if (framesToCopy < chunkFrames) {
                // Partial copy - create new chunk with remaining data
                audio_chunk::ptr newChunk = service_ptr_t<audio_chunk>();
                newChunk->set_data(
                    source + framesToCopy * chunk_->get_channel_count(),
                    chunkFrames - framesToCopy,
                    chunk_->get_sample_rate(),
                    chunk_->get_channels(),
                    chunk_->get_channel_config()
                );
                chunk_ = newChunk;
            } else {
                chunk_.release();
            }
        }

    } catch (const std::exception& e) {
        return framesDecoded;
    }

    return framesDecoded;
}

bool FoobarInputAdapter::seek(size_t frame) {
    if (!initialized_ || !decoder_) {
        return false;
    }

    try {
        decoder_->seek(frame, abort_callback_dummy());
        currentFrame_ = frame;
        chunk_.release();
        return true;

    } catch (const std::exception& e) {
        return false;
    }
}

std::map<std::string, std::string> FoobarInputAdapter::getMetadata() const {
    std::map<std::string, std::string> metadata;

    if (initialized_) {
        // Extract metadata from file info
        const char* keys[] = {
            "title", "artist", "album", "date", "genre",
            "tracknumber", "discnumber", "comment", nullptr
        };

        for (int i = 0; keys[i]; ++i) {
            const char* value = fileInfo_.info().info_get(keys[i]);
            if (value) {
                metadata[keys[i]] = value;
            }
        }

        // Add technical info
        metadata["format"] = fileInfo_.info().info_get("codec");
        metadata["bitrate"] = std::to_string(fileInfo_.info().info_get_int("bitrate"));
        metadata["duration"] = std::to_string(fileInfo_.get_length());
    }

    return metadata;
}
```

## 6. DSP Plugin Integration

### 6.1 DSP Chain Manager

```cpp
// foobar_dsp_manager.h
class FoobarDSPManager {
public:
    static FoobarDSPManager& instance();

    // DSP chain management
    bool loadDSPChain(const std::string& filename);
    bool saveDSPChain(const std::string& filename);

    // DSP processing
    void processAudio(float* buffer, size_t frames, int channels, int sampleRate);

    // Configuration
    void addDSP(const std::string& name);
    void removeDSP(const std::string& name);
    void moveDSP(size_t fromIndex, size_t toIndex);

    // Query
    std::vector<std::string> getAvailableDSPs() const;
    std::vector<std::string> getActiveDSPs() const;

private:
    FoobarDSPManager() = default;

    dsp_chain_manager_impl dspManager_;
    std::vector<dsp_entry_ptr> activeDSPs_;
};
```

### 6.2 DSP Processing Implementation

```cpp
// foobar_dsp_manager.cpp
void FoobarDSPManager::processAudio(float* buffer, size_t frames,
                                   int channels, int sampleRate) {
    if (activeDSPs_.empty()) {
        return;
    }

    // Create audio chunk from buffer
    audio_chunk::ptr chunk = service_ptr_t<audio_chunk>();
    chunk->set_data(buffer, frames, sampleRate, channels,
                    audio_chunk::channel_config_from_channels(channels));

    // Process through DSP chain
    for (auto& dsp : activeDSPs_) {
        dsp_entry_v2::ptr dspV2;
        if (dsp->service_query_t(dspV2)) {
            dspV2->run(chunk, abort_callback_dummy());
        } else {
            dsp->run(chunk, abort_callback_dummy());
        }
    }

    // Copy processed data back to buffer
    const audio_sample* processedData = chunk->get_data();
    memcpy(buffer, processedData, frames * channels * sizeof(float));
}

bool FoobarDSPManager::loadDSPChain(const std::string& filename) {
    try {
        // Clear existing chain
        activeDSPs_.clear();
        dspManager_.remove_all();

        // Load from file
        string8 path = filename.c_str();
        dspManager_.get_chain().from_file(path);

        // Update active DSPs list
        for (size_t i = 0; i < dspManager_.get_chain().get_count(); ++i) {
            dsp_entry_config::ptr config;
            if (dspManager_.get_chain().get_item(i)->query(config)) {
                activeDSPs_.push_back(config->get_ptr());
            }
        }

        return true;

    } catch (const std::exception& e) {
        return false;
    }
}
```

## 7. Output Plugin Integration

### 7.1 Output Manager

```cpp
// foobar_output_manager.h
class FoobarOutputAdapter : public AudioOutput {
public:
    FoobarOutputAdapter(std::shared_ptr<output> output);
    ~FoobarOutputAdapter() override;

    // AudioOutput interface
    bool initialize(const AudioFormat& format) override;
    void cleanup() override;
    bool start() override;
    bool stop() override;
    bool pause() override;

    // Configuration
    size_t getBufferSize() const override;
    void setBufferSize(size_t frames) override;
    double getLatency() const override;

private:
    void updateFormat();
    audio_chunk::ptr createChunk(const float* buffer, size_t frames);

    std::shared_ptr<output> output_;
    output_entry_ptr outputEntry_;
    AudioFormat currentFormat_;
    std::atomic<State> state_{State::Stopped};
};
```

### 7.2 Output Implementation

```cpp
// foobar_output_adapter.cpp
bool FoobarOutputAdapter::initialize(const AudioFormat& format) {
    currentFormat_ = format;

    try {
        // Get output entry
        if (!output_->service_query_get(outputEntry_)) {
            return false;
        }

        // Configure output format
        output_config config;
        config.sample_rate = format.sampleRate;
        config.bits_per_sample = format.bitDepth;
        config.channels = format.channels;
        config.channel_mask = audio_chunk::channel_config_from_channels(format.channels);

        // Initialize output
        outputEntry_->open(config, abort_callback_dummy());

        return true;

    } catch (const std::exception& e) {
        return false;
    }
}

bool FoobarOutputAdapter::start() {
    try {
        outputEntry_->write_samples_playback_start();
        state_ = State::Playing;
        return true;

    } catch (const std::exception& e) {
        return false;
    }
}

void FoobarOutputAdapter::writeAudio(const float* buffer, size_t frames) {
    if (state_ != State::Playing || !outputEntry_) {
        return;
    }

    try {
        // Create audio chunk
        audio_chunk::ptr chunk = createChunk(buffer, frames);

        // Write to output
        outputEntry_->write_samples(chunk.get(), abort_callback_dummy());

    } catch (const std::exception& e) {
        // Handle error
    }
}

audio_chunk::ptr FoobarOutputAdapter::createChunk(const float* buffer, size_t frames) {
    audio_chunk::ptr chunk = service_ptr_t<audio_chunk>();

    // Convert float to audio_sample (double)
    std::vector<audio_sample> samples(frames * currentFormat_.channels);
    for (size_t i = 0; i < samples.size(); ++i) {
        samples[i] = static_cast<audio_sample>(buffer[i]);
    }

    // Set chunk data
    chunk->set_data(samples.data(), frames,
                    currentFormat_.sampleRate,
                    currentFormat_.channels,
                    audio_chunk::channel_config_from_channels(currentFormat_.channels));

    return chunk;
}
```

## 8. Configuration Bridge

### 8.1 Configuration Manager

```cpp
// foobar_config_manager.h
class FoobarConfigManager {
public:
    static FoobarConfigManager& instance();

    // Core configuration
    void loadConfiguration();
    void saveConfiguration();

    // Component settings
    template<typename T>
    T getComponentSetting(const std::string& component,
                         const std::string& key,
                         const T& defaultValue = T());

    template<typename T>
    void setComponentSetting(const std::string& component,
                            const std::string& key,
                            const T& value);

    // Bridge to coreMusicPlayer settings
    void syncWithXpuMusicSettings();
    void exportToXpuMusicSettings();

private:
    FoobarConfigManager() = default;

    cfg_var_t* getConfigVar(const std::string& path);
    void setConfigVar(const std::string& path, const std::string& value);

    config_object::ptr configObject_;
    std::map<std::string, cfg_var_t*> configCache_;
};
```

### 8.2 Configuration Implementation

```cpp
// foobar_config_manager.cpp
template<typename T>
T FoobarConfigManager::getComponentSetting(const std::string& component,
                                          const std::string& key,
                                          const T& defaultValue) {
    std::string path = component + "." + key;

    auto it = configCache_.find(path);
    if (it != configCache_.end()) {
        // Convert from cfg_var_t to T
        if constexpr (std::is_same_v<T, int>) {
            return static_cast<cfg_int_t*>(it->second)->get_value();
        } else if constexpr (std::is_same_v<T, bool>) {
            return static_cast<cfg_bool_t*>(it->second)->get_value();
        } else if constexpr (std::is_same_v<T, std::string>) {
            return static_cast<cfg_string_t*>(it->second)->get_value().c_str();
        }
    }

    return defaultValue;
}

void FoobarConfigManager::syncWithXpuMusicSettings() {
    // Sync audio settings
    auto& audioSettings = SettingsModel::instance().audioSettings();

    setComponentSetting("output", "device", audioSettings.outputDevice.toStdString());
    setComponentSetting("output", "sample_rate", audioSettings.sampleRate);
    setComponentSetting("output", "buffer_length", audioSettings.bufferSize);

    // Sync DSP settings
    auto& playbackSettings = SettingsModel::instance().playbackSettings();

    setComponentSetting("dsp", "replaygain_mode", playbackSettings.replaygainMode);
    setComponentSetting("dsp", "replaygain_preamp", playbackSettings.replaygainPreamp);
}
```

## 9. Error Handling and Compatibility

### 9.1 Compatibility Checker

```cpp
// foobar_compatibility.h
class FoobarCompatibilityChecker {
public:
    struct PluginInfo {
        std::string name;
        std::string version;
        std::string author;
        bool isCompatible;
        std::vector<std::string> requirements;
    };

    static FoobarCompatibilityChecker& instance();

    // Plugin validation
    bool validatePlugin(const std::string& filename);
    std::vector<PluginInfo> analyzePlugins(const std::string& pluginPath);

    // Version compatibility
    bool isSDKVersionCompatible(const std::string& requiredVersion);
    bool isPluginCompatible(const std::string& pluginName,
                           const std::string& pluginVersion);

private:
    FoobarCompatibilityChecker() = default;

    std::string getSDKVersion();
    std::string getPluginVersion(const std::string& filename);
    bool checkDependencies(const PluginInfo& plugin);

    std::string sdkVersion_;
    std::map<std::string, std::string> compatibilityMatrix_;
};
```

### 9.2 Error Recovery

```cpp
// foobar_error_recovery.h
class FoobarErrorRecovery {
public:
    enum class ErrorType {
        PluginLoadFailure,
        DSPChainCorruption,
        OutputDeviceFailure,
        ConfigurationError
    };

    static FoobarErrorRecovery& instance();

    // Error handling
    bool handleError(ErrorType type, const std::string& details);
    void attemptRecovery(ErrorType type);

    // Fallback mechanisms
    void loadFallbackPlugins();
    void resetToDefaultConfiguration();
    void initializeDefaultOutput();

private:
    void logError(ErrorType type, const std::string& details);
    bool canRecover(ErrorType type);

    std::map<ErrorType, std::function<bool()>> recoveryStrategies_;
};
```

## 10. Testing

### 10.1 Plugin Integration Tests

```cpp
class FoobarPluginTest : public QObject {
    Q_OBJECT

private slots:
    void testPluginLoading();
    void testInputDecoding();
    void testDSPProcessing();
    void testOutputIntegration();
    void testConfigurationSync();

private:
    void createTestPlugin(const std::string& name);
    void cleanupTestPlugins();
};

void FoobarPluginTest::testInputDecoding() {
    // Create test plugin
    createTestPlugin("test_input.dll");

    // Load plugin
    FoobarPluginManager& manager = FoobarPluginManager::instance();
    QVERIFY(manager.loadPlugin("test_input.dll"));

    // Get decoder
    auto decoder = manager.getInputDecoder(".mp3");
    QVERIFY(decoder != nullptr);

    // Test decoding
    FoobarInputAdapter adapter(decoder);
    QVERIFY(adapter.open("test.mp3"));

    float buffer[1024];
    size_t frames = adapter.decode(buffer, 256);
    QVERIFY(frames > 0);

    adapter.close();
    cleanupTestPlugins();
}

void FoobarPluginTest::testDSPProcessing() {
    FoobarDSPManager& dspManager = FoobarDSPManager::instance();

    // Add test DSP
    dspManager.addDSP("equalizer");

    // Process audio
    std::vector<float> testAudio(1024);
    std::generate(testAudio.begin(), testAudio.end(),
                  []() { return static_cast<float>(rand()) / RAND_MAX; });

    float* originalData = testAudio.data();
    dspManager.processAudio(originalData, 256, 2, 44100);

    // Verify processing occurred
    QVERIFY(std::equal(testAudio.begin(), testAudio.begin() + 512,
                      originalData) == false);
}
```

This Foobar2000 SDK integration documentation provides comprehensive guidance for implementing and maintaining the plugin compatibility layer, enabling coreMusicPlayer to leverage the extensive Foobar2000 ecosystem effectively.