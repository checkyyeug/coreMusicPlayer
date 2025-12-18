# coreMusicPlayer - Development Setup Guide

## Prerequisites

### Required Tools
- CMake 3.14 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Git
- Google Test framework
- Qt6 (for GUI features)

### Platform-Specific Requirements

#### Windows
- Visual Studio 2019 or later (with C++ support)
- Windows SDK
- DirectX SDK (for audio output)

#### Linux
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake qt6-base-dev libgtest-dev libasound2-dev

# CentOS/RHEL/Fedora
sudo yum install gcc-c++ cmake qt6-base-devel gtest-devel alsa-lib-devel
```

#### macOS
```bash
# Using Homebrew
brew install cmake qt6 googletest portaudio
```

## Building the Project

### Using CMake (Recommended)

1. **Clone the repository**
```bash
git clone https://github.com/your-username/coreMusicPlayer.git
cd coreMusicPlayer
```

2. **Create build directory and configure**
```bash
mkdir build
cd build
cmake ..
```

3. **Build the project**
```bash
# Windows (Visual Studio)
cmake --build . --config Release

# Linux/macOS
make -j$(nproc)
```

### Using Provided Scripts

#### Windows
```cmd
build.bat
```

#### Linux/macOS
```bash
chmod +x build.sh
./build.sh
```

## Running Tests

### Using CMake
```bash
cd build
ctest --output-on-failure
```

### Using Provided Scripts

#### Windows
```cmd
run_tests.bat
```

#### Linux/macOS
```bash
chmod +x run_tests.sh
./run_tests.sh
```

## Project Structure

```
coreMusicPlayer/
├── CMakeLists.txt          # Main CMake configuration
├── README.md               # Project overview
├── RELEASE_NOTES.md        # Release information
├── docs/                   # Documentation
│   ├── API_DOCUMENTATION.md  # API reference
│   └── CORE_DOCUMENTATION.md # Core module documentation
├── include/                # Header files
├── src/                    # Source code
├── tests/                  # Unit and integration tests
├── config/                 # Configuration files
└── build/                  # Build artifacts (not committed)
```

## Development Workflow

### 1. Fork and Clone
```bash
git clone https://github.com/your-username/coreMusicPlayer.git
cd coreMusicPlayer
git checkout -b feature/your-feature-name
```

### 2. Make Changes
- Follow existing code style and patterns
- Add unit tests for new functionality
- Update documentation as needed

### 3. Build and Test
```bash
./build.sh
./run_tests.sh
```

### 4. Commit and Push
```bash
git add .
git commit -m "Add feature: your feature description"
git push origin feature/your-feature-name
```

### 5. Create Pull Request
Submit a pull request to the main repository with:
- Clear description of changes
- Test results
- Documentation updates

## Code Style Guidelines

### C++ Standards
- Use C++17 features where appropriate
- Follow Google C++ Style Guide
- Prefer RAII over manual resource management
- Use smart pointers instead of raw pointers

### Naming Conventions
- Classes: PascalCase (e.g., `AudioEngine`)
- Functions: camelCase (e.g., `initializeEngine`)
- Variables: camelCase (e.g., `sampleRate`)
- Constants: UPPER_CASE (e.g., `MAX_BUFFER_SIZE`)

### Documentation
- All public APIs must have Doxygen-style comments
- Complex algorithms should include inline comments
- Class and function descriptions should explain purpose and usage

## Contributing

We welcome contributions to coreMusicPlayer! Please follow these steps:

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Run all tests to ensure no regressions
6. Submit a pull request with clear description

## Support

For development questions or issues:
- Open an issue on GitHub
- Check existing documentation in docs/ directory
- Contact the development team via email

## License

coreMusicPlayer is released under the MIT License. See LICENSE for details.