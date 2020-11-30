#ifndef __QUEUE_ARRAY_H__
#define __QUEUE_ARRAY_H__

#include <stdlib.h>
#include "memory_watcher.h"
#include "log.h"

#define DEFAULT_BUF_SIZE 4

typedef struct {
    void **buf;
    int size;
    void **first;
    void **end;
    memwatch_t *mem;
} queue_array_t;

queue_array_t *create_queue_array(memwatch_t *mem);
void free_queue_array(queue_array_t *queue);
int push_queue_array(queue_array_t *queue, void *data);
void *pop_queue_array(queue_array_t *queue);

#endif // __QUEUE_ARRAY_H__
