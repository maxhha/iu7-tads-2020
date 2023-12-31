#include "../inc/stack_array.h"

stack_array_t *create_stack_array(size_t size, memwatch_t *mem)
{
    stack_array_t *s = wmalloc(mem, MEMPTR_STACK, sizeof(stack_array_t));

    if (s == NULL)
    {
        return NULL;
    }

    s->size = size == 0 ? DEFAULT_BUF_SIZE : size;
    s->limit = size != 0;
    s->mem = mem;

    s->buf = wmalloc(mem, MEMPTR_STACK, s->size * sizeof(*s->buf));

    if (s->buf == NULL)
    {
        free(s);
        return NULL;
    }

    s->end = s->buf;

    return s;
}

void free_stack_array(stack_array_t *stack)
{
    wfree(stack->mem, stack->buf);
    wfree(stack->mem, stack);
}

int stack_array_push(stack_array_t *stack, void *data)
{
    if (stack->end == stack->buf + stack->size)
    {
        if (stack->limit)
            return -1;

        size_t new_size = stack->size * 2;
        void **tmp = wrealloc(stack->mem, stack->buf, new_size * sizeof(*stack->buf));

        if (tmp == NULL)
            return -1;

        stack->end = tmp + stack->size;
        stack->buf = tmp;
        stack->size = new_size;
    }

    *stack->end = data;
    stack->end++;

    return 0;
}

void *stack_array_pop(stack_array_t *stack)
{
    if (stack->end == stack->buf)
        return NULL;

    stack->end--;
    void *data = *stack->end;

    return data;
}
