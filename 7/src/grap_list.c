#include "graph_list.h"

graph_list_t *create_graph_list(int n)
{
    graph_list_t *g = malloc(sizeof(graph_list_t));

    if (g == NULL)
        return NULL;

    g->data = malloc(sizeof(list_t *) * n);

    if (g->data == NULL)
    {
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++)
        g->data[i] = NULL;

    g->vertices_n = n;

    return g;
}

void free_graph_list(graph_list_t *g)
{
    for (int i = 0; i < g->vertices_n; i++)
    {
        for (list_t *p = g->data[i]; p != NULL; )
        {
            list_t *t = p->next;
            free(p);
            p = t;
        }
    }

    free(g->data);
    free(g);
}

int graph_list_set(graph_list_t *g, int v_from, int v_to, int weight)
{
    list_t **pp = g->data + v_from;

    while (*pp != NULL && (*pp)->vertex != v_to)
        pp = &(*pp)->next;

    if (*pp != NULL)
    {
        (*pp)->weight = weight;
        return 0;
    }

    *pp = malloc(sizeof(list_t));

    if (*pp == NULL)
        return 1;

    (*pp)->vertex = v_to;
    (*pp)->weight = weight;
    (*pp)->next = NULL;

    return 0;
}

int graph_list_get(graph_list_t *g, int v_from, int v_to)
{
    for (list_t *p = g->data[v_from]; p != NULL; p = p->next)
        if (p->vertex == v_to)
            return p->weight;

    return -1;
}

static list_t *prev_in_iter = NULL;
static int prev_iter_v_from = -1;

int graph_list_get_next(graph_list_t *g, int v_from, int v_last)
{
    list_t *p = g->data[v_from];

    if (p == NULL)
        return -1;

    if (v_last == -1)
    {
        prev_iter_v_from = v_from;
        return (prev_in_iter = p)->vertex;
    }

    if (prev_iter_v_from == v_from)
        p = prev_in_iter;
    else
        while (p != NULL && p->vertex != v_last)
            p = p->next;

    prev_in_iter = p = p->next;

    return p == NULL ? -1 : p->vertex;
}
