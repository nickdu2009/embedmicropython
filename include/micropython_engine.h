#ifndef MICROPYTHON_ENGINE_H
#define MICROPYTHON_ENGINE_H

#include <string>
#include <memory>
#include <stdexcept>

/**
 * MicroPython Engine Exception Class
 */
class MicroPythonException : public std::runtime_error {
public:
    explicit MicroPythonException(const std::string& message) 
        : std::runtime_error(message) {}
};

/**
 * MicroPython Engine Configuration
 */
struct MicroPythonConfig {
    size_t heap_size = 64 * 1024;  // Default 64KB heap
    bool enable_gc = true;          // Enable garbage collection
    bool enable_repl = false;       // Enable REPL mode
    std::string script_path = "";   // Path to Python scripts
};

/**
 * MicroPython Engine Wrapper Class
 * Provides C++ interface for embedding MicroPython
 */
class MicroPythonEngine {
public:
    /**
     * Constructor
     */
    MicroPythonEngine();
    
    /**
     * Destructor
     */
    ~MicroPythonEngine();
    
    /**
     * Initialize the MicroPython engine
     * @param config Configuration parameters
     * @return true if successful, false otherwise
     */
    bool initialize(const MicroPythonConfig& config = MicroPythonConfig());
    
    /**
     * Shutdown the MicroPython engine
     */
    void shutdown();
    
    /**
     * Check if engine is initialized
     * @return true if initialized, false otherwise
     */
    bool isInitialized() const;
    
    /**
     * Execute Python code from string
     * @param code Python code to execute
     * @return true if successful, false otherwise
     */
    bool executeString(const std::string& code);
    
    /**
     * Execute Python file
     * @param filename Path to Python file
     * @return true if successful, false otherwise
     */
    bool executeFile(const std::string& filename);
    
    /**
     * Get last error message
     * @return Error message string
     */
    std::string getLastError() const;
    
    /**
     * Force garbage collection
     */
    void collectGarbage();
    
    /**
     * Get memory usage statistics
     * @return Memory usage in bytes
     */
    size_t getMemoryUsage() const;
    
    /**
     * Get heap size
     * @return Heap size in bytes
     */
    size_t getHeapSize() const;

private:
    // Private implementation details
    class Impl;
    std::unique_ptr<Impl> pImpl;
    
    // Non-copyable
    MicroPythonEngine(const MicroPythonEngine&) = delete;
    MicroPythonEngine& operator=(const MicroPythonEngine&) = delete;
};

#endif // MICROPYTHON_ENGINE_H
