#include "../inc/memory_watcher.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLAMP(x,a,b) MAX(MIN(x, b), a)

typedef struct membatch_s membatch_t;

struct membatch_s {
    size_t start;
    char pointers[MEMORY_BATCH_SIZE];
    membatch_t *next;
};

struct memwatch_s {
    membatch_t *head;
    size_t free_space;
    int show;
};

membatch_t *create_batch(size_t start)
{
    membatch_t *b = malloc(sizeof(membatch_t));

    if (b == NULL)
    {
        LOG_ERROR("не получилось выделить память для адресов%s", "");
        return NULL;
    }

    b->next = NULL;
    b->start = start;
    b->pointers[0] = MEMPTR_UNUSED;
    memset(b->pointers + 1, MEMPTR_PREV, MEMORY_BATCH_SIZE - 1);

    return b;
}

memwatch_t *create_memory_watch(size_t limit)
{
    memwatch_t *m = malloc(sizeof(memwatch_t));

    if (m == NULL)
    {
        return NULL;
    }

    m->head = create_batch(0);

    if (m->head == NULL)
    {
        free(m);
        return NULL;
    }

    m->free_space = limit;

    return m;
}

void free_memory_watch(memwatch_t *m)
{
    while (m->head != NULL)
    {
        membatch_t* tmp = m->head;
        m->head = tmp->next;
        free(tmp);
    }
    free(m);
}

char memwatch_get_type(memwatch_t *m, size_t x)
{
    x /= sizeof(void **);
    char prev = MEMPTR_UNUSED;

    for (membatch_t *b = m->head; b != NULL; b = b->next)
    {
        size_t u_to = MIN(MEMORY_BATCH_SIZE, x + 1 - b->start);

        for (size_t i = 0; i < u_to; i++)
            if (b->pointers[i] != MEMPTR_PREV)
                prev = b->pointers[i];

        if (u_to != MEMORY_BATCH_SIZE)
            break;
    }

    return prev;
}

size_t memwatch_get_size(memwatch_t *m, size_t x)
{
    x /= sizeof(void **);
    size_t size = 1;

    membatch_t *b = m->head;

    while (b != NULL && (x < b->start || x >= b->start + MEMORY_BATCH_SIZE))
        b = b->next;

    if (b == NULL)
        return 0;

    x = x - b->start + 1;


    while (b != NULL && b->pointers[x] == MEMPTR_PREV)
    {
        size++;
        x++;

        if (x == MEMORY_BATCH_SIZE)
        {
            x = 0;
            b = b->next;
        }
    }

    return size * sizeof(void **);
}

void memwatch_set(memwatch_t *m, size_t start, size_t size, char type)
{
    size_t end = (start + size + sizeof(void **) - 1) / sizeof(void **);
    start /= sizeof(void **);

    for (membatch_t *b = m->head; b != NULL; b = b->next)
    {
        size_t u_from = CLAMP(start, b->start, MEMORY_BATCH_SIZE + b->start) - b->start;
        size_t u_to = CLAMP(end, b->start, MEMORY_BATCH_SIZE + b->start) - b->start;

        if (u_from == u_to)
            continue;

        if (u_to < MEMORY_BATCH_SIZE)
        {
            b->pointers[u_to] = memwatch_get_type(m, u_to * sizeof(void **));
        }
        else if (b->next != NULL)
        {
            b->next->pointers[0] = memwatch_get_type(m, u_to * sizeof(void **));
        }

        if (u_from + b->start == start)
        {
            b->pointers[u_from] = type;
            memset(b->pointers + u_from + 1, MEMPTR_PREV, u_to - u_from - 1);
        }
        else
        {
            memset(b->pointers + u_from, MEMPTR_PREV, u_to - u_from);
        }
    }
}

int memwatch_grow(memwatch_t *m, size_t start, size_t size)
{
    size_t end = (start + size + sizeof(void **) - 1) / sizeof(void **);
    start /= sizeof(void **);

    membatch_t *b = m->head;
    membatch_t *prev_b = NULL;

    for (size_t i = start / MEMORY_BATCH_SIZE * MEMORY_BATCH_SIZE; i <= end; i += MEMORY_BATCH_SIZE)
    {
        while (b != NULL && b->start != 0 && b->start < i)
        {
            prev_b = b;
            b = b->next;
        }

        if (b == NULL)
        {
            b = create_batch(i);

            if (b == NULL)
                return EXIT_FAILURE;

            if (prev_b == NULL)
            {
                m->head = b;
            }
            else
            {
                prev_b->next = b;
            }

            continue;
        }

        if (b->start == 0)
        {
            b->start = i;
            continue;
        }

        if (b->start == i)
        {
            continue;
        }

        membatch_t *b_new = create_batch(i);

        if (b_new == NULL)
            return EXIT_FAILURE;

        if (prev_b == NULL)
            m->head = b_new;
        else
            prev_b->next = b_new;

        b_new->next = b;
        prev_b = b_new;
    }

    memwatch_set(m, (size_t) m, sizeof(*m), MEMPTR_WATCHER);

    for (membatch_t *b = m->head; b != NULL; b = b->next)
        memwatch_set(m, (size_t) b, sizeof(*b), MEMPTR_WATCHER);

    return EXIT_SUCCESS;
}

