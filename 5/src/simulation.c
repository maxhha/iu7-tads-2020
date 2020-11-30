#include "../inc/simulation.h"

#define Q queue_list
#include "./simulation.tmpl.c"
#undef Q

#define Q queue_array
#include "./simulation.tmpl.c"
#undef Q
