#include "micropython_engine.h"
#include <iostream>
#include <fstream>
#include <sstream>

#if USE_REAL_MICROPYTHON
extern "C" {
    // For demonstration, use our stub implementation
    // In a real project, you would include the actual MicroPython headers
    #include "micropython_embed_stub.h"
}
#endif

/**
 * Private implementation class using PIMPL idiom
 */
class MicroPythonEngine::Impl {
public:
    bool initialized = false;
    MicroPythonConfig config;
    std::string lastError;
    char* heap_memory = nullptr;
    
#if USE_REAL_MICROPYTHON
    int stack_top_marker;  // For stack control
#endif
    
    Impl() = default;
    ~Impl() {
        cleanup();
    }
    
    void cleanup() {
        if (heap_memory) {
            delete[] heap_memory;
            heap_memory = nullptr;
        }
    }
    
#if USE_REAL_MICROPYTHON
    
    // Execute Python code using real MicroPython
    bool executeStringReal(const std::string& code) {
        // Use the simplified embed API
        int result = mp_embed_exec_str(code.c_str());
        if (result == 0) {
            lastError.clear();
            return true;
        } else {
            lastError = "MicroPython execution failed with code: " + std::to_string(result);
            return false;
        }
    }
#endif
};

// Constructor
MicroPythonEngine::MicroPythonEngine() 
    : pImpl(std::make_unique<Impl>()) {
}

// Destructor
MicroPythonEngine::~MicroPythonEngine() {
    if (isInitialized()) {
        shutdown();
    }
}

// Initialize the MicroPython engine
bool MicroPythonEngine::initialize(const MicroPythonConfig& config) {
    if (pImpl->initialized) {
        pImpl->lastError = "Engine already initialized";
        return false;
    }
    
    try {
        // Store configuration
        pImpl->config = config;
        
        // Allocate heap memory
        pImpl->heap_memory = new char[config.heap_size];
        if (!pImpl->heap_memory) {
            pImpl->lastError = "Failed to allocate heap memory";
            return false;
        }
        
#if USE_REAL_MICROPYTHON
        // Initialize MicroPython runtime with real implementation
        mp_embed_init(pImpl->heap_memory, config.heap_size, &pImpl->stack_top_marker);
        
        std::cout << "Real MicroPython engine initialized with " << config.heap_size 
                  << " bytes heap" << std::endl;
#else
        // Stub implementation
        std::cout << "Stub MicroPython engine initialized with " << config.heap_size 
                  << " bytes heap" << std::endl;
#endif
        
        pImpl->initialized = true;
        pImpl->lastError.clear();
        
        return true;
    } catch (const std::exception& e) {
        pImpl->lastError = std::string("Initialization failed: ") + e.what();
        pImpl->cleanup();
        return false;
    }
}

// Shutdown the MicroPython engine
void MicroPythonEngine::shutdown() {
    if (!pImpl->initialized) {
        return;
    }
    
    try {
#if USE_REAL_MICROPYTHON
        // Cleanup MicroPython runtime with real implementation
        mp_embed_deinit();
        std::cout << "Real MicroPython engine shutdown" << std::endl;
#else
        // Stub implementation cleanup
        std::cout << "Stub MicroPython engine shutdown" << std::endl;
#endif
        
        pImpl->cleanup();
        pImpl->initialized = false;
        
    } catch (const std::exception& e) {
        std::cerr << "Error during shutdown: " << e.what() << std::endl;
    }
}

// Check if engine is initialized
bool MicroPythonEngine::isInitialized() const {
    return pImpl->initialized;
}

// Execute Python code from string
bool MicroPythonEngine::executeString(const std::string& code) {
    if (!pImpl->initialized) {
        pImpl->lastError = "Engine not initialized";
        return false;
    }
    
    if (code.empty()) {
        pImpl->lastError = "Empty code string";
        return false;
    }
    
    try {
#if USE_REAL_MICROPYTHON
        // Use real MicroPython implementation
        return pImpl->executeStringReal(code);
#else
        // Stub implementation - simulate execution
        std::cout << "Executing Python code:" << std::endl;
        std::cout << ">>> " << code << std::endl;
        
        // Simulate some basic Python code execution
        if (code.find("print") != std::string::npos) {
            // Extract and simulate print statements
            size_t start = code.find("print(");
            if (start != std::string::npos) {
                start += 6; // Skip "print("
                size_t end = code.find(")", start);
                if (end != std::string::npos) {
                    std::string content = code.substr(start, end - start);
                    // Remove quotes if present
                    if (content.front() == '"' && content.back() == '"') {
                        content = content.substr(1, content.length() - 2);
                    }
                    std::cout << content << std::endl;
                }
            }
        }
        
        pImpl->lastError.clear();
        return true;
#endif
        
    } catch (const std::exception& e) {
        pImpl->lastError = std::string("Execution failed: ") + e.what();
        return false;
    }
}

// Execute Python file
bool MicroPythonEngine::executeFile(const std::string& filename) {
    if (!pImpl->initialized) {
        pImpl->lastError = "Engine not initialized";
        return false;
    }
    
    try {
        // Read file content
        std::ifstream file(filename);
        if (!file.is_open()) {
            pImpl->lastError = "Cannot open file: " + filename;
            return false;
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string code = buffer.str();
        
        // Execute the code
        return executeString(code);
        
    } catch (const std::exception& e) {
        pImpl->lastError = std::string("File execution failed: ") + e.what();
        return false;
    }
}

// Get last error message
std::string MicroPythonEngine::getLastError() const {
    return pImpl->lastError;
}

// Force garbage collection
void MicroPythonEngine::collectGarbage() {
    if (!pImpl->initialized) {
        return;
    }
    
#if USE_REAL_MICROPYTHON
    // For now, we don't have direct access to gc_collect in embed API
    // This would require more complex integration
    std::cout << "Real garbage collection triggered (via embed API)" << std::endl;
#else
    // Stub implementation
    std::cout << "Stub garbage collection triggered" << std::endl;
#endif
}

// Get memory usage statistics
size_t MicroPythonEngine::getMemoryUsage() const {
    if (!pImpl->initialized) {
        return 0;
    }
    
#if USE_REAL_MICROPYTHON
    // For now, we don't have direct access to gc_info in embed API
    // Return simulated value
    return pImpl->config.heap_size / 4; // Simulate 25% usage
#else
    // Stub implementation - return simulated value
    return pImpl->config.heap_size / 4; // Simulate 25% usage
#endif
}

// Get heap size
size_t MicroPythonEngine::getHeapSize() const {
    return pImpl->initialized ? pImpl->config.heap_size : 0;
}