void memwatch_print_batches(memwatch_t *m, int n, ...)
{
    char prev = MEMPTR_UNUSED;

    for (membatch_t *b = m->head; b != NULL; b = b->next)
    {
        fprintf(stderr, "%14p [", (void *) (b->start * sizeof(void **)));

        for (size_t i = 0; i < MEMORY_BATCH_SIZE; i++)
        {
            if (b->pointers[i] != MEMPTR_PREV)
                prev = b->pointers[i];

            fprintf(stderr, "%c", prev);
        }

        fprintf(stderr, "]\n");

        char highlight[MEMORY_BATCH_SIZE + 1];
        memset(highlight, ' ', MEMORY_BATCH_SIZE);
        highlight[MEMORY_BATCH_SIZE] = '\0';

        int print_highlight = 0;

        va_list args;
        va_start(args, n);

        for (int i = 0; i < n; i++)
        {
            size_t start = va_arg(args, size_t);
            size_t size = va_arg(args, size_t);

            size_t end = (start + size + sizeof(void **) - 1) / sizeof(void **);
            start /= sizeof(void **);

            size_t u_from = CLAMP(start, b->start, MEMORY_BATCH_SIZE + b->start) - b->start;
            size_t u_to = CLAMP(end, b->start, MEMORY_BATCH_SIZE + b->start) - b->start;

            if (u_from != u_to)
            {
                print_highlight = 1;
                memset(highlight + u_from, '^', u_to - u_from);
            }
        }

        va_end(args);

        if (print_highlight)
            fprintf(stderr, "%14s  %s\n", "", highlight);
    }
}

void pause(void)
{
    char c;
    scanf("%c", &c);
}

void memwatch_set_show(memwatch_t *m, int show)
{
    m->show = show;
}

int memwatch_get_show(memwatch_t *m)
{
    return m->show;
}

void memwatch_set_free_space(memwatch_t *m, size_t free_space)
{
    m->free_space = free_space;
}

size_t memwatch_get_free_space(memwatch_t *m)
{
    return m->free_space;
}

void *wmalloc(memwatch_t *m, char type, size_t size)
{
    if (m == NULL)
        return malloc(size);

    if (size > m->free_space)
    {
        LOG_ERROR("превышен объем выделенной памяти%s", "");
        return NULL;
    }

    void *ptr = malloc(size);

    if (!ptr)
        return NULL;

    if (memwatch_grow(m, (size_t) ptr, size) == EXIT_FAILURE)
    {
        free(ptr);
        return NULL;
    }

    m->free_space -= size;

    memwatch_set(m, (size_t) ptr, size, type);

    if (!m->show)
        return ptr;

    fprintf(stderr, "=== memory watcher ===\n");
    fprintf(stderr, "malloc(%lu) -> %p\n", size, ptr);

    memwatch_print_batches(m, 1, (size_t) ptr, size);

    fprintf(stderr, "=== memory watcher ===\n");
    pause();

    return ptr;
}

void *wrealloc(memwatch_t *m, void *ptr, size_t new_size)
{
    if (m == NULL)
        return realloc(ptr, new_size);

    size_t size = memwatch_get_size(m, (size_t) ptr);

    if (new_size > m->free_space + size)
    {
        LOG_ERROR("превышен объем выделенной памяти%s", "");
        return NULL;
    }

    void *p = realloc(ptr, new_size);

    if (p == NULL)
        return p;

    if (memwatch_grow(m, (size_t) p, new_size) == EXIT_FAILURE)
    {
        ptr = realloc(ptr, size);
        return NULL;
    }

    char type = memwatch_get_type(m, (size_t) ptr);

    memwatch_set(m, (size_t) ptr, size, MEMPTR_USED);
    memwatch_set(m, (size_t) p, new_size, type);

    m->free_space += size;
    m->free_space -= new_size;

    if (!m->show)
        return p;

    fprintf(stderr, "=== memory watcher ===\n");
    fprintf(stderr, "realloc(%p, %lu) -> %p\n", ptr, new_size, p);

    memwatch_print_batches(m, 2, (size_t) p, new_size, (size_t) ptr, size);

    fprintf(stderr, "=== memory watcher ===\n");
    pause();

    return p;
}

void wfree(memwatch_t *m, void *ptr)
{
    free(ptr);

    if (m == NULL)
        return;

    size_t size = memwatch_get_size(m, (size_t) ptr);
    m->free_space += size;
    memwatch_set(m, (size_t) ptr, size, MEMPTR_USED);

    if (!m->show)
        return;

    fprintf(stderr, "=== memory watcher ===\n");
    fprintf(stderr, "free(%p)\n", ptr);

    memwatch_print_batches(m, 1, (size_t) ptr, size);

    fprintf(stderr, "=== memory watcher ===\n");
    pause();
}
