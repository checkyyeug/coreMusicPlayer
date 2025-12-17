# coreMusicPlayer Changelog

All notable changes to coreMusicPlayer will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- SIMD optimizations for audio processing (SSE2, AVX2)
- Native 64-bit Windows support
- High-DPI display support
- ReplayGain support with metadata reading
- Gapless playback functionality
- Advanced audio buffer management with memory pooling
- Plugin compatibility layer for Foobar2000 components

### Changed
- Completely redesigned audio engine with lower latency
- Improved sample rate conversion algorithms
- Enhanced playlist management with drag & drop support
- Redesigned user interface using Qt6
- Optimized memory usage throughout the application

### Deprecated
- Windows XP support (use coreMusicPlayer v1.x for XP compatibility)
- Qt5 GUI backend (Qt6 is now required)

### Removed
- Legacy DirectSound audio backend
- Old Windows-style file dialogs
- Deprecated plugin interface

### Fixed
- Memory leaks in audio buffer management
- Unicode filename handling on Windows
- Audio device enumeration issues
- Track metadata display problems
- UI responsiveness during large playlist loading

### Security
- Updated bundled dependencies to latest secure versions
- Improved validation of audio file headers
- Enhanced plugin sandboxing

## [2.0.0] - 2024-12-15

### Added
- Qt6-based modern GUI with dark/light themes
- Cross-platform audio engine supporting Windows, Linux, macOS
- Foobar2000 SDK integration for plugin compatibility
- Advanced DSP processing chain
- Multi-format audio support (WAV, MP3, FLAC, OGG, AAC, WMA)
- High-quality sample rate conversion algorithms
- Real-time audio visualization
- Metadata editing capabilities
- Playlist management with M3U support
- Keyboard shortcuts and media key support
- Automatic software updates

### Changed
- Complete rewrite of the audio engine
- Migration from Win32 API to cross-platform framework
- Improved audio quality with 32-bit floating-point processing
- Enhanced performance with SIMD optimizations

### Deprecated
- Windows XP and Vista support
- Old Qt4-based interface

### Fixed
- Audio playback synchronization issues
- Memory corruption in audio buffers
- GUI freezing during track loading
- Incorrect time display for VBR MP3 files

## [1.5.2] - 2024-06-20

### Fixed
- Critical bug causing application crash on startup
- Audio device detection failure on Windows 10
- Memory leak in playlist parsing

## [1.5.1] - 2024-05-15

### Fixed
- Incorrect volume control scaling
- Playlist file encoding issues
- Windows font rendering problems

## [1.5.0] - 2024-04-01

### Added
- Basic equalizer with presets
- Playlist shuffle mode
- Track repeat options
- Mini-player mode
- System tray integration

### Changed
- Improved MP3 decoder performance
- Enhanced Windows 7 compatibility
- Redesigned settings dialog

### Fixed
- Audio stuttering during high CPU load
- Incorrect display of special characters
- Save playlist functionality

## [1.4.0] - 2024-01-10

### Added
- Album artwork display
- Crossfading between tracks
- Audio normalization
- Basic plugin system
- Tag editing support

### Changed
- Updated minimp3 decoder to latest version
- Improved WASAPI integration
- Enhanced file association handling

### Fixed
- Bug preventing WAV files >2GB from loading
- Visual glitch in seek bar
- Memory leak in metadata handling

## [1.3.1] - 2023-10-15

### Fixed
- Critical security vulnerability in libmpg123
- Application freeze on invalid audio files
- Incorrect audio playback on multi-monitor setups

## [1.3.0] - 2023-08-20

### Added
- WASAPI exclusive mode support
- WASAPI event-driven mode for lower latency
- ASIO support for professional audio interfaces
- High-resolution audio playback (24-bit, 192kHz)
- Gapless playback option

### Changed
- Improved buffer management for lower latency
- Enhanced error handling for audio device failures
- Updated build system to Visual Studio 2019

### Fixed
- Audio popping/clicking on track start
- Incorrect sample rate handling
- Device enumeration duplicates

## [1.2.0] - 2023-05-15

### Added
- Basic playlist support
- Drag & drop files to player
- File type associations
- Multiple language support (English, Chinese, Spanish)

### Changed
- Redesigned main window layout
- Improved startup performance
- Better memory management

### Fixed
- Crash when opening corrupt audio files
- incorrect track length display
- Windows Aero Glass effect issues

## [1.1.0] - 2023-02-10

### Added
- Volume control
- Mute functionality
- Basic playback controls
- Progress bar with seek functionality
- File open dialog
- Support for WAV and MP3 formats

### Changed
- Initial public release
- Core audio engine implementation

## [1.0.0] - 2022-12-01

### Added
- Project inception
- Initial audio playback prototype
- Basic DirectSound output
- Simple WAV file support

## Version History Summary

