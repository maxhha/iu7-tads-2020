#ifndef __QUEUE_LIST_H__
#define __QUEUE_LIST_H__

#include <stdlib.h>
#include "memory_watcher.h"
#include "log.h"

typedef struct list_s list_t;

struct list_s {
    void *data;
    list_t *next;
};

typedef struct {
    list_t *last;
    list_t *first;
    memwatch_t *mem;
} queue_list_t;

queue_list_t *create_queue_list(memwatch_t *mem);
void free_queue_list(queue_list_t *queue);
int push_queue_list(queue_list_t *queue, void *data);
void *pop_queue_list(queue_list_t *queue);

#endif // __QUEUE_LIST_H__
