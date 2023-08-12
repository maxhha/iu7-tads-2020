#include "graph_matrix.h"

// typedef struct
// {
//     int *data;
//     int vertices_n;
// } graph_matrix_t;

graph_matrix_t *create_graph_matrix(int n)
{
    graph_matrix_t *g = malloc(sizeof(graph_matrix_t));

    if (g == NULL)
    {
        return NULL;
    }

    g->data = malloc(sizeof(*g->data)* n * n);

    if (g->data == NULL)
    {
        free(g);
        return NULL;
    }

    for (int i = 0; i < n * n; i++)
        g->data[i] = -1;

    g->vertices_n = n;
    return g;
}

void free_graph_matrix(graph_matrix_t *g)
{
    free(g->data);
    free(g);
}

int graph_matrix_set(graph_matrix_t *g, int v_from, int v_to, int weight)
{
    g->data[v_to + v_from * g->vertices_n] = weight;
    return 0;
}

int graph_matrix_get(graph_matrix_t *g, int v_from, int v_to)
{
    return g->data[v_to + v_from * g->vertices_n];
}

int graph_matrix_get_next(graph_matrix_t *g, int v_from, int v_last)
{
    int j = v_from * g->vertices_n;

    for (int i = v_last + 1; i < g->vertices_n; i++)
    {
        if (g->data[i + j] != -1)
            return i;
    }

    return -1;
}
