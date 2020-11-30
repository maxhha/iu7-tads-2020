#ifndef __MEASURE_H__
#define __MEASURE_H__

#include <stdint.h>
#include "template.h"
#include "queue_list.h"
#include "queue_array.h"

#define MEASURE_TIME_REPEATS 1000

int measure_time_of_queue_list(int n, uint64_t *push_ticks, uint64_t *pop_ticks);
int measure_time_of_queue_array(int n, uint64_t *push_ticks, uint64_t *pop_ticks);

#endif // __MEASURE_H__
