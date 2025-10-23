#include "micropython_engine.h"
#include <iostream>
#include <string>

/**
 * Basic MicroPython Engine Usage Example
 */
int main() {
    std::cout << "=== MicroPython C++ Integration Example ===" << std::endl;
    
    try {
        // Create engine instance
        MicroPythonEngine engine;
        
        // Configure engine
        MicroPythonConfig config;
        config.heap_size = 128 * 1024;  // 128KB heap
        config.enable_gc = true;
        config.enable_repl = false;
        
        // Initialize engine
        std::cout << "\n1. Initializing MicroPython engine..." << std::endl;
        if (!engine.initialize(config)) {
            std::cerr << "Failed to initialize engine: " << engine.getLastError() << std::endl;
            return -1;
        }
        
        std::cout << "Engine initialized successfully!" << std::endl;
        std::cout << "Heap size: " << engine.getHeapSize() << " bytes" << std::endl;
        
        // Execute simple Python code
        std::cout << "\n2. Executing Python code..." << std::endl;
        
        // Test basic print statement
        std::string code1 = "print(\"Hello from MicroPython!\")";
        if (engine.executeString(code1)) {
            std::cout << "Code executed successfully!" << std::endl;
        } else {
            std::cerr << "Execution failed: " << engine.getLastError() << std::endl;
        }
        
        // Test variable assignment and arithmetic
        std::string code2 = R"(
x = 10
y = 20
result = x + y
print("Result: " + str(result))
)";
        
        std::cout << "\n3. Executing more complex code..." << std::endl;
        if (engine.executeString(code2)) {
            std::cout << "Complex code executed successfully!" << std::endl;
        } else {
            std::cerr << "Execution failed: " << engine.getLastError() << std::endl;
        }
        
        // Test memory usage
        std::cout << "\n4. Memory statistics..." << std::endl;
        std::cout << "Memory usage: " << engine.getMemoryUsage() << " bytes" << std::endl;
        std::cout << "Heap size: " << engine.getHeapSize() << " bytes" << std::endl;
        
        // Force garbage collection
        std::cout << "\n5. Running garbage collection..." << std::endl;
        engine.collectGarbage();
        std::cout << "Memory usage after GC: " << engine.getMemoryUsage() << " bytes" << std::endl;
        
        // Test error handling
        std::cout << "\n6. Testing error handling..." << std::endl;
        std::string invalid_code = "invalid_syntax_here +++";
        if (!engine.executeString(invalid_code)) {
            std::cout << "Error correctly caught: " << engine.getLastError() << std::endl;
        }
        
        // Engine will be automatically shut down when destructor is called
        std::cout << "\n7. Engine will shutdown automatically..." << std::endl;
        
    } catch (const MicroPythonException& e) {
        std::cerr << "MicroPython exception: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "\nExample completed successfully!" << std::endl;
    return 0;
}
