#include "micropython_engine.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <chrono>
#include <regex>
#include <algorithm>
#include <iomanip>
#include <map>

/**
 * Script Execution Example with Code Block Parsing and REPL Display
 * Demonstrates parsing Python code into logical blocks and executing with REPL-style output
 */

// Structure to represent a code block
struct CodeBlock {
    std::string content;
    std::string type;  // "statement", "expression", "function", "class", "loop", "conditional"
    int startLine;
    int endLine;
    
    CodeBlock(const std::string& c, const std::string& t, int start, int end) 
        : content(c), type(t), startLine(start), endLine(end) {}
};

// Helper function to read file content
std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Helper function to split string by lines
std::vector<std::string> splitLines(const std::string& text) {
    std::vector<std::string> lines;
    std::stringstream ss(text);
    std::string line;
    
    while (std::getline(ss, line)) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); // Remove carriage return
        }
        lines.push_back(line);
    }
    
    return lines;
}

// Helper function to trim whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Helper function to get indentation level
int getIndentLevel(const std::string& line) {
    int indent = 0;
    for (char c : line) {
        if (c == ' ') indent++;
        else if (c == '\t') indent += 4;
        else break;
    }
    return indent;
}

// Advanced code block parser
std::vector<CodeBlock> parseCodeBlocks(const std::string& code) {
    std::vector<CodeBlock> blocks;
    std::vector<std::string> lines = splitLines(code);
    
    std::string currentBlock = "";
    std::string blockType = "statement";
    int blockStartLine = 0;
    int currentIndent = 0;
    bool inMultiLineBlock = false;
    
    for (int i = 0; i < lines.size(); i++) {
        std::string line = lines[i];
        std::string trimmedLine = trim(line);
        
        // Skip empty lines and comments when not in a multi-line block
        if (trimmedLine.empty() || trimmedLine[0] == '#') {
            if (!inMultiLineBlock) {
                if (!currentBlock.empty()) {
                    blocks.emplace_back(currentBlock, blockType, blockStartLine, i - 1);
                    currentBlock = "";
                    blockType = "statement";
                }
                continue;
            } else {
                // Include empty lines and comments in multi-line blocks
                currentBlock += line + "\n";
                continue;
            }
        }
        
        int lineIndent = getIndentLevel(line);
        
        // Detect block type based on content
        std::string newBlockType = "statement";
        if (trimmedLine.find("def ") == 0) newBlockType = "function";
        else if (trimmedLine.find("class ") == 0) newBlockType = "class";
        else if (trimmedLine.find("for ") == 0 || trimmedLine.find("while ") == 0) newBlockType = "loop";
        else if (trimmedLine.find("if ") == 0 || trimmedLine.find("elif ") == 0 || trimmedLine.find("else:") == 0) newBlockType = "conditional";
        else if (trimmedLine.find("try:") == 0 || trimmedLine.find("except") == 0 || trimmedLine.find("finally:") == 0) newBlockType = "exception";
        
        // Check if this line starts a new multi-line block
        bool startsMultiLineBlock = (trimmedLine.back() == ':') || 
                                   (newBlockType != "statement");
        
        // If we're starting a new block or the indentation changed significantly
        if (!inMultiLineBlock && (!currentBlock.empty() && 
            (startsMultiLineBlock || newBlockType != blockType || 
             (lineIndent == 0 && currentIndent > 0)))) {
            // Save current block
            blocks.emplace_back(currentBlock, blockType, blockStartLine, i - 1);
            currentBlock = "";
            blockType = newBlockType;
            blockStartLine = i;
        }
        
        // Start new block if current is empty
        if (currentBlock.empty()) {
            blockStartLine = i;
            blockType = newBlockType;
            currentIndent = lineIndent;
        }
        
        // Add line to current block
        currentBlock += line + "\n";
        
        // Update multi-line block status
        if (startsMultiLineBlock) {
            inMultiLineBlock = true;
            currentIndent = lineIndent;
        } else if (inMultiLineBlock && lineIndent <= currentIndent && trimmedLine != "") {
            // End of multi-line block
            inMultiLineBlock = false;
            if (lineIndent == 0) {
                // This line starts a new block
                blocks.emplace_back(currentBlock.substr(0, currentBlock.length() - line.length() - 1), 
                                  blockType, blockStartLine, i - 1);
                currentBlock = line + "\n";
                blockType = newBlockType;
                blockStartLine = i;
                currentIndent = lineIndent;
            }
        }
    }
    
    // Add the last block if not empty
    if (!currentBlock.empty()) {
        blocks.emplace_back(currentBlock, blockType, blockStartLine, lines.size() - 1);
    }
    
    return blocks;
}

