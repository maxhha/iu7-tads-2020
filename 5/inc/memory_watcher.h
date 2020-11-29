#ifndef __MEMORY_WATCHER_H__
#define __MEMORY_WATCHER_H__

#include <stdlib.h>
#include <stdarg.h>
#include "log.h"

#define MEMPTR_PREV 0
#define MEMPTR_UNUSED ' '
#define MEMPTR_USED '.'
#define MEMPTR_WATCHER 'W'

#define MEMPTR_QUEUE 'Q'
#define MEMPTR_DATA 'D'


#define MEMORY_BATCH_SIZE 128

typedef struct memwatch_s memwatch_t;

memwatch_t *create_memory_watch(void);
void free_memory_watch(memwatch_t *w);

void *wmalloc(memwatch_t *w, char type, size_t size);
void *wrealloc(memwatch_t *w, void *ptr, size_t new_size);
void wfree(memwatch_t *w, void *ptr);

#endif // __MEMORY_WATCHER_H__
