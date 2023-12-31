#ifndef __STACK_ARRAY_H__
#define __STACK_ARRAY_H__

#include <stdlib.h>
#include <stdbool.h>
#include "log.h"
#include "memory_watcher.h"

#define DEFAULT_BUF_SIZE 4

typedef struct {
    void **buf;
    void **end;
    size_t size;
    bool limit;
    memwatch_t *mem;
} stack_array_t;

stack_array_t *create_stack_array(size_t size, memwatch_t *mem);
void free_stack_array(stack_array_t *stack);
int stack_array_push(stack_array_t *stack, void *data);
void *stack_array_pop(stack_array_t *stack);


#endif // __STACK_ARRAY_H__
