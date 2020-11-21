#ifndef __STACK_LIST_H__
#define __STACK_LIST_H__

#include <stdlib.h>
#include <stdbool.h>
#include "log.h"
#include "memory_watcher.h"

typedef struct list_s list_t;

struct list_s {
    list_t *next;
    void *data;
};

typedef struct {
    list_t *head;
    size_t size;
    size_t n_elems;
    bool limit;
    memwatch_t *mem;
} stack_list_t;

stack_list_t *create_stack_list(size_t size, memwatch_t *mem);
void free_stack_list(stack_list_t *stack);
int stack_list_push(stack_list_t *stack, void *data);
void *stack_list_pop(stack_list_t *stack);

#endif // __STACK_LIST_H__
