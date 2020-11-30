#include "../inc/actions.h"

action_params_t init_action_params(void)
{
    action_params_t p = {
        .mem = NULL, // create_memory_watch(),//
        .type = ARRAY,
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

void print_action_params(action_params_t p)
{
    printf("Тип очереди: " YEL "%s" RESET "\n", p.type == ARRAY ? "вектор" : "список");
    printf("Выводить адреса: " YEL "%s" RESET "\n", p.mem ? "да" : "нет");
    printf("T1: " YEL "%.1lf..%.1lf" RESET "\n", p.t1_from, p.t1_to);
    printf("T2: " YEL "%.1lf..%.1lf" RESET "\n", p.t2_from, p.t2_to);
    printf("T3: " YEL "%.1lf..%.1lf" RESET "\n", p.t3_from, p.t3_to);
    printf("T4: " YEL "%.1lf..%.1lf" RESET "\n", p.t4_from, p.t4_to);
}

void action_change_type(action_params_t *params)
{
    printf("%d" YEL " - использовать cписок" RESET "\n", LIST);
    printf("%d" YEL " - использовать вектор" RESET "\n", ARRAY);
    printf("Выбор:\n");

    char *buf = NULL;
    size_t buf_size = 0;

    if (getline(&buf, &buf_size, stdin) < 0)
    {
        if (buf)
            free(buf);

        printf(RED "Не получилось считать строку" RESET "\n");
        return;
    }

    int x;

    if (sscanf(buf, "%d", &x) != 1 || !(x == ARRAY || x == LIST))
    {
        printf(RED "Неправильный выбор" RESET "\n");
        free(buf);
        return;
    }
    free(buf);

    params->type = x;
}

void action_process(action_params_t params)
{
    simulation_result_t result = params.type == LIST ? \
        simulate_using_queue_list(params) : \
        simulate_using_queue_array(params);

    LOG_DEBUG("result->error = %d", result.error);

    printf("error: %d\n", result.error);
    printf("in_orders1: %d\n", result.in_orders1);
    printf("in_orders2: %d\n", result.in_orders2);
    printf("out_orders1: %d\n", result.out_orders1);
    printf("out_orders2: %d\n", result.out_orders2);
    printf("time_wait: %.0lf\n", result.time_wait);
    printf("time_work: %.0lf\n", result.time_work);
}
