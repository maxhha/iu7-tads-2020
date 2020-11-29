#include "../inc/stack_list.h"

stack_list_t *create_stack_list(size_t size, memwatch_t *mem)
{
    stack_list_t *stack = wmalloc(mem, MEMPTR_STACK, sizeof(stack_list_t));

    if (stack == NULL)
        return NULL;

    stack->head = NULL;
    stack->size = size;
    stack->n_elems = 0;
    stack->limit = size != 0;
    stack->mem = mem;

    return stack;
}

void free_stack_list(stack_list_t *stack)
{
    wfree(stack->mem, stack);
}

int stack_list_push(stack_list_t *stack, void *data)
{
    if (stack->limit && stack->n_elems == stack->size)
        return -1;

    list_t *l = wmalloc(stack->mem, MEMPTR_STACK, sizeof(list_t));

    if (l == NULL)
        return -1;

    l->data = data;
    l->next = stack->head;
    stack->head = l;

    stack->n_elems++;

    return 0;
}

void *stack_list_pop(stack_list_t *stack)
{
    if (stack->head == NULL)
        return NULL;

    list_t *next = stack->head->next;
    void *data = stack->head->data;

    wfree(stack->mem, stack->head);
    stack->head = next;

    stack->n_elems--;

    return (void *) data;
}
