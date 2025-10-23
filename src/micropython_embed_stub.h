/*
 * MicroPython Embed API Stub Header
 * 
 * This header provides declarations for the MicroPython embed API
 * stub implementations.
 */

#ifndef MICROPYTHON_EMBED_STUB_H
#define MICROPYTHON_EMBED_STUB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// MicroPython embed API stub functions
int mp_embed_init(void *heap, size_t heap_size, void *stack_top);
void mp_embed_deinit(void);
int mp_embed_exec_str(const char *code);

#ifdef __cplusplus
}
#endif

#endif // MICROPYTHON_EMBED_STUB_H
