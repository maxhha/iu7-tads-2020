#ifndef __READ_GRAPH_H__
#define __READ_GRAPH_H__

#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "graph_matrix.h"

#define OUTPUT_FILE "graph.gv"
#define OUTPUT_IMG_FILE "result.png"

int process_with_graph_matrix(char *filename, int capital, int param_t);

#endif // __READ_GRAPH_H__