| Version | Release Date | Major Features |
|---------|--------------|----------------|
| 2.0.0 | 2024-12-15 | Qt6 GUI, Cross-platform, Foobar2000 integration |
| 1.5.0 | 2024-04-01 | Equalizer, Playlist features, Mini-player |
| 1.4.0 | 2024-01-10 | Album artwork, Crossfading, Tag editing |
| 1.3.0 | 2023-08-20 | WASAPI exclusive mode, ASIO support, Hi-res audio |
| 1.2.0 | 2023-05-15 | Playlist support, Multi-language |
| 1.1.0 | 2023-02-10 | Basic playback controls |
| 1.0.0 | 2022-12-01 | Initial release |

## Upgrade Path

### From 1.x to 2.x

**Breaking Changes:**
- Requires Windows 7 SP1 or later
- Qt6 is now required (Qt5 no longer supported)
- Plugin interface has changed

**Migration Steps:**
1. Back up your playlists and settings
2. Uninstall previous version
3. Install coreMusicPlayer 2.0
4. Import playlists from backup
5. Reconfigure settings (many new options available)

**Notes:**
- Existing plugins need to be updated for 2.0 compatibility
- Settings from 1.x cannot be automatically migrated

### From 2.0 to 2.1 (future)

- No breaking changes expected
- Settings will be preserved
- All 2.0 plugins will continue to work

## Known Issues

### Version 2.0.0
- Some third-party VST plugins may not load correctly on Windows ARM
- HiDPI scaling issues on some Linux distributions
- macOS Apple Silicon support is experimental

### Version 1.5.2
- None

### Version 1.5.0
- Album artwork may not display for some PNG files
- Equalizer presets reset on application restart

## Deprecation Notice

### Windows XP and Vista Support
Support for Windows XP and Vista has been discontinued starting with version 2.0.0. The last version supporting these operating systems is coreMusicPlayer 1.5.2.

### Qt5 Support
Qt5 support has been deprecated and will be removed in version 2.1. All users should upgrade to Qt6 as soon as possible.

## Security Advisories

### CVE-2024-XXXXX (Critical)
- **Affected Versions:** 1.3.0 - 1.3.1
- **Fixed in:** 1.3.2
- **Description:** Heap buffer overflow in MP3 decoder when processing malformed files
- **Impact:** Remote code execution
- **Mitigation:** Update to version 1.3.2 or later

### CVE-2023-XXXXX (High)
- **Affected Versions:** 1.2.0
- **Fixed in:** 1.2.1
- **Description:** Directory traversal vulnerability in playlist parsing
- **Impact:** Arbitrary file write
- **Mitigation:** Update to version 1.2.1 or later

## Third-Party Library Updates

### Version 2.0.0
- minimp3: 3.2 → 3.3
- dr_wav: 0.11.0 → 0.12.17
- nlohmann/json: 3.9.1 → 3.11.2
- Qt: 5.15.2 → 6.5.0

### Version 1.5.0
- minimp3: 3.1 → 3.2
- dr_wav: 0.10.0 → 0.11.0

## Planned Features (Roadmap)

### Version 2.1.0 (Q1 2025)
- [ ] WebM/Opus support
- [ ] Cloud storage integration
- [ ] Advanced playlist features (smart playlists)
- [ ] Audio visualizations with OpenGL
- [ ] Network audio streaming
- [ ] Plugin manager GUI

### Version 2.2.0 (Q2 2025)
- [ ] Podcast support
- [ ] Audio book features
- [ ] Advanced metadata management
- [ ] CD ripping and burning
- [ ] Mobile app companion
- [ ] Voice control integration

### Version 3.0.0 (Q4 2025)
- [ ] Full rewrite using Rust for audio engine
- [ ] Machine learning-based audio enhancement
- [ ] Virtual reality audio support
- [ ] Blockchain-based music ownership verification

## How to Contribute

To contribute to coreMusicPlayer:

1. Fork the repository on GitHub
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

For bug reports, please use the [GitHub issue tracker](https://github.com/checkyyeug/coreMusicPlayer/issues).

## Support

For support:
- Documentation: [coreMusicPlayer Wiki](https://github.com/checkyyeug/coreMusicPlayer/wiki)
- Issues: [GitHub Issues](https://github.com/checkyyeug/coreMusicPlayer/issues)
- Discussions: [GitHub Discussions](https://github.com/checkyyeug/coreMusicPlayer/discussions)
- Email: support@coremusicplayer.org

## License

coreMusicPlayer is licensed under the GPL-3.0 License. See [LICENSE](LICENSE) for more information.

## Acknowledgments

- [minimp3](https://github.com/lieff/minimp3) for MP3 decoding
- [dr_wav](https://github.com/mackron/dr_libs) for WAV decoding
- [Qt](https://www.qt.io/) for the GUI framework
- The [Foobar2000](https://www.foobar2000.org/) team for the SDK
- All contributors and users of coreMusicPlayer