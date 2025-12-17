# coreMusicPlayer User Manual

## Table of Contents
1. [Introduction](#1-introduction)
2. [Installation](#2-installation)
3. [Quick Start](#3-quick-start)
4. [Command-Line Interface](#4-command-line-interface)
5. [Configuration](#5-configuration)
6. [Audio Formats](#6-audio-formats)
7. [Plugins](#7-plugins)
8. [Troubleshooting](#8-troubleshooting)
9. [Advanced Usage](#9-advanced-usage)

## 1. Introduction

coreMusicPlayer is a lightweight, cross-platform audio player designed for high-quality audio playback with minimal resource usage. It supports multiple audio formats and can be extended through plugins.

### Key Features
- **Cross-platform support**: Windows and Linux
- **Multiple audio formats**: WAV, MP3, FLAC, OGG
- **Low latency playback**: Optimized for real-time performance
- **Plugin system**: Compatible with Foobar2000 plugins
- **Command-line interface**: Perfect for scripting and automation
- **Lightweight design**: Minimal resource footprint

### Available Versions
- `music_player_legacy`: Basic WAV player
- `music_player_complete`: Full-featured player with plugin support
- `final_wav_player`: Production-ready WAV player
- `multi_format_player`: Main application with multi-format support

## 2. Installation

### Windows

#### Option 1: Download Pre-built Binary
1. Download the latest release from GitHub
2. Extract the ZIP file to a folder
3. Run `coreMusicPlayer.bat` or `multi_format_player.exe`

#### Option 2: Build from Source
```batch
# Clone the repository
git clone https://github.com/checkyyeug/coreMusicPlayer.git
cd coreMusicPlayer

# Build using the provided script
build_windows.bat
```

### Linux

#### Option 1: Download Pre-built Binary
```bash
# Download and extract
wget https://github.com/checkyyeug/coreMusicPlayer/releases/latest/coreMusicPlayer-Linux.tar.gz
tar -xzf coreMusicPlayer-Linux.tar.gz
cd coreMusicPlayer

# Run the player
./multi_format_player
```

#### Option 2: Build from Source
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install build-essential cmake libasound2-dev

# Clone and build
git clone https://github.com/checkyyeug/coreMusicPlayer.git
cd coreMusicPlayer
mkdir build && cd build
cmake ..
make -j4
```

## 3. Quick Start

### Basic Playback
```bash
# Play an audio file directly
./multi_format_player path/to/music.wav

# Or run in interactive mode
./multi_format_player
> load path/to/music.mp3
> play
```

### Using Different Players
```bash
# For WAV files only
./final_wav_player music.wav

# For full feature set
./music_player_complete music.flac

# For real-time processing
./real_player music.wav
```

## 4. Command-Line Interface

### Interactive Mode Commands

When you run the player without specifying a file, you enter interactive mode:

| Command | Description | Example |
|---------|-------------|---------|
| `load <file>` | Load an audio file | `load music.mp3` |
| `play` | Start or resume playback | `play` |
| `pause` | Pause playback | `pause` |
| `stop` | Stop playback and reset position | `stop` |
| `seek <percent>` | Seek to position (0-100) | `seek 50` |
| `status` | Show current playback status | `status` |
| `volume <level>` | Set volume (0-100) | `volume 75` |
| `help` | Show available commands | `help` |
| `quit` or `exit` | Exit the player | `quit` |

### Command-Line Options

```bash
# Usage: multi_format_player [options] [file]

Options:
  -h, --help          Show help message
  -v, --version       Show version information
  -c, --config <file> Use custom configuration file
  -d, --device <id>   Use specific audio device
  --list-devices      List available audio devices
  --no-gui            Disable GUI components
```

### Examples

```bash
# Play with specific configuration
./multi_format_player -c myconfig.ini music.wav

# List available audio devices
./multi_format_player --list-devices

# Play with specific device
./multi_format_player -d 2 music.wav
```

## 5. Configuration

### Configuration File Location
- Windows: `%APPDATA%\coreMusicPlayer\config.ini`
- Linux: `~/.config/coreMusicPlayer/config.ini`
- Local: `config.ini` in application directory

### Configuration Options

```ini
# Audio Settings
[audio]
device=default          # Audio device name or 'default'
sample_rate=44100       # Preferred sample rate
buffer_size=1024        # Audio buffer size
latency=low             # latency: low/medium/high

# Playback Settings
[playback]
volume=80               # Volume level (0-100)
auto_play=false         # Auto-play when file loaded
gapless=true           # Enable gapless playback

# User Interface
[gui]
theme=dark              # Theme: light/dark
show_progress=true     # Show progress bar
always_on_top=false    # Keep window on top

# Advanced
[advanced]
log_level=info          # Logging: debug/info/warning/error
enable_plugins=true    # Enable plugin system
plugin_path=plugins/    # Plugin directory
```

### Runtime Configuration

You can change most settings at runtime using the configuration commands:

```bash
# In interactive mode
> set volume 90
> set device speakers
> save config
```

## 6. Audio Formats

### Supported Formats

| Format | Extension | Support | Notes |
|--------|-----------|---------|-------|
| WAV | .wav | ✅ Native | 8, 16, 24, 32-bit PCM |
| MP3 | .mp3 | ✅ Plugin | Requires foo_input_mp3.dll |
| FLAC | .flac | ✅ Plugin | Requires foo_input_flac.dll |
| OGG | .ogg | ✅ Plugin | Requires foo_input_vorbis.dll |
| AAC | .aac/.m4a | ⚠️ Limited | Experimental support |
| WMA | .wma | ⚠️ Limited | Experimental support |

### WAV File Support

coreMusicPlayer has native support for WAV files with the following specifications:
- **Sample rates**: 8kHz to 192kHz
- **Bit depths**: 8, 16, 24, 32-bit
- **Channels**: Mono, Stereo
- **Formats**: PCM (uncompressed) only

### High-Resolution Audio

For high-resolution audio files:
- Ensure your hardware supports the sample rate
- Use `final_wav_player` for best quality
- Consider using exclusive mode on Windows

## 7. Plugins

### Installing Plugins

1. Download Foobar2000 compatible plugins
2. Place `.dll` files in the plugins directory:
   - Windows: `plugins/` folder
   - Linux: `~/.config/coreMusicPlayer/plugins/`

### Common Plugins

| Plugin | Function | Download |
|--------|----------|----------|
| foo_input_mp3 | MP3 decoding | Foobar2000 website |
| foo_input_flac | FLAC decoding | Foobar2000 website |
| foo_input_vorbis | OGG Vorbis decoding | Foobar2000 website |
| foo_dsp_eq | Equalizer | Foobar2000 website |

### Plugin Configuration

```ini
# Enable/disable plugins
[plugins]
foo_input_mp3=true
foo_input_flac=true
foo_dsp_eq=false

# Plugin-specific settings
[plugin_foo_dsp_eq]
preset=rock
preamp=0.0
band1=2.0
band2=1.5
```

## 8. Troubleshooting

### Common Issues

#### No Sound Output

**Windows:**
1. Check if Windows Audio Service is running
2. Ensure audio device is not muted
3. Try running as administrator
4. Check for audio driver updates

**Linux:**
```bash
# Check if user is in audio group
groups $USER | grep audio

# Add to audio group if needed
sudo usermod -a -G audio $USER
# Log out and log back in

# Test audio device
speaker-test -t wav
```

#### Plugin Not Loading

1. Verify plugin compatibility (Foobar2000 components only)
2. Check plugin architecture (64-bit required)
3. Ensure plugin dependencies are installed
4. Check plugin permissions

#### High CPU Usage

1. Try increasing buffer size:
   ```ini
   [audio]
   buffer_size=2048
   ```
2. Disable visualizations
3. Use appropriate quality settings for sample rate conversion

#### File Not Supported

1. Check if format is supported (see Section 6)
2. Verify file is not corrupted
3. Install required plugins for the format
4. Check file permissions

### Debug Mode

Enable debug logging to troubleshoot issues:

```ini
[advanced]
log_level=debug
debug_file=debug.log
```

### Getting Help

- **GitHub Issues**: https://github.com/checkyyeug/coreMusicPlayer/issues
- **Documentation**: Check the `docs/` directory
- **Community**: Join our Discord server

## 9. Advanced Usage

### Scripting

coreMusicPlayer can be used in scripts for automation:

```bash
#!/bin/bash
# Play a playlist
for file in *.mp3; do
    ./multi_format_player "$file"
    sleep 1  # Wait between tracks
done
```

### Keyboard Shortcuts (GUI)

| Shortcut | Function |
|----------|----------|
| Space | Play/Pause |
| S | Stop |
| → | Next Track |
| ← | Previous Track |
| ↑ | Volume Up |
| ↓ | Volume Down |
| F | Fullscreen |
| Esc | Exit |

### Performance Tips

1. **Use appropriate buffer sizes**:
   - Low latency: 256-512 samples
   - General use: 1024-2048 samples
   - Poor hardware: 4096+ samples

2. **Optimize for your hardware**:
   ```ini
   [audio]
   # For multi-core systems
   threads=4

   # For systems with limited memory
   max_memory_mb=256
   ```

3. **Disable unused features**:
   ```ini
   [gui]
   show_visualization=false

   [playback]
   gapless=false  # If not needed
   ```

### Network Streaming

While not a primary feature, some basic streaming is possible:

```bash
# Stream from HTTP (if format supported)
./multi_format_player http://example.com/stream.mp3

# Use with curl for custom protocols
curl -s http://example.com/stream.wav | ./multi_format_player -
```

## Appendix

### Exit Codes

| Code | Meaning |
|------|---------|
| 0 | Success |
| 1 | General error |
| 2 | File not found |
| 3 | Unsupported format |
| 4 | Audio device error |
| 5 | Configuration error |

### Default Keyboard Mappings

When in interactive mode:
- **Tab**: Autocomplete commands
- **↑/↓**: Navigate command history
- **Ctrl+C**: Cancel current operation
- **Ctrl+D**: Exit player

### Environment Variables

| Variable | Description |
|----------|-------------|
| `XPU_MUSIC_CONFIG` | Override config file location |
| `XPU_MUSIC_PLUGIN_PATH` | Override plugin directory |
| `XPU_MUSIC_LOG_LEVEL` | Override log level |

For the latest updates and detailed documentation, visit:
https://github.com/checkyyeug/coreMusicPlayer