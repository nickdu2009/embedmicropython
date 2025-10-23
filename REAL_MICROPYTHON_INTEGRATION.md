# 真实 MicroPython 集成指南

本文档说明如何将真实的 MicroPython 源码集成到 C++ 项目中。

## 当前实现状态

### ✅ 已完成的集成工作

1. **项目结构设置**：创建了支持真实 MicroPython 的项目结构
2. **构建系统配置**：CMake 支持 `USE_REAL_MICROPYTHON` 选项
3. **API 接口设计**：定义了完整的 C++ 包装接口
4. **存根实现**：创建了 MicroPython API 的演示存根
5. **测试验证**：验证了集成框架的正确性

### 🔧 存根实现详情

当前使用存根实现演示真实集成的架构：

- **`src/micropython_stubs.c`**：MicroPython C API 的存根实现
- **`src/micropython_embed_stub.h`**：API 声明头文件
- **模拟功能**：基本的 Python 代码执行模拟

## 集成真实 MicroPython 的步骤

### 1. 准备 MicroPython 源码

```bash
# MicroPython 源码已下载到 micropython/ 目录
cd micropython/examples/embedding
make -f micropython_embed.mk
```

这将生成 `micropython_embed/` 目录，包含嵌入所需的所有文件。

### 2. 替换存根实现

要使用真实的 MicroPython，需要：

#### 修改 CMakeLists.txt

```cmake
# 替换存根实现部分
if(USE_REAL_MICROPYTHON)
    # 使用真实的 MicroPython 源文件
    file(GLOB_RECURSE MICROPYTHON_SOURCES
        ${MICROPYTHON_EMBED_DIR}/py/*.c
        ${MICROPYTHON_EMBED_DIR}/port/*.c
        ${MICROPYTHON_EMBED_DIR}/shared/*.c
        # 注意：extmod 可能需要额外的依赖
    )
    
    target_sources(micropython_engine PRIVATE ${MICROPYTHON_SOURCES})
    set_source_files_properties(${MICROPYTHON_SOURCES} PROPERTIES LANGUAGE C)
    
    # 链接数学库
    target_link_libraries(micropython_engine m)
endif()
```

#### 修改头文件包含

在 `src/micropython_engine.cpp` 中：

```cpp
#if USE_REAL_MICROPYTHON
extern "C" {
    #include "port/micropython_embed.h"
    // 根据需要包含其他 MicroPython 头文件
    #include "py/compile.h"
    #include "py/runtime.h"
    #include "py/gc.h"
}
#endif
```

### 3. 实现高级功能

#### 真实的垃圾收集

```cpp
void MicroPythonEngine::collectGarbage() {
#if USE_REAL_MICROPYTHON
    gc_collect();
#endif
}
```

#### 真实的内存统计

```cpp
size_t MicroPythonEngine::getMemoryUsage() const {
#if USE_REAL_MICROPYTHON
    gc_info_t info;
    gc_info(&info);
    return info.used;
#endif
}
```

#### 异常处理

```cpp
bool executeStringReal(const std::string& code) {
    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        // 编译并执行代码
        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR_, code.c_str(), code.length(), 0);
        mp_parse_tree_t parse_tree = mp_parse(lex, MP_PARSE_FILE_INPUT);
        mp_obj_t module_fun = mp_compile(&parse_tree, lex->source_name, true);
        mp_parse_tree_clear(&parse_tree);
        
        mp_call_function_0(module_fun);
        nlr_pop();
        return true;
    } else {
        // 处理异常
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
        return false;
    }
}
```

## 配置选项

### MicroPython 配置文件

- **`micropython_config/mpconfigport.h`**：MicroPython 功能配置
- **`micropython_config/help_text.c`**：帮助文本定义

### 重要配置项

```c
// 核心功能
#define MICROPY_ENABLE_COMPILER                 (1)
#define MICROPY_ENABLE_GC                       (1)

// Python 功能
#define MICROPY_PY_SYS                          (1)
#define MICROPY_PY_BUILTINS_HELP                (1)
#define MICROPY_PY_MATH                         (1)

// 性能设置
#define MICROPY_ALLOC_PATH_MAX                  (256)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_DOUBLE)
```

## 构建和测试

### 构建真实集成

```bash
# 使用真实 MicroPython
cmake -DUSE_REAL_MICROPYTHON=ON -B build
make -C build

# 使用存根实现（默认）
cmake -DUSE_REAL_MICROPYTHON=OFF -B build
make -C build
```

### 运行测试

```bash
# 基础功能测试
./build/basic_example

# 文件执行测试
./build/file_example
```

## 潜在问题和解决方案

### 1. 编译错误

**问题**：MicroPython 源码编译错误
**解决**：
- 检查 MicroPython 版本兼容性
- 确保所有必需的头文件路径正确
- 验证配置文件设置

### 2. 链接错误

**问题**：未定义的符号
**解决**：
- 确保所有必需的源文件都被包含
- 链接必要的系统库（如 libm）
- 检查符号导出设置

### 3. 运行时错误

**问题**：初始化或执行失败
**解决**：
- 验证堆内存配置
- 检查栈大小设置
- 确保正确的异常处理

## 下一步开发

1. **数据类型转换**：实现 C++ 和 Python 对象间的转换
2. **函数绑定**：允许 Python 调用 C++ 函数
3. **模块系统**：注册自定义 C++ 模块
4. **调试支持**：集成调试和性能分析功能

## 参考资源

- [MicroPython 官方文档](https://docs.micropython.org/)
- [MicroPython 嵌入指南](https://github.com/micropython/micropython/tree/master/examples/embedding)
- [MicroPython C API 参考](https://docs.micropython.org/en/latest/develop/cmodules.html)

---

*本指南提供了从存根实现迁移到真实 MicroPython 集成的完整路径。*
