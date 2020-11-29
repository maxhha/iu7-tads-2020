#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <stdlib.h>
#include <stdio.h>
#include "log.h"
#include "memory_watcher.h"
#include "template.h"
#include "actions.h"
#include "queue_list.h"

#define SIMULATION_LIMIT_Q1 1000
#define SIMULATION_SHOW_STATUS_EVERY 100

typedef struct {
    int in_orders1;
    int in_orders2;
    int out_orders1;
    int out_orders2;
    double time_wait;
    double time_work;
    double time_sim;
    int error;
} simulation_result_t;

typedef struct {
    int number;
    int type;
} order_t;

simulation_result_t simulate_using_queue_list(action_params_t params);

#endif // __SIMULATION_H__
