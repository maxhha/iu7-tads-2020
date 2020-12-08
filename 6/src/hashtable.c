#include "hashtable.h"

size_t simple_hash(int x)
{
    return x;
}

/* Robert Jenkins' 16 bit integer hash function */
size_t complex_hash(int a)
{
    unsigned short x = a;
    x = (x + 0xa3ca) + (x << 12);
    x = (x ^ 0xa652) ^ (x >> 15);
    x = (x + 0x1a24) + (x << 13);
    x = (x + 0x1219) ^ (x << 3);
    x = (x + 0x70a1) + (x << 15);
    x = (x ^ 0x5e1b) ^ (x >> 14);

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
    table->n_collisions = 0;

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

            table->n_collisions += a;

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
    }

    return table;
}

int get_hashtable_count_items(hashtable_t *table)
{
    int n_elems = 0;

    for (int i = 0; i < table->size; i++)
        if (table->items[i] != NULL)
            n_elems++;

    return n_elems;
}

void print_hashtable(hashtable_t *table)
{
    int key_len = snprintf(NULL, 0, "%d", table->size - 1);
    int n = 0;

    printf("{\n");

    for (int i = 0; i < table->size; i++)
    {
        printf("  %-*d: ", key_len, i);

        if (table->items[i] == NULL)
            printf("_\n");
        else
        {
            printf("%d\n", *table->items[i]);
            n++;
        }
    }
    printf("}\n");

    printf("Среднее число коллизий: %.0lf\n", (double) table->n_collisions / n);
}

hashtable_t *restructure_hashtable(hashtable_t *table, hashfunc_t new_hash, int new_size)
{
    hashtable_t *new_table = create_hashtable(new_hash, new_size);

    if (new_table == NULL)
        return NULL;

    for (int i = 0; i < table->size; i++)
    {
        if (table->items[i] != NULL)
            if (add_to_hashtable(new_table, *table->items[i]))
            {
                LOG_ERROR("таблица переполнена%s", "");
                free_hashtable(new_table);
                return NULL;
            }
    }

    return new_table;
}
