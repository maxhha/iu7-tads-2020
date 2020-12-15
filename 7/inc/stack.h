#ifndef __STACK_ARRAY_H__
#define __STACK_ARRAY_H__

#include <stdlib.h>
#include <stdbool.h>
#include "log.h"

#define DEFAULT_BUF_SIZE 4

typedef struct {
    void **buf;
    void **end;
    size_t size;
} stack_t;

stack_t *create_stack(size_t size);
void free_stack(stack_t *stack);
int stack_push(stack_t *stack, void *data);
void *stack_pop(stack_t *stack);


#endif // __STACK_ARRAY_H__
