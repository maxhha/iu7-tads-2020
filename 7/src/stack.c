#include "stack.h"

stack_t *create_stack(size_t size)
{
    stack_t *s = malloc(sizeof(stack_t));

    if (s == NULL)
        return NULL;

    s->size = size == 0 ? DEFAULT_BUF_SIZE : size;
    s->buf = malloc(s->size * sizeof(*s->buf));

    if (s->buf == NULL)
    {
        free(s);
        return NULL;
    }

    s->end = s->buf;

    return s;
}

void free_stack(stack_t *stack)
{
    free(stack->buf);
    free(stack);
}

int stack_push(stack_t *stack, void *data)
{
    if (stack->end == stack->buf + stack->size)
    {
        size_t new_size = stack->size * 2;
        void **tmp = realloc(stack->buf, new_size * sizeof(*stack->buf));

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

void *stack_pop(stack_t *stack)
{
    if (stack->end == stack->buf)
        return NULL;

    stack->end--;
    void *data = *stack->end;

    return data;
}
