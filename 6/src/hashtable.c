#include "hashtable.h"

size_t simple_hash(int x)
{
    return x;
}

/* Robert Jenkins' 16 bit integer hash function */
size_t complex_hash(int a)
{
    unsigned short x = a;
    x = (x + 0x49ff) + (x << 15);
    x = (x ^ 0x9f80) ^ (x >> 3);
    x = (x + 0x031a) + (x << 9);
    x = (x + 0xa5bb) ^ (x << 13);
    x = (x + 0x1ca4) + (x << 13);
    x = (x ^ 0x6205) ^ (x >> 12);

    return x;
}

void free_hashtable(hashtable_t *t)
{
    for (int i = 0; i < t->size; i++)
        if (t->items[i] != NULL)
            free(t->items[i]);

    free(t->items);
    free(t);
}

hashtable_t *create_hashtable(hashfunc_t hash, int size)
{
    hashtable_t *table = malloc(sizeof(hashtable_t));

    if (table == NULL)
        return NULL;

    table->items = malloc(sizeof(*table->items) * size);

    if (table->items == NULL)
    {
        free(table);
        return NULL;
    }

    for (int i = 0; i < size; i++)
        table->items[i] = NULL;

    table->size = size;
    table->hash = hash;
    table->max_collisions = 0;

    return table;
}

int add_to_hashtable(hashtable_t *table, int x)
{
    int h = table->hash(x) % table->size;
    LOG_DEBUG("hash(%d) = %d", x, h);

    for (int a = 0; a < table->size; a++)
    {
        int i = (h + a * a) % table->size;
        LOG_DEBUG("i = %d", i);

        if (table->items[i] == NULL)
        {
            int *item = malloc(sizeof(int));

            if (item == NULL)
                return 1;

            *item = x;

            table->items[i] = item;

            if (a > table->max_collisions)
                table->max_collisions = a;

            return 0;
        }

        if (*(table->items[i]) == x)
            return 0;
    }

    return 1;
}

hashtable_t *read_to_hashtable(FILE *f, hashfunc_t hash, int size)
{
    hashtable_t *table = create_hashtable(hash, size);

    int x;
    while (fscanf(f, "%d", &x) == 1)
    {
        if (add_to_hashtable(table, x))
        {
            LOG_ERROR("таблица переполнена%s", "");
            free_hashtable(table);
            return NULL;
        }
        #ifdef DEBUG
        LOG_DEBUG("add element %d", x);
        print_hashtable(table);
        #endif
    }

    return table;
}

void print_hashtable(hashtable_t *table)
{
    int key_len = snprintf(NULL, 0, "%d", table->size - 1);

    printf("{\n");

    for (int i = 0; i < table->size; i++)
    {
        printf("  %-*d: ", key_len, i);

        if (table->items[i] == NULL)
            printf("_\n");
        else
            printf("%d\n", *table->items[i]);
    }
    printf("}\n");


    printf("Максимальное число коллизий: %d\n", table->max_collisions);
}
