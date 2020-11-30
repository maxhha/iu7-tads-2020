#include "../inc/measure.h"

#define Q queue_list
#include "measure.tmpl.c"
#undef Q

#define Q queue_array
#include "measure.tmpl.c"
#undef Q
