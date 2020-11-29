#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include "memory_watcher.h"
#include "log.h"

typedef struct {
    enum { ARRAY, LIST} type;
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

void action_process(action_params_t);

#endif // __ACTIONS_H__
