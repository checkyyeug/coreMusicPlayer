# coreMusicPlayer Platform-Specific Implementation Details

## 1. Overview

coreMusicPlayer is designed as a cross-platform audio player with specialized implementations for each target platform. This document details the platform-specific code paths, optimizations, and considerations for Windows, Linux, and macOS.

## 2. Platform Architecture

### 2.1 Abstraction Layers

```
Platform Abstraction
├── Core Audio Engine (Platform Independent)
│   ├── Audio Buffer Management
│   ├── Audio Decoders
│   ├── Sample Rate Conversion
│   └── DSP Processing
├── Platform Interface Layer
│   ├── Audio Output Interface
│   ├── File System Interface
│   ├── Threading Interface
│   └── Configuration Interface
└── Platform Implementations
    ├── Windows (WASAPI, WinAPI)
    ├── Linux (ALSA/PulseAudio, POSIX)
    └── macOS (CoreAudio, Cocoa)
```

### 2.2 Platform Detection

```cpp
// platform_config.h
#pragma once

// Platform detection macros
#if defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
    #ifdef _WIN64
        #define PLATFORM_WINDOWS_64
    #else
        #define PLATFORM_WINDOWS_32
    #endif
#elif defined(__APPLE__)
    #define PLATFORM_MACOS
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC
        #define PLATFORM_MACOS_NATIVE
    #endif
#elif defined(__linux__)
    #define PLATFORM_LINUX
    #if defined(__ANDROID__)
        #define PLATFORM_ANDROID
    #endif
#endif

// Compiler detection
#if defined(_MSC_VER)
    #define COMPILER_MSVC
#elif defined(__GNUC__)
    #define COMPILER_GCC
    #define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#elif defined(__clang__)
    #define COMPILER_CLANG
#endif

// Architecture detection
#if defined(_M_X64) || defined(__x86_64__)
    #define ARCH_X64
#elif defined(_M_IX86) || defined(__i386__)
    #define ARCH_X86
#elif defined(_M_ARM64) || defined(__aarch64__)
    #define ARCH_ARM64
#endif

// Platform-specific includes
#ifdef PLATFORM_WINDOWS
    #include <windows.h>
    #include <mmsystem.h>
    #include <audioclient.h>
    #include <mmdeviceapi.h>
#elif defined(PLATFORM_LINUX)
    #include <alsa/asoundlib.h>
    #include <pthread.h>
    #include <unistd.h>
    #include <sys/types.h>
#elif defined(PLATFORM_MACOS)
    #include <CoreAudio/CoreAudio.h>
    #include <AudioToolbox/AudioToolbox.h>
    #include <Foundation/Foundation.h>
#endif
```

## 3. Windows Implementation

### 3.1 Audio Output - WASAPI

```cpp
// platform/windows/wasapi_output.h
class WASAPIOutput : public AudioOutput {
public:
    WASAPIOutput();
    ~WASAPIOutput() override;

    // AudioOutput interface
    bool initialize(const AudioFormat& format) override;
    void cleanup() override;
    bool start() override;
    bool stop() override;
    bool pause() override;

    // Windows-specific features
    bool initializeExclusive(const AudioFormat& format);
    bool enableEventDrivenMode();
    std::vector<std::string> enumerateDevices();

private:
    // WASAPI components
    IMMDeviceEnumerator* deviceEnumerator_;
    IMMDevice* device_;
    IAudioClient* audioClient_;
    IAudioRenderClient* renderClient_;
    IAudioClock* audioClock_;

    // Threading
    HANDLE audioThread_;
    HANDLE stopEvent_;
    HANDLE audioReadyEvent_;

    // Buffer management
    UINT32 bufferFrames_;
    bool eventDriven_;
    bool exclusiveMode_;

    // Helper methods
    HRESULT initializeAudioClient();
    HRESULT setupEventHandling();
    HRESULT configureBuffer();
    void audioThreadProc();
};
```

