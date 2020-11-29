#include "../inc/actions.h"

action_params_t init_action_params(void)
{
    action_params_t p = {
        .mem = NULL,
        .type = LIST,
        .t1_from = 1,
        .t1_to = 5,
        .t2_from = 0,
        .t2_to = 3,
        .t3_from = 0,
        .t3_to = 4,
        .t4_from = 0,
        .t4_to = 1,
    };
    return p;
}


void action_process(action_params_t params)
{
    // simulation_result_t result = params.type == LIST ? simulate_using_queue_list(params)
    simulation_result_t result = simulate_using_queue_list(params);

    LOG_DEBUG("result->error = %d", result.error);

    printf("error: %d\n", result.error);
    printf("in_orders1: %d\n", result.in_orders1);
    printf("in_orders2: %d\n", result.in_orders2);
    printf("out_orders1: %d\n", result.out_orders1);
    printf("out_orders2: %d\n", result.out_orders2);
    printf("time_wait: %.0lf\n", result.time_wait);
    printf("time_work: %.0lf\n", result.time_work);
}
