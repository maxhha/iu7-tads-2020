#ifndef __GRAPH_MATRIX_H__
#define __GRAPH_MATRIX_H__

#include <stdlib.h>

typedef struct
{
    int *data;
    int verteces_n;
} graph_matrix_t;

graph_matrix_t *create_graph_matrix(int n);
void free_graph_matrix(graph_matrix_t *g);
int graph_matrix_set(graph_matrix_t *g, int v_from, int v_to, int weight);
int graph_matrix_get(graph_matrix_t *g, int v_from, int v_to);
int graph_matrix_get_next(graph_matrix_t *g, int v_from, int v_last);

#endif // __GRAPH_MATRIX_H__