```cpp
// platform/windows/wasapi_output.cpp
bool WASAPIOutput::initialize(const AudioFormat& format) {
    HRESULT hr;

    // Initialize COM
    hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    if (FAILED(hr)) return false;

    // Create device enumerator
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr,
                         CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
                         reinterpret_cast<void**>(&deviceEnumerator_));
    if (FAILED(hr)) return false;

    // Get default audio endpoint
    hr = deviceEnumerator_->GetDefaultAudioEndpoint(eRender, eConsole, &device_);
    if (FAILED(hr)) return false;

    // Activate audio client
    hr = device_->Activate(__uuidof(IAudioClient), CLSCTX_ALL,
                          nullptr, reinterpret_cast<void**>(&audioClient_));
    if (FAILED(hr)) return false;

    // Configure audio format
    WAVEFORMATEX* waveFormat = nullptr;
    hr = audioClient_->GetMixFormat(&waveFormat);
    if (FAILED(hr)) return false;

    // Adjust format to match request
    waveFormat->wFormatTag = WAVE_FORMAT_EXTENSIBLE;
    waveFormat->nSamplesPerSec = format.sampleRate;
    waveFormat->nChannels = format.channels;
    waveFormat->wBitsPerSample = format.bitDepth;
    waveFormat->nBlockAlign = (waveFormat->nChannels * waveFormat->wBitsPerSample) / 8;
    waveFormat->nAvgBytesPerSec = waveFormat->nSamplesPerSec * waveFormat->nBlockAlign;

    // Initialize audio client
    hr = audioClient_->Initialize(
        AUDCLNT_SHAREMODE_SHARED,
        eventDriven_ ? AUDCLNT_STREAMFLAGS_EVENTCALLBACK : 0,
        0,  // Buffer duration
        0,  // Periodicity
        waveFormat,
        nullptr
    );

    if (FAILED(hr)) {
        CoTaskMemFree(waveFormat);
        return false;
    }

    // Get buffer size
    hr = audioClient_->GetBufferSize(&bufferFrames_);
    if (FAILED(hr)) return false;

    // Get render client
    hr = audioClient_->GetService(__uuidof(IAudioRenderClient),
                                 reinterpret_cast<void**>(&renderClient_));
    if (FAILED(hr)) return false;

    // Setup event handling if needed
    if (eventDriven_) {
        hr = setupEventHandling();
        if (FAILED(hr)) return false;
    }

    CoTaskMemFree(waveFormat);
    return true;
}

void WASAPIOutput::audioThreadProc() {
    HRESULT hr;
    HANDLE waitHandles[] = { stopEvent_, audioReadyEvent_ };

    while (true) {
        DWORD waitResult = WaitForMultipleObjects(2, waitHandles, FALSE, INFINITE);

        if (waitResult == WAIT_OBJECT_0) {
            // Stop event
            break;
        }

        if (waitResult == WAIT_OBJECT_0 + 1) {
            // Audio ready event
            UINT32 padding;
            hr = audioClient_->GetCurrentPadding(&padding);
            if (SUCCEEDED(hr)) {
                UINT32 framesAvailable = bufferFrames_ - padding;
                if (framesAvailable > 0) {
                    BYTE* data;
                    hr = renderClient_->GetBuffer(framesAvailable, &data);
                    if (SUCCEEDED(hr)) {
                        // Get audio data from callback
                        if (audioCallback_) {
                            float* audioData = audioCallback_(framesAvailable);
                            if (audioData) {
                                // Convert float to device format
                                memcpy(data, audioData,
                                      framesAvailable * currentFormat_.channels *
                                      sizeof(float));
                            }
                        }
                        renderClient_->ReleaseBuffer(framesAvailable, 0);
                    }
                }
            }
        }
    }
}
```

### 3.2 Windows Threading

```cpp
// platform/windows/threading.h
class WindowsThreadManager {
public:
    static WindowsThreadManager& instance();

    // Thread creation with priority
    HANDLE createThread(std::function<void()> func,
                       ThreadPriority priority = ThreadPriority::Normal);
    bool setThreadPriority(HANDLE thread, ThreadPriority priority);
    void setThreadAffinity(HANDLE thread, uint64_t mask);

    // Timer functions
    HANDLE createHighResolutionTimer();
    bool setTimerPeriod(HANDLE timer, int periodMs);

private:
    WindowsThreadManager() = default;

    std::vector<HANDLE> threads_;
    CRITICAL_SECTION threadListLock_;
};
```

