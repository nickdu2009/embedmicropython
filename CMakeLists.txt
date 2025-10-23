cmake_minimum_required(VERSION 3.12)
project(MicroPythonCppIntegration VERSION 1.0.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/micropython_engine.cpp
)

# Create static library
add_library(micropython_engine STATIC ${SOURCES})

# Set library properties
set_target_properties(micropython_engine PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# Include directories for the library
target_include_directories(micropython_engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Example executables
add_executable(basic_example examples/basic_example.cpp)
target_link_libraries(basic_example micropython_engine)

add_executable(file_example examples/file_example.cpp)
target_link_libraries(file_example micropython_engine)

# Install rules
install(TARGETS micropython_engine
    EXPORT MicroPythonEngineTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

install(EXPORT MicroPythonEngineTargets
    FILE MicroPythonEngineTargets.cmake
    NAMESPACE MicroPythonEngine::
    DESTINATION lib/cmake/MicroPythonEngine
)

# Create config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    MicroPythonEngineConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MicroPythonEngineConfigVersion.cmake
    DESTINATION lib/cmake/MicroPythonEngine
)

# Create a simple config file
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/MicroPythonEngineConfig.cmake
"include(\${CMAKE_CURRENT_LIST_DIR}/MicroPythonEngineTargets.cmake)
")

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/MicroPythonEngineConfig.cmake
    DESTINATION lib/cmake/MicroPythonEngine
)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")

# Option to build with actual MicroPython
option(USE_REAL_MICROPYTHON "Use real MicroPython implementation" OFF)

if(USE_REAL_MICROPYTHON)
    message(STATUS "Using real MicroPython implementation")
    
    # Check if MicroPython embed directory exists
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/micropython_embed)
        message(FATAL_ERROR "MicroPython embed directory not found. Please run: cd micropython/examples/embedding && make -f micropython_embed.mk")
    endif()
    
    # Add MicroPython include directories
    set(MICROPYTHON_EMBED_DIR ${CMAKE_SOURCE_DIR}/micropython_embed)
    
    target_include_directories(micropython_engine PRIVATE
        ${MICROPYTHON_EMBED_DIR}
        ${MICROPYTHON_EMBED_DIR}/py
        ${MICROPYTHON_EMBED_DIR}/port
        ${MICROPYTHON_EMBED_DIR}/genhdr
        ${MICROPYTHON_EMBED_DIR}/shared
        ${MICROPYTHON_EMBED_DIR}/extmod
        ${CMAKE_SOURCE_DIR}/micropython_config
    )
    
    # For demonstration purposes, we'll create stub implementations
    # In a real project, you would link against a pre-built MicroPython library
    message(STATUS "Creating MicroPython stub implementations for demonstration")
    
    # Add our stub implementation file
    target_sources(micropython_engine PRIVATE 
        ${CMAKE_SOURCE_DIR}/src/micropython_stubs.c
    )
    
    # Set C language for stub file
    set_source_files_properties(${CMAKE_SOURCE_DIR}/src/micropython_stubs.c PROPERTIES LANGUAGE C)
    
    # Define preprocessor macros for real MicroPython
    target_compile_definitions(micropython_engine PRIVATE
        USE_REAL_MICROPYTHON=1
        MICROPY_ENABLE_GC=1
        MICROPY_ENABLE_COMPILER=1
    )
    
    # Add math library for floating point operations
    if(UNIX)
        target_link_libraries(micropython_engine m)
    endif()
    
    message(STATUS "MicroPython embed directory: ${MICROPYTHON_EMBED_DIR}")
    message(STATUS "Found ${list_length} MicroPython source files")
    
else()
    message(STATUS "Using stub MicroPython implementation for demonstration")
    target_compile_definitions(micropython_engine PRIVATE
        USE_REAL_MICROPYTHON=0
    )
endif()
