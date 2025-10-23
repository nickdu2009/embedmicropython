/* MicroPython configuration for C++ embedding */

// Include common MicroPython embed configuration.
#include <port/mpconfigport_common.h>

// Use a more comprehensive configuration than minimal
#define MICROPY_CONFIG_ROM_LEVEL                (MICROPY_CONFIG_ROM_LEVEL_CORE_FEATURES)

// Enable essential features for C++ integration
#ifndef MICROPY_ENABLE_COMPILER
#define MICROPY_ENABLE_COMPILER                 (1)
#endif
#ifndef MICROPY_ENABLE_GC
#define MICROPY_ENABLE_GC                       (1)
#endif
#define MICROPY_ENABLE_FINALISER                (1)
#define MICROPY_STACK_CHECK                     (1)
#define MICROPY_ENABLE_EMERGENCY_EXCEPTION_BUF  (1)
#define MICROPY_KBD_EXCEPTION                   (1)

// Python core features
#define MICROPY_PY_GC                           (1)
#define MICROPY_PY_SYS                          (1)
#define MICROPY_PY_SYS_MAXSIZE                  (1)
#ifndef MICROPY_PY_SYS_PLATFORM
#define MICROPY_PY_SYS_PLATFORM                 "cpp-embed"
#endif
#define MICROPY_PY_SYS_EXIT                     (1)
#define MICROPY_PY_SYS_STDFILES                 (1)
#define MICROPY_PY_SYS_STDIO_BUFFER             (1)

// Built-in modules
#define MICROPY_PY_BUILTINS_HELP                (1)
#define MICROPY_PY_BUILTINS_HELP_TEXT           cpp_embed_help_text
#define MICROPY_PY_BUILTINS_INPUT               (1)
#define MICROPY_PY_BUILTINS_POW3                (1)
#define MICROPY_PY_BUILTINS_ROUND_INT           (1)
#define MICROPY_PY_BUILTINS_TIMEOUTERROR        (1)
#define MICROPY_PY_BUILTINS_MEMORYVIEW          (1)
#define MICROPY_PY_BUILTINS_SET                 (1)
#define MICROPY_PY_BUILTINS_SLICE               (1)
#define MICROPY_PY_BUILTINS_PROPERTY            (1)
#define MICROPY_PY_BUILTINS_MIN_MAX             (1)
#define MICROPY_PY_BUILTINS_FILTER              (1)
#define MICROPY_PY_BUILTINS_MAP                 (1)
#define MICROPY_PY_BUILTINS_ENUMERATE           (1)
#define MICROPY_PY_BUILTINS_REVERSED            (1)
#define MICROPY_PY_BUILTINS_NOTIMPLEMENTED      (1)

// Collections module
#define MICROPY_PY_COLLECTIONS                  (1)
#define MICROPY_PY_COLLECTIONS_DEQUE            (1)
#define MICROPY_PY_COLLECTIONS_ORDEREDDICT      (1)

// Math module
#define MICROPY_PY_MATH                         (1)
#define MICROPY_PY_MATH_SPECIAL_FUNCTIONS       (1)

// IO module
#define MICROPY_PY_IO                           (1)
#define MICROPY_PY_IO_IOBASE                    (1)
#define MICROPY_PY_IO_FILEIO                    (1)
#define MICROPY_PY_IO_BYTESIO                   (1)
#define MICROPY_PY_IO_BUFFEREDWRITER            (1)

// String operations
#define MICROPY_PY_BUILTINS_STR_UNICODE         (1)
#define MICROPY_PY_BUILTINS_STR_CENTER          (1)
#define MICROPY_PY_BUILTINS_STR_PARTITION       (1)
#define MICROPY_PY_BUILTINS_STR_SPLITLINES      (1)

// Exception handling
#define MICROPY_PY_BUILTINS_COMPILE             (1)
#define MICROPY_PY_BUILTINS_EXECFILE            (0)
#define MICROPY_PY_MICROPYTHON_MEM_INFO         (1)

// Memory and performance settings
#define MICROPY_ALLOC_PATH_MAX                  (256)
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_PERSISTENT_CODE_SAVE            (1)

// C++ integration specific
#define MICROPY_ENABLE_EXTERNAL_IMPORT          (1)
#define MICROPY_READER_POSIX                    (1)

// Platform specific (only define if not already set)
#ifndef MICROPY_PY_SYS_PLATFORM
#if defined(__APPLE__)
    #define MICROPY_PY_SYS_PLATFORM             "darwin"
#elif defined(__linux__)
    #define MICROPY_PY_SYS_PLATFORM             "linux"
#elif defined(_WIN32)
    #define MICROPY_PY_SYS_PLATFORM             "win32"
#else
    #define MICROPY_PY_SYS_PLATFORM             "cpp-embed"
#endif
#endif

// Help text
extern const char cpp_embed_help_text[];

// Memory allocation functions
#define MICROPY_MEM_STATS                       (1)

// Error reporting
#define MICROPY_ERROR_REPORTING                 (MICROPY_ERROR_REPORTING_DETAILED)

// Float precision
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_DOUBLE)

// Enable debugging features in debug builds
#ifdef DEBUG
    #define MICROPY_DEBUG_VERBOSE               (1)
    #define MICROPY_DEBUG_PRINTER               (&mp_debug_print)
#endif
