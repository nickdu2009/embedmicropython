#include "micropython_engine.h"
#include <iostream>

/**
 * File Execution Example
 */
int main() {
    std::cout << "=== MicroPython File Execution Example ===" << std::endl;
    
    try {
        // Create and initialize engine
        MicroPythonEngine engine;
        MicroPythonConfig config;
        config.heap_size = 64 * 1024;  // 64KB heap
        
        if (!engine.initialize(config)) {
            std::cerr << "Failed to initialize engine: " << engine.getLastError() << std::endl;
            return -1;
        }
        
        std::cout << "Engine initialized successfully!" << std::endl;
        
        // Execute Python file
        std::string filename = "examples/test_script.py";
        std::cout << "\nExecuting Python file: " << filename << std::endl;
        
        if (engine.executeFile(filename)) {
            std::cout << "\nFile executed successfully!" << std::endl;
        } else {
            std::cerr << "File execution failed: " << engine.getLastError() << std::endl;
        }
        
        // Show memory usage
        std::cout << "\nMemory usage: " << engine.getMemoryUsage() << " bytes" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}
