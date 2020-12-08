#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdlib.h>
#include <stdio.h>
#include "log.h"

typedef size_t (*hashfunc_t) (int);

typedef struct
{
    int **items;
    int size;
    hashfunc_t hash;
    int n_collisions;
} hashtable_t;

size_t simple_hash(int x);
size_t complex_hash(int x);

hashtable_t *create_hashtable(hashfunc_t hash, int size);
void free_hashtable(hashtable_t *t);

int add_to_hashtable(hashtable_t *table, int x);
hashtable_t *read_to_hashtable(FILE *f, hashfunc_t hash, int size);

void print_hashtable(hashtable_t *table);
int get_hashtable_count_items(hashtable_t *table);

hashtable_t *restructure_hashtable(hashtable_t *table, hashfunc_t new_hash, int new_size);

#endif // __HASHTABLE_H__
