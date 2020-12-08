#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log.h"

typedef struct tree_s tree_t;

struct tree_s
{
    int val;
    tree_t *left;
    tree_t *right;
};

tree_t *read_to_tree(FILE *f);
void free_tree(tree_t *root);
void print_tree(tree_t *root);
tree_t *tree_into_bamboo(tree_t *root);
tree_t *balance_tree(tree_t *root);

#endif // __TREE_H__