### 3.3 Windows Configuration

```cpp
// platform/windows/config_manager.h
class WindowsConfigManager {
public:
    static WindowsConfigManager& instance();

    // Registry access
    bool getRegistryValue(const std::string& key,
                         const std::string& value,
                         std::string& result);
    bool setRegistryValue(const std::string& key,
                         const std::string& value,
                         const std::string& data);

    // Windows-specific settings
    bool getAutoStartEnabled();
    void setAutoStartEnabled(bool enabled);
    bool getFileAssociationEnabled();
    void setFileAssociationEnabled(bool enabled);

    // Integration
    bool registerFileAssociations();
    bool unregisterFileAssociations();
    bool registerShellExtensions();

private:
    WindowsConfigManager() = default;

    const std::string REGISTRY_KEY = "Software\\coreMusicPlayer";
    const std::string AUTO_START_KEY = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
};
```

## 4. Linux Implementation

### 4.1 Audio Output - ALSA

```cpp
// platform/linux/alsa_output.h
class ALSAOutput : public AudioOutput {
public:
    ALSAOutput();
    ~ALSAOutput() override;

    // AudioOutput interface
    bool initialize(const AudioFormat& format) override;
    void cleanup() override;
    bool start() override;
    bool stop() override;
    bool pause() override;

    // Linux-specific features
    bool setDeviceName(const std::string& device);
    bool enableMMap();
    bool setHardwareParams();

    std::vector<std::string> enumerateDevices();
    std::string getCardName(int card);
    std::string getDeviceName(int card, int device);

private:
    // ALSA components
    snd_pcm_t* pcmHandle_;
    snd_pcm_hw_params_t* hwParams_;
    snd_pcm_sw_params_t* swParams_;

    // Device configuration
    std::string deviceName_;
    bool useMMap_;
    int bufferTime_;
    int periodTime_;

    // Threading
    pthread_t audioThread_;
    std::atomic<bool> shouldStop_{false};

    // Helper methods
    bool setHwParams();
    bool setSwParams();
    void* audioThreadProc(void* arg);

    // static thread wrapper
    static void* threadWrapper(void* arg);
};
```

