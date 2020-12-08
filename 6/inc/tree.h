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
    int height;
    tree_t *left;
    tree_t *right;
};

tree_t *read_to_tree(FILE *f);
void free_tree(tree_t *root);
void print_tree(tree_t *root);
tree_t *tree_into_bamboo(tree_t *root);
tree_t *balance_tree(tree_t *root);
int copy_tree(tree_t *root, tree_t **new_root);
tree_t *find_tree_val(tree_t *root, int val);
tree_t *delete_element_from_tree(tree_t* root, int val, int *cmprs);
tree_t *delete_element_from_balanced_tree(tree_t* root, int val, int *cmprs);

#endif // __TREE_H__
