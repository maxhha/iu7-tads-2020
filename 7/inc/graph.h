#ifndef __READ_GRAPH_H__
#define __READ_GRAPH_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "log.h"
#include "stack.h"
#include "graph_matrix.h"

#define OUTPUT_FILE "graph.gv"
#define OUTPUT_IMG_FILE "result.png"
#define SELECTED_VERTEX_DOT_ATTRS " style = filled, fillcolor = lightpink,"
#define UNVISITED_VERTEX_DOT_ATTRS " style = filled, fillcolor = gray,"
#define CAPITAL_VERTEX_DOT_ATTRS " style = filled, fillcolor = lightgoldenrod1,"

uint64_t ticks(void);

int stack_push_vertex(stack_t *s, int vertex, int distance);
int stack_pop_vertex(stack_t *s, int *vertex, int *distance);

void print_result_verteces(int *distances, int n, int param_t);

int process_with_graph_matrix(char *filename, int capital, int param_t);


#endif // __READ_GRAPH_H__