```cpp
// platform/linux/alsa_output.cpp
bool ALSAOutput::initialize(const AudioFormat& format) {
    int err;

    // Open PCM device
    const char* device = deviceName_.empty() ? "default" : deviceName_.c_str();
    err = snd_pcm_open(&pcmHandle_, device,
                       SND_PCM_STREAM_PLAYBACK, SND_PCM_NONBLOCK);
    if (err < 0) {
        std::cerr << "Unable to open PCM device: " << snd_strerror(err) << std::endl;
        return false;
    }

    // Allocate hardware parameter structure
    err = snd_pcm_hw_params_malloc(&hwParams_);
    if (err < 0) return false;

    // Fill with default values
    err = snd_pcm_hw_params_any(pcmHandle_, hwParams_);
    if (err < 0) return false;

    // Set access type
    snd_pcm_access_t access = useMMap_ ? SND_PCM_ACCESS_MMAP_INTERLEAVED
                                       : SND_PCM_ACCESS_RW_INTERLEAVED;
    err = snd_pcm_hw_params_set_access(pcmHandle_, hwParams_, access);
    if (err < 0) {
        std::cerr << "Access type not available: " << snd_strerror(err) << std::endl;
        return false;
    }

    // Set sample format
    err = snd_pcm_hw_params_set_format(pcmHandle_, hwParams_, SND_PCM_FORMAT_FLOAT_LE);
    if (err < 0) return false;

    // Set channel count
    err = snd_pcm_hw_params_set_channels(pcmHandle_, hwParams_, format.channels);
    if (err < 0) return false;

    // Set sample rate
    unsigned int rate = format.sampleRate;
    err = snd_pcm_hw_params_set_rate_near(pcmHandle_, hwParams_, &rate, 0);
    if (err < 0) return false;

    // Set buffer time
    err = snd_pcm_hw_params_set_buffer_time_near(pcmHandle_, hwParams_,
                                                 (unsigned int*)&bufferTime_, 0);
    if (err < 0) return false;

    // Set period time
    err = snd_pcm_hw_params_set_period_time_near(pcmHandle_, hwParams_,
                                                 (unsigned int*)&periodTime_, 0);
    if (err < 0) return false;

    // Apply hardware parameters
    err = snd_pcm_hw_params(pcmHandle_, hwParams_);
    if (err < 0) {
        std::cerr << "Unable to set HW parameters: " << snd_strerror(err) << std::endl;
        return false;
    }

    // Allocate software parameter structure
    err = snd_pcm_sw_params_malloc(&swParams_);
    if (err < 0) return false;

    // Fill with default values
    err = snd_pcm_sw_params_current(pcmHandle_, swParams_);
    if (err < 0) return false;

    // Start threshold
    err = snd_pcm_sw_params_set_start_threshold(pcmHandle_, swParams_, 1);
    if (err < 0) return false;

    // Apply software parameters
    err = snd_pcm_sw_params(pcmHandle_, swParams_);
    if (err < 0) return false;

    return true;
}

void* ALSAOutput::audioThreadProc(void* arg) {
    ALSAOutput* self = static_cast<ALSAOutput*>(arg);

    // Set thread priority
    struct sched_param param;
    param.sched_priority = 50;  // Real-time priority
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);

    float* buffer = new float[4096 * currentFormat_.channels];

    while (!self->shouldStop_) {
        // Get audio data
        if (self->audioCallback_) {
            float* audioData = self->audioCallback_(4096);
            if (audioData) {
                memcpy(buffer, audioData, 4096 * currentFormat_.channels * sizeof(float));

                // Write to ALSA
                snd_pcm_sframes_t framesWritten = snd_pcm_writei(
                    self->pcmHandle_, buffer, 4096);

                if (framesWritten < 0) {
                    if (framesWritten == -EPIPE) {
                        // Buffer underrun
                        std::cerr << "Buffer underrun" << std::endl;
                        snd_pcm_prepare(self->pcmHandle_);
                    } else {
                        std::cerr << "Write error: " << snd_strerror(framesWritten) << std::endl;
                    }
                }
            }
        } else {
            usleep(10000);  // 10ms sleep
        }
    }

    delete[] buffer;
    return nullptr;
}
```

### 4.2 Linux File System Integration

```cpp
// platform/linux/filesystem.h
class LinuxFileSystemManager {
public:
    static LinuxFileSystemManager& instance();

    // File watching (inotify)
    bool watchDirectory(const std::string& path,
                       std::function<void(const std::string&)> callback);
    void stopWatching(const std::string& path);

    // Desktop integration
    bool installDesktopFile();
    bool registerMimeTypes();
    bool installIcons();

    // Linux-specific paths
    std::string getConfigPath();
    std::string getCachePath();
    std::string getMusicPath();

private:
    LinuxFileSystemManager() = default;

    // inotify
    int inotifyFd_;
    std::map<int, std::string> watchDescriptors_;
    std::thread watchThread_;
    std::atomic<bool> watching_{false};

    void processInotifyEvents();
};
```

### 4.3 Linux Desktop Integration

```cpp
// platform/linux/desktop_integration.cpp
bool LinuxDesktopIntegration::installDesktopFile() {
    std::string desktopDir = getDesktopFileDir();
    if (desktopDir.empty()) return false;

    std::string desktopFile = desktopDir + "/coremusicplayer.desktop";
    std::ofstream file(desktopFile);

    if (!file.is_open()) return false;

    file << "[Desktop Entry]\n";
    file << "Version=1.0\n";
    file << "Type=Application\n";
    file << "Name=coreMusicPlayer\n";
    file << "Comment=Cross-platform audio player\n";
    file << "Exec=" << getExecutablePath() << " %U\n";
    file << "Icon=coremusicplayer\n";
    file << "MimeType=audio/mpeg;audio/wav;audio/flac;audio/ogg;application/ogg;\n";
    file << "Categories=AudioVideo;Audio;Player;\n";
    file << "StartupNotify=true\n";

    file.close();

    // Update desktop database
    std::system("update-desktop-database ~/.local/share/applications/");

    return true;
}

bool LinuxDesktopIntegration::registerMimeTypes() {
    std::string mimeDir = std::string(getenv("HOME")) + "/.local/share/mime/packages";
    createDirectory(mimeDir);

    std::string mimeFile = mimeDir + "/coremusicplayer.xml";
    std::ofstream file(mimeFile);

    if (!file.is_open()) return false;

    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<mime-info xmlns='http://www.freedesktop.org/standards/shared-mime-info'>\n";
    file << "  <mime-type type=\"audio/x-coremusicplayer-playlist\">\n";
    file << "    <comment>coreMusicPlayer Playlist</comment>\n";
    file << "    <glob pattern=\"*.xpl\"/>\n";
    file << "  </mime-type>\n";
    file << "</mime-info>\n";

    file.close();

    // Update mime database
    std::system("update-mime-database ~/.local/share/mime/");

    return true;
}
```

