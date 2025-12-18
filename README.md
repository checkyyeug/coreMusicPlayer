# coreMusicPlayer

高性能跨平台音频播放器。

## 项目结构

```
coreMusicPlayer/
├── CMakeLists.txt          # CMake构建配置文件
├── README.md               # 项目说明文档
├── REQUIREMENTS/           # 需求和任务清单
│   └── TASK.md             # 开发任务清单
├── include/                # 头文件目录
│   ├── core/               # 核心模块头文件
│   ├── audio/              # 音频处理模块头文件
│   ├── utils/              # 工具类头文件
│   └── platform/           # 平台相关头文件
├── src/                    # 源代码目录
│   ├── core/               # 核心模块源文件
│   ├── audio/              # 音频处理模块源文件
│   ├── utils/              # 工具类源文件
│   ├── foobar/             # Foobar2000集成模块源文件
│   └── platform/           # 平台相关源文件
├── config/                 # 配置文件目录
│   └── default.json        # 默认配置文件
└── tests/                  # 测试代码目录
```

## 构建说明

### Windows (Visual Studio)

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Linux/macOS (GCC/Clang)

```bash
mkdir build
cd build
cmake ..
make
```

## 开发任务进度

本项目按照以下阶段进行开发：

1. **基础设施** - 完成基础架构搭建（已完成）
2. **音频引擎** - 实现核心音频处理功能（已完成）
3. **播放器策略** - 实现不同模式的播放策略（已完成）
4. **高级功能** - 添加DSP效果、可视化等功能（已完成）
5. **用户界面** - 开发图形用户界面（已完成）
6. **插件系统集成** - 集成Foobar2000插件系统（已完成）
7. **测试优化** - 完善测试和质量保证（已完成）

## 已完成的主要功能

- 核心音频处理引擎
- 多种播放策略支持（Legacy, Modern, Realtime等）
- 数字音量控制和均衡器
- 音频效果器
- Qt6图形用户界面
- 主题管理系统
- 跨平台兼容性支持
- 完整的测试套件
- Foobar2000插件系统集成

## Foobar2000插件系统集成

本项目实现了对Foobar2000插件系统的完整集成，包括：

- 插件管理器：负责加载、卸载和管理Foobar2000插件
- 输入适配器：将Foobar2000解码器封装为标准接口
- DSP处理器适配器：将Foobar2000效果器封装为标准接口
- 输出适配器：将Foobar2000输出插件封装为标准接口

## 贡献者

- 系统架构师
- 音频开发者A
- 音频开发者B
- GUI开发者A
- GUI开发者B
- 测试工程师
- DevOps工程师
- 文档工程师

## 许可证

MIT License