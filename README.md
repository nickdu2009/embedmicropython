# MicroPython C++ Integration

一个用于在 C++ 应用程序中嵌入 MicroPython 脚本引擎的完整解决方案。

## 项目结构

```
embedmicropython/
├── include/                    # 头文件
│   └── micropython_engine.h   # MicroPython 引擎接口
├── src/                        # 源文件
│   └── micropython_engine.cpp # MicroPython 引擎实现
├── examples/                   # 示例代码
│   ├── basic_example.cpp       # 基础使用示例
│   ├── file_example.cpp        # 文件执行示例
│   └── test_script.py          # 测试 Python 脚本
├── CMakeLists.txt              # CMake 构建配置
├── Makefile                    # 简化构建命令
├── README.md                   # 项目文档
└── micropython_cpp_integration_plan.md  # 功能规划文档
```

## 功能特性

### 已实现的核心功能

- ✅ **引擎初始化/销毁**：完整的生命周期管理
- ✅ **脚本执行**：支持字符串代码和文件执行
- ✅ **错误处理**：完善的异常捕获和错误信息
- ✅ **内存管理**：可配置的堆大小和垃圾回收

### 核心类和接口

#### MicroPythonEngine 类
主要的引擎包装类，提供以下接口：

```cpp
class MicroPythonEngine {
public:
    // 初始化和销毁
    bool initialize(const MicroPythonConfig& config = MicroPythonConfig());
    void shutdown();
    bool isInitialized() const;
    
    // 脚本执行
    bool executeString(const std::string& code);
    bool executeFile(const std::string& filename);
    
    // 错误处理
    std::string getLastError() const;
    
    // 内存管理
    void collectGarbage();
    size_t getMemoryUsage() const;
    size_t getHeapSize() const;
};
```

#### MicroPythonConfig 结构
引擎配置参数：

```cpp
struct MicroPythonConfig {
    size_t heap_size = 64 * 1024;  // 堆大小（默认64KB）
    bool enable_gc = true;          // 启用垃圾回收
    bool enable_repl = false;       // 启用REPL模式
    std::string script_path = "";   // Python脚本路径
};
```

## 构建和使用

### 系统要求
- C++17 或更高版本
- CMake 3.12 或更高版本
- 支持的编译器：GCC, Clang, MSVC

### 构建项目

#### 使用 Makefile（推荐）
```bash
# 构建项目
make build

# 调试模式构建
make debug

# 发布模式构建
make release

# 运行示例
make run-all

# 清理构建
make clean
```

#### 使用 CMake
```bash
mkdir build
cd build
cmake ..
make
```

### 运行示例

#### 基础示例
```bash
./build/basic_example
```

#### 文件执行示例
```bash
./build/file_example
```

## 使用示例

### 基础用法

```cpp
#include "micropython_engine.h"
#include <iostream>

int main() {
    // 创建引擎实例
    MicroPythonEngine engine;
    
    // 配置引擎
    MicroPythonConfig config;
    config.heap_size = 128 * 1024;  // 128KB
    
    // 初始化
    if (!engine.initialize(config)) {
        std::cerr << "初始化失败: " << engine.getLastError() << std::endl;
        return -1;
    }
    
    // 执行 Python 代码
    std::string code = "print('Hello from MicroPython!')";
    if (engine.executeString(code)) {
        std::cout << "执行成功!" << std::endl;
    } else {
        std::cerr << "执行失败: " << engine.getLastError() << std::endl;
    }
    
    return 0;
}
```

### 文件执行

```cpp
// 执行 Python 文件
if (engine.executeFile("script.py")) {
    std::cout << "文件执行成功!" << std::endl;
} else {
    std::cerr << "文件执行失败: " << engine.getLastError() << std::endl;
}
```

### 内存管理

```cpp
// 查看内存使用情况
std::cout << "内存使用: " << engine.getMemoryUsage() << " bytes" << std::endl;
std::cout << "堆大小: " << engine.getHeapSize() << " bytes" << std::endl;

// 强制垃圾回收
engine.collectGarbage();
```

## 当前实现状态

### ✅ 已完成的功能
- 核心引擎集成框架
- 基础数据类型处理
- 脚本执行机制
- 错误处理和异常管理
- 内存管理配置
- 完整的示例代码
- 构建系统配置

### 🔄 下一步开发计划
根据 `micropython_cpp_integration_plan.md` 中的规划：

1. **数据类型转换**：实现 C++ 和 Python 间的数据转换
2. **函数调用**：支持 C++ 调用 Python 函数，反之亦然
3. **模块系统**：注册自定义 C++ 模块到 Python
4. **性能优化**：预编译缓存、多实例支持
5. **调试支持**：断点调试、变量监控
6. **安全机制**：沙箱环境、资源限制

## 注意事项

### 当前限制
- 这是一个演示版本，使用模拟的 MicroPython 实现
- 要使用真实的 MicroPython，需要设置 `USE_REAL_MICROPYTHON=ON`
- 某些高级功能需要实际的 MicroPython 源码集成

### 真实 MicroPython 集成
要集成真实的 MicroPython：

1. 下载 MicroPython 源码
2. 配置构建选项：`cmake -DUSE_REAL_MICROPYTHON=ON ..`
3. 修改源码以包含实际的 MicroPython API 调用

## 贡献

欢迎提交 Issue 和 Pull Request 来改进这个项目！

## 许可证

MIT License - 详见 LICENSE 文件
