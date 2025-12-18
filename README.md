# coreMusicPlayer v1.0.0

## 项目概述

coreMusicPlayer 是一个高性能跨平台音频播放器，支持多种音频格式和Foobar2000插件系统集成。该项目实现了完整的音频处理管道，包括解码、处理、渲染等核心功能。

## 主要特性

### 音频处理
- 支持 WAV, MP3, FLAC, OGG 等多种音频格式
- 数字音量控制和多段均衡器
- DSP效果处理器
- SIMD优化的音频处理算法

### 播放策略
- Legacy模式：基础播放功能
- Complete模式：完整功能集
- Realtime模式：低延迟实时处理
- Production模式：生产就绪版本
- MultiFormat模式：智能格式检测

### Foobar2000集成
- 完整的插件管理系统
- 输入、DSP、输出插件适配器
- 插件热加载支持

### 用户界面
- Qt6图形用户界面
- 主题管理系统（亮色/暗色/自定义）
- 国际化支持
- 响应式设计

## 构建和运行

### 系统要求
- CMake 3.14 或更高版本
- C++17 编译器
- Qt6 开发库
- Google Test 框架

### 构建步骤

#### Windows
```cmd
build.bat
```

#### Linux/macOS
```bash
chmod +x build.sh
./build.sh
```

### 运行测试
```bash
# Windows
run_tests.bat

# Linux/macOS
chmod +x run_tests.sh
./run_tests.sh
```

## 项目结构

```
coreMusicPlayer/
├── CMakeLists.txt          # 构建配置
├── README.md               # 项目说明
├── include/                # 头文件
├── src/                    # 源代码
├── tests/                  # 测试代码
├── docs/                   # 文档
├── build.bat               # Windows构建脚本
├── build.sh                # Linux/macOS构建脚本
└── RELEASE_NOTES.md        # 发布说明
```

## 许可证

MIT License - 详见 LICENSE 文件

## 贡献

欢迎通过提交issue或pull request来贡献代码。

## 支持

如有问题，请在GitHub上提交issue。