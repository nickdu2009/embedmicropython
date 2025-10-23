cmake_minimum_required(VERSION 3.12)
project(MicroPythonCppIntegration VERSION 1.0.0 LANGUAGES CXX)

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
    # Here you would add the actual MicroPython source compilation
    # This would involve:
    # - Downloading MicroPython source
    # - Configuring MicroPython build
    # - Linking with MicroPython libraries
    
    # Example (would need actual MicroPython source):
    # add_subdirectory(micropython)
    # target_link_libraries(micropython_engine micropython_core)
    
else()
    message(STATUS "Using stub MicroPython implementation for demonstration")
endif()
