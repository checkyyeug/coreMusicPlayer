# coreMusicPlayer Frequently Asked Questions (FAQ)

## Table of Contents
- [Installation](#installation)
- [Audio Playback](#audio-playback)
- [Formats and Codecs](#formats-and-codecs)
- [Performance and Optimization](#performance-and-optimization)
- [GUI and Interface](#gui-and-interface)
- [Plugins and Extensions](#plugins-and-extensions)
- [Troubleshooting](#troubleshooting)
- [Advanced Topics](#advanced-topics)
- [Development](#development)

## Installation

### Q: What are the system requirements for coreMusicPlayer?

**A: Minimum Requirements:**
- **Windows**: Windows 7 SP1 or later (Windows 10/11 recommended)
- **Linux**: Any modern distribution with ALSA support
- **macOS**: macOS 10.15 (Catalina) or later
- **CPU**: Dual-core processor at 1.8GHz
- **RAM**: 4GB (8GB recommended)
- **Disk Space**: 100MB for installation

**Recommended Requirements:**
- **CPU**: Quad-core processor at 2.5GHz
- **RAM**: 8GB or more
- **Audio**: ASIO-capable sound card for professional use

### Q: Does coreMusicPlayer work on Windows ARM?

**A:** Yes, coreMusicPlayer 2.0 and later includes native Windows ARM64 support. However, some third-party plugins designed for x86/x64 may not work correctly on ARM systems.

### Q: Can I install coreMusicPlayer on a USB drive?

**A:** Yes, coreMusicPlayer supports portable installation. Download the portable version, extract to your USB drive, and run `coreMusicPlayer.exe`. The application will store all settings in the application directory.

### Q: How do I install coreMusicPlayer on Linux?

**A:** There are several installation methods:

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install ./xpumusic_2.0.0_amd64.deb
```

**Fedora/CentOS:**
```bash
sudo dnf install ./coremusicplayer-2.0.0.x86_64.rpm
```

**AppImage (Universal):**
```bash
chmod +x coreMusicPlayer-2.0.0.AppImage
./coreMusicPlayer-2.0.0.AppImage
```

**Flatpak:**
```bash
flatpak install flathub org.coremusicplayer.coreMusicPlayer
flatpak run org.coremusicplayer.coreMusicPlayer
```

### Q: Do I need to install any additional software?

**A:** coreMusicPlayer includes all necessary dependencies for basic operation. However, for full functionality:

- **Windows**: DirectX 9.0c or later (usually included with Windows)
- **Linux**: ALSA development libraries (`libasound2-dev` on Debian/Ubuntu)
- **Codecs**: Some proprietary formats may require additional codec packs

## Audio Playback

### Q: Why is there no sound coming from coreMusicPlayer?

**A:** Check the following:

1. **Volume Settings**:
   - Master volume in coreMusicPlayer is not muted
   - System volume is not muted
   - Volume slider in coreMusicPlayer is above 0%

2. **Audio Device**:
   - Go to Settings → Audio → Output Device
   - Select the correct audio device
   - Test with "Test Audio" button

3. **Windows Specific**:
   - Check Windows Sound settings
   - Ensure the default playback device is correct
   - Try disabling "Exclusive Mode" in coreMusicPlayer settings

4. **Linux Specific**:
   - Run `alsamixer` in terminal to check ALSA settings
   - Ensure your user is in the `audio` group: `sudo usermod -a -G audio $USER`

### Q: How do I reduce audio latency for real-time applications?

**A:** To achieve the lowest possible latency:

1. **Use WASAPI Exclusive Mode (Windows)**:
   - Settings → Audio → Enable "Exclusive Mode"
   - Set buffer size to minimum (128 frames)

2. **Use ASIO (Professional setups)**:
   - Install ASIO drivers for your audio interface
   - Select ASIO device in coreMusicPlayer settings

3. **Optimize Buffer Settings**:
   - Reduce buffer size gradually
   - Monitor for audio glitches
   - Typical low-latency settings: 128-256 frames

4. **System Optimizations**:
   - Disable other audio applications
   - Close unnecessary programs
   - Use high-performance power plan

### Q: What audio quality settings are recommended?

**A:** For optimal audio quality:

1. **Output Settings**:
   - Bit Depth: 24-bit or 32-bit float
   - Sample Rate: Match source or 96kHz for upsampling
   - Buffer Size: 512-1024 frames (balance of latency and stability)

2. **DSP Settings**:
   - Enable high-quality sample rate conversion
   - Use "Best" quality for resampling
   - Enable dithering for 16-bit output

3. **Format Support**:
   - Use lossless formats (FLAC, WAV, ALAC) for archival
   - High-bitrate MP3 (320kbps) for portable use
   - Consider OGG or AAC for better quality at lower bitrates

## Formats and Codecs

### Q: What audio formats does coreMusicPlayer support?

**A:** coreMusicPlayer supports a wide range of formats:

**Lossless:**
- WAV (PCM, IEEE Float)
- FLAC
- ALAC (Apple Lossless)
- WMA Lossless
- APE (Monkey's Audio)

**Lossy:**
- MP3 (all variants including VBR)
- OGG Vorbis
- AAC (M4A)
- WMA
- Opus
- Musepack

**Container Formats:**
- MP4/M4A
- Matroska (MKV - audio only)
- AVI (audio only)

**Note:** Support for some formats may require additional components or plugins.

### Q: Can coreMusicPlayer play DSD or SACD files?

**A:** Direct DSD playback is not natively supported in version 2.0. However:
- DSD can be converted to PCM in real-time
- Future versions (2.1+) will include native DSD support
- Foobar2000 SACD plugin may work through compatibility layer

### Q: Why can't coreMusicPlayer play my AAC files purchased from iTunes?

**A:** iTunes purchases before 2009 may have DRM protection:
- These files cannot be played in any player except iTunes/iOS devices
- Files purchased after 2009 are DRM-free and should work
- Use iTunes Match to upgrade older DRM-protected tracks

### Q: Does coreMusicPlayer support gapless playback?

**A:** Yes, gapless playback is supported for:
- All lossless formats
- MP3 with LAME-encoded gapless information
- OGG Vorbis
- FLAC

To enable: Settings → Playback → Check "Enable gapless playback"

## Performance and Optimization

### Q: Why is coreMusicPlayer using so much CPU?

**A:** High CPU usage may be caused by:

1. **High-Quality DSP**:
   - Check if you're using "Best" quality sample rate conversion
   - Try "Good" quality for lower CPU usage

2. **Real-Time Visualizations**:
   - Disable visualizations if not needed
   - Simplify visualization settings

3. **High Sample Rates**:
   - Playing 192kHz audio requires more processing
   - Consider downsampling if not using high-end audio equipment

4. **Plugin Issues**:
   - Disable plugins one by one to identify the cause
   - Update outdated plugins

5. **Antivirus Interference**:
   - Add coreMusicPlayer to antivirus exclusions
   - Disable real-time scanning for music folders

### Q: How can I reduce memory usage?

**A:** To minimize memory usage:

1. **Playlist Management**:
   - Limit playlist size (<10,000 tracks recommended)
   - Clear playlist history regularly

2. **Cache Settings**:
   - Reduce audio buffer size in settings
   - Limit metadata cache size
   - Disable album art caching if not needed

3. **File Format**:
   - Use smaller album art images (<500x500 pixels)
   - Consider compressed formats for large libraries

### Q: coreMusicPlayer stutters when I have other programs running. What can I do?

**A:** Try these solutions:

1. **Priority Settings**:
   - Increase audio thread priority in advanced settings
   - Close unnecessary background applications

2. **Buffer Adjustment**:
   - Increase buffer size (2048-4096 frames)
   - Enable "Safeguard against buffer underruns"

3. **Power Settings**:
   - Use "High Performance" power plan
   - Disable CPU throttling in power options

4. **Hardware Acceleration**:
   - Ensure audio drivers are up to date
   - Disable exclusive mode if not needed

## GUI and Interface

### Q: Can I customize the interface?

**A:** Yes, coreMusicPlayer offers extensive customization:

1. **Themes**:
   - Built-in Light and Dark themes
   - Custom theme support (CSS-based)
   - Download themes from the community

2. **Layout**:
   - Dockable panels for playlist, library, etc.
   - Save and switch between layouts
   - Compact mode for minimal interface

3. **Fonts and Colors**:
   - Customizable fonts in settings
   - Adjustable font sizes
   - Color scheme editor

### Q: How do I use keyboard shortcuts?

**A:** Default shortcuts include:

**Playback:**
- Space: Play/Pause
- S: Stop
- Left Arrow: Previous track
- Right Arrow: Next track
- Ctrl+Left: Seek backward
- Ctrl+Right: Seek forward

**Volume:**
- Up Arrow: Volume up
- Down Arrow: Volume down
- M: Mute/Unmute

**Playlist:**
- Delete: Remove selected track
- Ctrl+A: Select all
- Ctrl+F: Find in playlist

**Full list:** View → Keyboard Shortcuts

Custom shortcuts can be configured in Settings → Keyboard

### Q: Can I control coreMusicPlayer with media keys on my keyboard?

**A:** Yes, coreMusicPlayer supports standard media keys:
- Play/Pause
- Stop
- Next/Previous
- Volume Up/Down
- Mute

If media keys don't work:
1. Check if another application is capturing them
2. Ensure "Enable media key support" is checked in settings
3. On Windows, check if Windows is using "Focus assist" mode

### Q: How do I make coreMusicPlayer stay on top of other windows?

**A:** Enable "Always on top" mode:
- Right-click title bar → "Always on top"
- Or: View → Always on top
- Shortcut: Ctrl+T

## Plugins and Extensions

### Q: Can I use Foobar2000 plugins with coreMusicPlayer?

**A:** Yes, coreMusicPlayer 2.0+ includes a Foobar2000 compatibility layer:

1. **Supported Plugin Types**:
   - Input decoders (additional format support)
   - DSP processors (effects, enhancements)
   - Visualization plugins
   - Some UI components (limited)

2. **Installation**:
   - Copy .dll files to `plugins/foobar2000/` directory
   - Restart coreMusicPlayer
   - Enable plugins in Settings → Plugins

3. **Limitations**:
   - Not all plugins are compatible
   - UI components may not work correctly
   - Some features might be unavailable

### Q: Where can I find plugins for coreMusicPlayer?

**A:** Sources for plugins:

1. **Official Repository**: https://plugins.coremusicplayer.org
2. **GitHub**: Search for "coremusicplayer-plugin"
3. **Foobar2000 Components**: Many work with compatibility layer
4. **Community Forums**: User-created plugins and tweaks

### Q: How do I develop my own plugin?

**A:** Plugin development requires:

1. **SDK Installation**:
   - Download coreMusicPlayer SDK from website
   - Include headers and libraries in your project

2. **Sample Projects**:
   - Input plugin sample
   - DSP plugin sample
   - UI plugin sample

3. **Documentation**:
   - Plugin Development Guide
   - API Reference
   - Best Practices

See `docs/plugin_development.md` for detailed instructions.

## Troubleshooting

### Q: coreMusicPlayer crashes on startup. What should I do?

**A:** Try these solutions:

1. **Reset Settings**:
   ```bash
   # Windows
   del %APPDATA%\coreMusicPlayer\settings.json

   # Linux
   rm ~/.config/coreMusicPlayer/settings.json
   ```

2. **Disable Plugins**:
   - Move `plugins/` directory to `plugins_old/`
   - Restart coreMusicPlayer
   - Re-enable plugins one by one

3. **Check Audio Drivers**:
   - Update audio drivers
   - Try with different audio device
   - Disable audio enhancements

4. **Create Log File**:
   - Run with `--debug` flag
   - Check `logs/` directory for error details

### Q: Why does coreMusicPlayer crash when opening certain files?

**A:** This usually indicates:

1. **Corrupted File**:
   - Try opening with another player
   - Check file integrity with a tool like `mp3val`

2. **Codec Issue**:
   - Update coreMusicPlayer to latest version
   - Install codec packs (K-Lite, CCCP)
   - Try converting to another format

3. **Memory Issue**:
   - Very large files may exceed memory limits
   - Try reducing buffer size in settings

4. **Bug Report**:
   - Submit crash report with file sample
   - Include debug logs if possible

### Q: Album artwork is not displaying. Why?

**A:** Common solutions:

1. **Check Settings**:
   - Settings → Interface → Show album artwork
   - Check "Download missing artwork"

2. **Artwork Location**:
   - Same folder as audio file (folder.jpg, cover.jpg)
   - Embedded in file metadata
   - Subfolder `artwork/`

3. **Supported Formats**:
   - JPG, PNG, BMP, GIF
   - Maximum size: 10MB
   - Recommended size: 500x500 pixels

4. **Refresh Library**:
   - Right-click → Refresh metadata
   - Re-scan library folders

### Q: Search is very slow in large libraries. How can I speed it up?

**A:** Optimize search performance:

1. **Index Settings**:
   - Enable "Build search index" in settings
   - Schedule regular indexing during off-hours

2. **Search Filters**:
   - Use specific fields (artist:, title:, album:)
   - Limit search scope with folders
   - Use quotes for exact phrases

3. **Library Management**:
   - Split large libraries into smaller collections
   - Remove duplicate entries
   - Clean up non-audio files

## Advanced Topics

### Q: Can I use coreMusicPlayer as a Digital Audio Transport (DAT)?

**A:** Yes, coreMusicPlayer can be configured for audiophile use:

1. **Bit-Perfect Playback**:
   - Disable all DSP and volume control
   - Use WASAPI exclusive mode
   - Set to highest supported sample rate

2. **Hardware Requirements**:
   - High-quality DAC (Digital-to-Analog Converter)
   - Low-jitter audio interface
   - Quality interconnects

3. **Configuration**:
   - Settings → Audio → Advanced
   - Enable "Bit-perfect output"
   - Disable all audio enhancements

### Q: How does coreMusicPlayer handle ReplayGain?

**A:** ReplayGain support includes:

1. **Metadata Reading**:
   - Automatic detection in ID3v2, Vorbis, APE tags
   - Track and album gain modes
   - Peak level information

2. **Processing**:
   - Lossless gain adjustment
   - Clipping protection
   - Per-track or per-album modes

3. **Configuration**:
   - Settings → Playback → ReplayGain
   - Set reference loudness (-89dB or -18LUFS)
   - Choose preprocessing mode

### Q: Can coreMusicPlayer be controlled remotely?

**A:** Remote control options:

1. **Web Interface**:
   - Enable web server in settings
   - Access via browser on same network
   - Control playback, manage playlist

2. **HTTP API**:
   - RESTful API for integration
   - JSON responses
   - Authentication support

3. **Third-party Apps**:
   - Official mobile app (in development)
   - Community remote control apps
   - Home automation integration

### Q: How does coreMusicPlayer handle gapless playback?

**A:** Gapless playback implementation:

1. **Supported Formats**:
   - Native: FLAC, WAV, OGG
   - Requires metadata: MP3 (LAME), AAC

2. **Technical Details**:
   - Pre-loading of next track
   - Sample-accurate timing
   - Crossfade option (configurable)

3. **Requirements**:
   - Sufficient buffer size
   - Fast storage for track loading
   - Accurate length metadata

## Development

### Q: I want to contribute to coreMusicPlayer. Where do I start?

**A:** Contribution guidelines:

1. **Get the Code**:
   ```bash
   git clone https://github.com/checkyyeug/coreMusicPlayer.git
   cd coreMusicPlayer
   ```

2. **Build Instructions**:
   - Follow `BUILD_GUIDE.md`
   - Set up development environment
   - Install required dependencies

3. **Areas Needing Help**:
   - Linux/ARM porting
   - Plugin development
   - Documentation
   - Translation
   - Bug fixes

4. **Submit Changes**:
   - Fork repository
   - Create feature branch
   - Submit pull request
   - Follow code standards

### Q: What programming language is coreMusicPlayer written in?

**A:** coreMusicPlayer uses multiple technologies:

1. **Core Engine**: C++17
   - Audio processing
   - File handling
   - Platform abstraction

2. **GUI**: Qt6 (C++)
   - Cross-platform UI
   - Signal/slot system
   - Model/View framework

3. **Plugins**: C++
   - C-compatible API
   - Dynamic loading
   - Version compatibility

4. **Scripts**: Various
   - Build: CMake, Batch, Shell
   - Tools: Python, PowerShell

### Q: How do I report bugs or request features?

**A:** Best practices for reporting:

1. **Bug Reports**:
   - Use GitHub Issues
   - Include system information
   - Provide reproduction steps
   - Attach relevant logs

2. **Feature Requests**:
   - Check existing issues first
   - Provide detailed description
   - Explain use case
   - Consider contribution

3. **Contact Information**:
   - Issues: https://github.com/checkyyeug/coreMusicPlayer/issues
   - Discussions: https://github.com/checkyyeug/coreMusicPlayer/discussions
   - Email: bugs@coremusicplayer.org

### Q: Is coreMusicPlayer open source?

**A:** Yes, coreMusicPlayer is licensed under GPL-3.0:

- Free to use, modify, and distribute
- Source code available on GitHub
- Contributions welcome
- Commercial use requires GPL compliance

For proprietary licensing options, contact: licensing@coremusicplayer.org

---

## Still Have Questions?

- **Documentation**: Check the [Wiki](https://github.com/checkyyeug/coreMusicPlayer/wiki)
- **Community**: Join our [Discord server](https://discord.gg/coremusicplayer)
- **Email**: support@coremusicplayer.org
- **Forums**: https://forums.coremusicplayer.org

Last updated: December 15, 2024