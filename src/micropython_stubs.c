/*
 * MicroPython C API Stub Implementations
 * 
 * This file provides stub implementations of the MicroPython C API
 * for demonstration purposes. In a real implementation, you would
 * link against the actual MicroPython library.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Stub implementations of MicroPython API functions

int mp_embed_init(void *heap, size_t heap_size, void *stack_top) {
    printf("MicroPython stub: mp_embed_init called with heap_size=%zu\n", heap_size);
    return 0;
}

void mp_embed_deinit(void) {
    printf("MicroPython stub: mp_embed_deinit called\n");
}

int mp_embed_exec_str(const char *code) {
    printf("MicroPython stub: executing code:\n%s\n", code);
    
    // Simple simulation of Python print statements
    if (strstr(code, "print(") != NULL) {
        const char *start = strstr(code, "print(");
        if (start) {
            start += 6; // Skip "print("
            const char *end = strchr(start, ')');
            if (end) {
                // Extract content between parentheses
                size_t len = end - start;
                char *content = malloc(len + 1);
                if (content) {
                    strncpy(content, start, len);
                    content[len] = '\0';
                    
                    // Remove quotes if present
                    if (content[0] == '"' && content[len-1] == '"') {
                        content[len-1] = '\0';
                        printf("%s\n", content + 1);
                    } else {
                        printf("%s\n", content);
                    }
                    free(content);
                }
            }
        }
    }
    
    // Simulate other Python features
    if (strstr(code, "import")) {
        printf("MicroPython stub: import statement detected\n");
    }
    
    if (strstr(code, "for ") && strstr(code, "range(")) {
        printf("MicroPython stub: for loop with range detected\n");
    }
    
    if (strstr(code, "def ")) {
        printf("MicroPython stub: function definition detected\n");
    }
    
    return 0; // Success
}
