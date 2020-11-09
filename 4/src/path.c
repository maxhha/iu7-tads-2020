#include "../inc/path.h"

#define STACK stack_list

#include "path.tmpl.c"

#undef STACK
#define STACK stack_array

#include "path.tmpl.c"