## 5. macOS Implementation

### 5.1 Audio Output - CoreAudio

```cpp
// platform/macos/coreaudio_output.h
class CoreAudioOutput : public AudioOutput {
public:
    CoreAudioOutput();
    ~CoreAudioOutput() override;

    // AudioOutput interface
    bool initialize(const AudioFormat& format) override;
    void cleanup() override;
    bool start() override;
    bool stop() override;
    bool pause() override;

    // macOS-specific features
    bool setDeviceID(AudioDeviceID deviceID);
    std::vector<AudioDeviceID> enumerateDevices();
    bool enableHogMode();
    bool setIOProcFrameSize(UInt32 frameSize);

private:
    // CoreAudio components
    AudioDeviceID deviceID_;
    AudioUnit audioUnit_;
    AudioObjectID defaultOutputDevice_;

    // Format conversion
    AudioStreamBasicDescription inputFormat_;
    AudioStreamBasicDescription outputFormat_;
    AudioConverterRef converter_;

    // Threading
    std::thread audioThread_;
    std::atomic<bool> shouldStop_{false};

    // Callbacks
    static OSStatus audioInputProc(void* inRefCon,
                                   AudioUnitRenderActionFlags* ioActionFlags,
                                   const AudioTimeStamp* inTimeStamp,
                                   UInt32 inBusNumber,
                                   UInt32 inNumberFrames,
                                   AudioBufferList* ioData);
    OSStatus handleAudioInput(AudioUnitRenderActionFlags* ioActionFlags,
                             const AudioTimeStamp* inTimeStamp,
                             UInt32 inNumberFrames,
                             AudioBufferList* ioData);

    // Helper methods
    bool setupAudioUnit();
    bool setupFormatConversion();
    OSStatus devicePropertyChanged(AudioDeviceID inDevice,
                                   UInt32 inChannel,
                                   AudioObjectPropertyAddress inAddress,
                                   void* inClientData);
};
```

