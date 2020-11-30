#include "../inc/queue_list.h"

queue_list_t *create_queue_list(memwatch_t *mem)
{
    queue_list_t *q = wmalloc(mem, MEMPTR_QUEUE, sizeof(queue_list_t));

    if (!q)
        return NULL;

    q->last = NULL;
    q->first = NULL;
    q->mem = mem;

    return q;
}

void free_queue_list(queue_list_t *q)
{
    if (q->last || q->first)
        LOG_ERROR("q->last or q->first is not null%s", "");

    free(q);
}

int push_queue_list(queue_list_t *q, void *data)
{
    list_t *l = wmalloc(q->mem, MEMPTR_QUEUE, sizeof(list_t));

    if (!l)
        return -1;

    l->data = data;
    l->next = NULL;

    if (q->last)
        q->last->next = l;
    
    q->last = l;

    if (q->first == NULL)
        q->first = l;

    return 0;
}

void *pop_queue_list(queue_list_t *q)
{
    if (q->first == NULL)
        return NULL;

    list_t *l = q->first;
    void *data = l->data;

    if ((q->first = l->next) == NULL)
        q->last = NULL;

    wfree(q->mem, l);

    return data;
}
