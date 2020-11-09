#include "../inc/stack_list.h"

stack_list_t *create_stack_list(void)
{
    stack_list_t *stack = malloc(sizeof(stack_list_t));

    if (stack == NULL)
        return NULL;

    stack->head = NULL;

    return stack;
}

void free_stack_list(stack_list_t *stack)
{
    free(stack);
}

int stack_list_push(stack_list_t *stack, void *data)
{
    list_t *l = malloc(sizeof(list_t));

    if (l == NULL)
        return -1;

    l->data = data;
    l->next = stack->head;
    stack->head = l;

    return 0;
}

void *stack_list_pop(stack_list_t *stack)
{
    if (stack->head == NULL)
        return NULL;

    list_t *next = stack->head->next;
    const void *data = stack->head->data;

    free(stack->head);
    stack->head = next;

    return (void *) data;
}