```cpp
// platform/macos/coreaudio_output.cpp
bool CoreAudioOutput::initialize(const AudioFormat& format) {
    OSStatus status;

    // Get default output device
    AudioObjectPropertyAddress propertyAddress = {
        kAudioHardwarePropertyDefaultOutputDevice,
        kAudioObjectPropertyScopeGlobal,
        kAudioObjectPropertyElementMaster
    };

    UInt32 dataSize = sizeof(defaultOutputDevice_);
    status = AudioObjectGetPropertyData(kAudioObjectSystemObject,
                                       &propertyAddress,
                                       0, nullptr,
                                       &dataSize, &defaultOutputDevice_);
    if (status != noErr) return false;

    deviceID_ = defaultOutputDevice_;

    // Setup input format (our format)
    inputFormat_.mSampleRate = format.sampleRate;
    inputFormat_.mFormatID = kAudioFormatLinearPCM;
    inputFormat_.mFormatFlags = kAudioFormatFlagIsFloat |
                                kAudioFormatFlagIsPacked |
                                kAudioFormatFlagsNativeEndian;
    inputFormat_.mChannelsPerFrame = format.channels;
    inputFormat_.mFramesPerPacket = 1;
    inputFormat_.mBitsPerChannel = format.bitDepth;
    inputFormat_.mBytesPerFrame = (inputFormat_.mBitsPerChannel *
                                   inputFormat_.mChannelsPerFrame) / 8;
    inputFormat_.mBytesPerPacket = inputFormat_.mBytesPerFrame;

    // Get device format
    propertyAddress.mSelector = kAudioDevicePropertyStreamFormat;
    dataSize = sizeof(outputFormat_);
    status = AudioObjectGetPropertyData(deviceID_,
                                       &propertyAddress,
                                       0, nullptr,
                                       &dataSize, &outputFormat_);
    if (status != noErr) return false;

    // Setup AudioUnit
    if (!setupAudioUnit()) return false;

    // Setup format conversion if needed
    if (!AudioStreamBasicDescriptionEqualTo(&inputFormat_, &outputFormat_, false)) {
        if (!setupFormatConversion()) return false;
    }

    return true;
}

bool CoreAudioOutput::setupAudioUnit() {
    OSStatus status;

    // Create AudioUnit
    AudioComponentDescription desc;
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_DefaultOutput;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;

    AudioComponent component = AudioComponentFindNext(nullptr, &desc);
    if (!component) return false;

    status = AudioComponentInstanceNew(component, &audioUnit_);
    if (status != noErr) return false;

    // Set device
    status = AudioUnitSetProperty(audioUnit_,
                                 kAudioOutputUnitProperty_CurrentDevice,
                                 kAudioUnitScope_Global,
                                 0, &deviceID_, sizeof(deviceID_));
    if (status != noErr) return false;

    // Set input format
    status = AudioUnitSetProperty(audioUnit_,
                                 kAudioUnitProperty_StreamFormat,
                                 kAudioUnitScope_Input,
                                 0, &inputFormat_, sizeof(inputFormat_));
    if (status != noErr) return false;

    // Set callback
    AURenderCallbackStruct callback;
    callback.inputProc = audioInputProc;
    callback.inputProcRefCon = this;

    status = AudioUnitSetProperty(audioUnit_,
                                 kAudioOutputUnitProperty_SetInputCallback,
                                 kAudioUnitScope_Input,
                                 0, &callback, sizeof(callback));
    if (status != noErr) return false;

    // Initialize AudioUnit
    status = AudioUnitInitialize(audioUnit_);
    if (status != noErr) return false;

    return true;
}

OSStatus CoreAudioOutput::handleAudioInput(AudioUnitRenderActionFlags* ioActionFlags,
                                          const AudioTimeStamp* inTimeStamp,
                                          UInt32 inNumberFrames,
                                          AudioBufferList* ioData) {
    // Get audio data from callback
    if (audioCallback_) {
        float* audioData = audioCallback_(inNumberFrames);
        if (audioData) {
            // Copy to output buffer
            float* outputBuffer = static_cast<float*>(ioData->mBuffers[0].mData);
            memcpy(outputBuffer, audioData,
                   inNumberFrames * inputFormat_.mChannelsPerFrame * sizeof(float));
        }
    }

    return noErr;
}
```

### 5.2 macOS Menu Bar Integration

```cpp
// platform/macos/menubar.h
@interface XpuMusicMenuBar : NSObject {
    NSStatusItem* statusItem_;
    NSMenu* menu_;
}

- (void)createMenuBar;
- (void)updateMenuBar;
- (void)showPlaybackState:(PlaybackState)state;
- (void)showCurrentTrack:(NSString*)title;

@end

@implementation XpuMusicMenuBar

- (void)createMenuBar {
    NSStatusBar* statusBar = [NSStatusBar systemStatusBar];
    statusItem_ = [statusBar statusItemWithLength:NSVariableStatusItemLength];

    [statusItem_ setTitle:@"♫ coreMusicPlayer"];
    [statusItem_ setHighlightMode:YES];

    menu_ = [[NSMenu alloc] init];

    // Add menu items
    [menu_ addItemWithTitle:@"Play/Pause"
                     action:@selector(togglePlayPause)
              keyEquivalent:@""];

    [menu_ addItemWithTitle:@"Stop"
                     action:@selector(stopPlayback)
              keyEquivalent:@""];

    [menu_ addItem:[NSMenuItem separatorItem]];

    [menu_ addItemWithTitle:@"Previous"
                     action:@selector(previousTrack)
              keyEquivalent:@""];

    [menu_ addItemWithTitle:@"Next"
                     action:@selector(nextTrack)
              keyEquivalent:@""];

    [menu_ addItem:[NSMenuItem separatorItem]];

    [menu_ addItemWithTitle:@"Show coreMusicPlayer"
                     action:@selector(showMainWindow)
              keyEquivalent:@""];

    [menu_ addItemWithTitle:@"Quit"
                     action:@selector(quitApplication)
              keyEquivalent:@""];

    [statusItem_ setMenu:menu_];
}

@end
```

