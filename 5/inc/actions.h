#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include "memory_watcher.h"
#include "colors.h"
#include "log.h"
#include "measure.h"

typedef struct {
    enum { LIST = 1, ARRAY = 2} type;
    double t1_from;
    double t1_to;
    double t2_from;
    double t2_to;
    double t3_from;
    double t3_to;
    double t4_from;
    double t4_to;
    memwatch_t *mem;
} action_params_t;

#include "simulation.h"

action_params_t init_action_params(void);
void print_action_params(action_params_t);

void action_set_time_ranges(action_params_t *params);
void action_set_address(action_params_t *params);
void action_change_type(action_params_t *params);
void action_measure_queue(action_params_t params);
void action_process(action_params_t);

#endif // __ACTIONS_H__
