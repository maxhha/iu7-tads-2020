#ifndef __GRAPH_LIST_H__
#define __GRAPH_LIST_H__

#include <stdlib.h>

typedef struct list_s list_t;

struct list_s
{
    int vertex;
    int weight;
    list_t *next;
};

typedef struct
{
    list_t **data;
    int verteces_n;
} graph_list_t;

graph_list_t *create_graph_list(int n);
void free_graph_list(graph_list_t *g);
int graph_list_set(graph_list_t *g, int v_from, int v_to, int weight);
int graph_list_get(graph_list_t *g, int v_from, int v_to);
int graph_list_get_next(graph_list_t *g, int v_from, int v_last);

#endif // __GRAPH_LIST_H__