## 6. Performance Optimization

### 6.1 Platform-Specific SIMD

```cpp
// platform/simd/platform_simd.h
class PlatformSIMD {
public:
    static bool hasSSE2();
    static bool hasSSE4();
    static bool hasAVX();
    static bool hasAVX2();
    static bool hasNEON();  // ARM

    // Optimized functions
    static void processAudioSSE2(float* buffer, size_t frames, float gain);
    static void processAudioAVX(float* buffer, size_t frames, float gain);
    static void processAudioNEON(float* buffer, size_t frames, float gain);

private:
    static bool detectSSE2();
    static bool detectSSE4();
    static bool detectAVX();
    static bool detectAVX2();
    static bool detectNEON();
};

// Platform-specific implementations
#ifdef _WIN32
bool PlatformSIMD::hasSSE2() {
    int cpuInfo[4];
    __cpuid(cpuInfo, 0);
    if (cpuInfo[0] >= 1) {
        __cpuid(cpuInfo, 1);
        return (cpuInfo[3] & (1 << 26)) != 0;  // EDX bit 26
    }
    return false;
}
#elif defined(__linux__)
bool PlatformSIMD::hasSSE2() {
    return __builtin_cpu_supports("sse2");
}
#endif
```

### 6.2 Threading Optimization

```cpp
// platform/threading/optimizations.h
class ThreadOptimizer {
public:
    // Set optimal thread priority for audio
    static void setAudioThreadPriority();

    // Set CPU affinity for audio thread
    static void setAudioThreadAffinity();

    // Optimize thread stack size
    static size_t getOptimalStackSize();

    // Platform-specific sleep implementations
    static void preciseSleep(std::chrono::microseconds duration);

private:
#ifdef PLATFORM_WINDOWS
    static HANDLE audioThread_;
#elif defined(PLATFORM_LINUX)
    static pthread_t audioThread_;
#endif
};

// Implementation
void ThreadOptimizer::setAudioThreadPriority() {
#ifdef PLATFORM_WINDOWS
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
#elif defined(PLATFORM_LINUX)
    struct sched_param param;
    param.sched_priority = 80;  // High priority
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
#elif defined(PLATFORM_MACOS)
    struct sched_param param;
    param.sched_priority = 47;  // High priority on macOS
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param);
#endif
}
```

## 7. Testing Platform-Specific Code

### 7.1 Cross-Platform Test Framework

```cpp
// tests/platform_test_helpers.h
class PlatformTestHelper {
public:
    // Mock platform services for testing
    static void initializeMockServices();
    static void cleanupMockServices();

    // Audio testing
    static bool createTestAudioFile(const std::string& path,
                                   const AudioFormat& format,
                                   size_t frames);

    // Device testing
    static std::vector<std::string> getMockAudioDevices();
    static bool simulateDeviceRemoval();

private:
    static std::vector<std::string> mockDevices_;
};

// Test example
TEST(AudioOutputTest, CrossPlatformInitialization) {
    AudioFormat format;
    format.sampleRate = 44100;
    format.channels = 2;
    format.bitDepth = 32;

#ifdef PLATFORM_WINDOWS
    auto output = std::make_unique<WASAPIOutput>();
#elif defined(PLATFORM_LINUX)
    auto output = std::make_unique<ALSAOutput>();
#elif defined(PLATFORM_MACOS)
    auto output = std::make_unique<CoreAudioOutput>();
#endif

    EXPECT_TRUE(output->initialize(format));
    EXPECT_NE(output->getLatency(), 0.0);
    EXPECT_TRUE(output->start());
    EXPECT_TRUE(output->stop());
}
```

This platform-specific documentation ensures coreMusicPlayer can be effectively developed, maintained, and optimized for each target platform while maintaining code consistency across platforms.