// REPL-style display function
void displayREPLPrompt(int blockNumber, const std::string& blockType) {
    std::cout << "\n";
    std::cout << ">>> [Block " << blockNumber << " - " << blockType << "] ";
    std::cout.flush();
}

void displayREPLContinuation() {
    std::cout << "... ";
    std::cout.flush();
}

void displayCodeBlock(const CodeBlock& block, bool showLineNumbers = true) {
    std::vector<std::string> lines = splitLines(block.content);
    
    std::cout << "\n┌─ Executing " << block.type << " (lines " 
              << block.startLine + 1 << "-" << block.endLine + 1 << ") ─┐" << std::endl;
    
    for (int i = 0; i < lines.size(); i++) {
        if (showLineNumbers) {
            std::cout << "│ " << std::setfill(' ') << std::setw(3) 
                      << (block.startLine + i + 1) << " │ " << lines[i] << std::endl;
        } else {
            std::cout << "│     │ " << lines[i] << std::endl;
        }
    }
    
    std::cout << "└" << std::string(50, '-') << "┘" << std::endl;
}

// Helper function to add delay for better visualization
void addDelay(int milliseconds = 800) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}

int main() {
    std::cout << "=== MicroPython Code Block Parser & REPL Execution ===" << std::endl;
    std::cout << "Parsing Python code into logical blocks and executing with REPL-style display" << std::endl;
    
    try {
        // Create and initialize engine
        MicroPythonEngine engine;
        MicroPythonConfig config;
        config.heap_size = 256 * 1024;  // 256KB heap
        config.enable_gc = true;
        config.enable_repl = true;  // Enable REPL mode for better display
        
        std::cout << "\n1. Initializing MicroPython engine..." << std::endl;
        if (!engine.initialize(config)) {
            std::cerr << "Failed to initialize engine: " << engine.getLastError() << std::endl;
            return -1;
        }
        
        std::cout << "✓ Engine initialized successfully!" << std::endl;
        std::cout << "  Heap size: " << engine.getHeapSize() << " bytes" << std::endl;
        std::cout << "  REPL mode: enabled" << std::endl;
        
        // Load and parse Python script
        std::cout << "\n2. Loading and parsing Python script..." << std::endl;
        std::cout << "=" << std::string(60, '=') << std::endl;
        
        std::string scriptContent;
        std::string filename = "examples/test_script.py";
        
        try {
            scriptContent = readFile(filename);
            std::cout << "✓ Loaded script: " << filename << std::endl;
        } catch (const std::exception& e) {
            std::cout << "⚠ Could not load file, using demo script instead" << std::endl;
            
            // Create a comprehensive demo script
            scriptContent = R"(# Demo Script - Various Python Constructs
print('=== MicroPython Code Block Demo ===')

# Basic variables and arithmetic
x = 10
y = 20
result = x + y
print(f'Calculation: {x} + {y} = {result}')

# Function definition
def calculate_square(n):
    """Calculate square of a number"""
    return n * n

def greet(name):
    return f'Hello, {name}!'

# Loop demonstration  
print('\n--- Loop Demo ---')
for i in range(1, 4):
    square = calculate_square(i)
    print(f'{i} squared = {square}')

# Conditional logic
print('\n--- Conditional Demo ---')
test_value = 15
if test_value > 10:
    print('Value is greater than 10')
    if test_value > 20:
        print('And greater than 20')
    else:
        print('But not greater than 20')
else:
    print('Value is 10 or less')

# List operations
print('\n--- List Demo ---')
numbers = [1, 2, 3, 4, 5]
print('Original list:', numbers)
doubled = [x * 2 for x in numbers]
print('Doubled:', doubled)

# String operations
message = greet('MicroPython')
print(f'\nGreeting: {message}')

# Exception handling
print('\n--- Exception Demo ---')
try:
    division_result = 10 / 2
    print(f'10 / 2 = {division_result}')
except ZeroDivisionError:
    print('Cannot divide by zero!')
finally:
    print('Division operation completed')

print('\n=== Demo Script Completed ==='))";
        }
        
        // Parse code into blocks
        std::cout << "\n3. Parsing code into logical blocks..." << std::endl;
        std::vector<CodeBlock> blocks = parseCodeBlocks(scriptContent);
        
        std::cout << "✓ Parsed " << blocks.size() << " code blocks:" << std::endl;
        for (size_t i = 0; i < blocks.size(); i++) {
            std::cout << "  Block " << (i + 1) << ": " << blocks[i].type 
                      << " (lines " << blocks[i].startLine + 1 
                      << "-" << blocks[i].endLine + 1 << ")" << std::endl;
        }
        
        // Execute blocks with REPL-style display
        std::cout << "\n4. Executing code blocks with REPL display..." << std::endl;
        std::cout << "=" << std::string(60, '=') << std::endl;
        
        for (size_t i = 0; i < blocks.size(); i++) {
            const CodeBlock& block = blocks[i];
            
            // Display the code block
            displayCodeBlock(block);
            
            // REPL-style prompt
            displayREPLPrompt(i + 1, block.type);
            addDelay(500);
            
            // Show the code being executed (first few lines)
            std::vector<std::string> lines = splitLines(block.content);
            for (size_t j = 0; j < std::min(lines.size(), size_t(3)); j++) {
                if (j > 0) displayREPLContinuation();
                std::cout << lines[j] << std::endl;
                if (lines.size() > 1) addDelay(200);
            }
            
            if (lines.size() > 3) {
                displayREPLContinuation();
                std::cout << "... (" << (lines.size() - 3) << " more lines)" << std::endl;
            }
            
            // Execute the block
            std::cout << "\n🔄 Executing..." << std::endl;
            addDelay(300);
            
            bool success = engine.executeString(block.content);
            
            if (success) {
                std::cout << "✓ Block executed successfully" << std::endl;
                
                // Show memory usage for complex blocks
                if (block.type == "function" || block.type == "class" || block.type == "loop") {
                    std::cout << "  Memory usage: " << engine.getMemoryUsage() << " bytes" << std::endl;
                }
            } else {
                std::cout << "✗ Execution failed: " << engine.getLastError() << std::endl;
                
                // Ask if we should continue
                std::cout << "  Continuing with next block..." << std::endl;
            }
            
            std::cout << std::string(60, '-') << std::endl;
            addDelay(800);
        }
        
        // Final statistics and cleanup
        std::cout << "\n5. Execution Summary" << std::endl;
        std::cout << "=" << std::string(60, '=') << std::endl;
        std::cout << "Total blocks processed: " << blocks.size() << std::endl;
        
        // Count blocks by type
        std::map<std::string, int> blockCounts;
        for (const auto& block : blocks) {
            blockCounts[block.type]++;
        }
        
        std::cout << "Block type distribution:" << std::endl;
        for (const auto& pair : blockCounts) {
            std::cout << "  " << pair.first << ": " << pair.second << " blocks" << std::endl;
        }
        
        std::cout << "\nMemory statistics:" << std::endl;
        std::cout << "  Current usage: " << engine.getMemoryUsage() << " bytes" << std::endl;
        std::cout << "  Heap size: " << engine.getHeapSize() << " bytes" << std::endl;
        std::cout << "  Utilization: " << std::fixed << std::setprecision(1)
                  << (double)engine.getMemoryUsage() / engine.getHeapSize() * 100 << "%" << std::endl;
        
        // Garbage collection
        std::cout << "\n6. Cleanup..." << std::endl;
        engine.collectGarbage();
        std::cout << "✓ Garbage collection completed" << std::endl;
        std::cout << "  Memory after GC: " << engine.getMemoryUsage() << " bytes" << std::endl;
        
    } catch (const MicroPythonException& e) {
        std::cerr << "MicroPython exception: " << e.what() << std::endl;
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "Standard exception: " << e.what() << std::endl;
        return -1;
    }
    
    std::cout << "\n🎉 Code block parsing and execution completed successfully!" << std::endl;
    return 0;
}
