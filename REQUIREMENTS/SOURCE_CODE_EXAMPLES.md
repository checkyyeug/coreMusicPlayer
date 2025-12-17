# coreMusicPlayer Source Code Examples

This document contains key code examples from the coreMusicPlayer project to help understand the implementation patterns and best practices.

## 1. Core Audio Engine Examples

### 1.1 Audio Buffer Implementation

```cpp
// src/audio/audio_buffer.h
#pragma once

#include <memory>
#include <vector>

/**
 * @brief Thread-safe audio buffer container
 *
 * Provides storage for multi-channel audio data with SIMD-aligned
 * memory allocation for optimal performance with vectorized
 * operations.
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
     * @brief Get pointer to audio data
     * @return Pointer to first sample
     */
    float* getData() noexcept { return data_.get(); }

    /**
     * @brief Get const pointer to audio data
     * @return Pointer to first sample
     */
    const float* getData() const noexcept { return data_.get(); }

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
     * @brief Access channel data
     * @param channel Channel index (0-based)
     * @return Pointer to channel data
     */
    float* getChannelData(size_t channel) noexcept {
        return data_.get() + channel * frames_;
    }

    const float* getChannelData(size_t channel) const noexcept {
        return data_.get() + channel * frames_;
    }

    /**
     * @brief Get sample at specific position
     * @param frame Frame index (0-based)
     * @param channel Channel index (0-based)
     * @return Sample value
     */
    float getSample(size_t frame, size_t channel) const noexcept {
        return data_[frame * channels_ + channel];
    }

    /**
     * @brief Set sample at specific position
     * @param frame Frame index (0-based)
     * @param channel Channel index (0-based)
     * @param value Sample value
     */
    void setSample(size_t frame, size_t channel, float value) {
        data_[frame * channels_ + channel] = value;
    }

private:
    size_t channels_;
    size_t frames_;
    std::unique_ptr<float[]> data_;
    mutable std::mutex mutex_;

    static constexpr size_t ALIGNMENT = 32; // For AVX optimization

    void allocateMemory();
    void freeMemory();
};
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

These examples showcase the core implementation patterns used throughout the coreMusicPlayer project, demonstrating proper C++ practices, design patterns, and platform-specific considerations. Each example includes proper error handling, resource management, and documentation.