#include "../inc/queue_array.h"

queue_array_t *create_queue_array(memwatch_t *mem)
{
    queue_array_t *q = wmalloc(mem, MEMPTR_QUEUE, sizeof(queue_array_t));

    if (!q)
        return NULL;

    q->size = DEFAULT_BUF_SIZE;
    q->buf = wmalloc(mem, MEMPTR_QUEUE, sizeof(*q->buf) * q->size);

    if (!q->buf)
    {
        wfree(mem, q);
        return NULL;
    }

    q->mem = mem;
    q->first = q->buf;
    q->end = q->buf;

    return q;
}

void free_queue_array(queue_array_t *q)
{
    wfree(q->mem, q->buf);
    wfree(q->mem, q);
}

int push_queue_array(queue_array_t *q, void *data)
{
    LOG_DEBUG("q = %p", (void *) q);
    LOG_DEBUG("q->first - q->buf = %ld", q->first - q->buf);
    LOG_DEBUG("q->end - q->buf = %ld", q->end - q->buf);
    LOG_DEBUG("q->size = %d", q->size);

    *q->end = data;
    q->end++;

    if (q->end == q->buf + q->size)
        q->end = q->buf;

    if (q->first == q->end)
    {
        int new_size = q->size * 2;
        LOG_DEBUG("grow to %d", new_size);

        void **tmp = wrealloc(q->mem, q->buf, new_size * sizeof(*q->buf));

        if (tmp == NULL)
            return -1;

        LOG_DEBUG("move [0, %ld) to [%d, %ld)", q->end - q->buf, q->size, q->end - q->buf + q->size);
        memcpy(tmp + q->size, tmp, (q->end - q->buf) * sizeof(*q->buf));

        q->first = q->first - q->buf + tmp;
        q->buf = tmp;
        q->end = q->first + q->size;
        q->size = new_size;

        LOG_DEBUG("new q->first - q->buf = %ld", q->first - q->buf);
        LOG_DEBUG("new q->end - q->buf = %ld", q->end - q->buf);
    }

    return 0;
}

void *pop_queue_array(queue_array_t *q)
{
    LOG_DEBUG("q = %p", (void *) q);
    LOG_DEBUG("q->first - q->buf = %ld", q->first - q->buf);
    LOG_DEBUG("q->end - q->buf = %ld", q->end - q->buf);
    LOG_DEBUG("q->size = %d", q->size);

    if (q->first == q->end)
        return NULL;

    void *data = *q->first;

    *q->first = NULL;
    q->first++;

    if (q->first == q->buf + q->size)
        q->first = q->buf;

    return data;
}
