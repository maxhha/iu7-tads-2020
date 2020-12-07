#include "../inc/simulation.h"

#ifndef Q
#define Q queue_list
#endif

#define Q_T TMPL(Q, t)
#define COROUTINE_T TMPL(Q, coroutine_t)

#define COROUNTINES_N 3

#define RAND_TIME(t1,t2) \
    ((double) rand() / RAND_MAX * ((t2) - (t1)) + (t1))

typedef struct TMPL(Q, coroutines_args_s) TMPL(Q, coroutines_args_t);

typedef double (*TMPL(callback_using, Q))(TMPL(Q, coroutines_args_t) *args);

typedef struct {
    TMPL(callback_using, Q) callback;
    double time;
} COROUTINE_T;

struct TMPL(Q, coroutines_args_s) {
    action_params_t *params;
    simulation_result_t *result;
    Q_T *q1;
    Q_T *q2;
    int prev;
    COROUTINE_T *coroutines;
    order_t *order;
};

COROUTINE_T *TMPL(get_next_coroutine, Q)(COROUTINE_T *coroutines, int n)
{
    COROUTINE_T *min_cor = coroutines;
    double min_time = coroutines[0].time;

    for (int i = 0; i < n; i++)
    {
        LOG_DEBUG("coroutines[%i].time = %lf", i, coroutines[i].time);
        if (min_time > coroutines[i].time)
        {
            min_time = coroutines[i].time;
            min_cor = coroutines + i;
        }
    }

    LOG_DEBUG("min_time = %lf", min_time);
    LOG_DEBUG("cor_i = %d", (int) (min_cor - coroutines));

    return min_cor;
}

void TMPL(print_current_result_for, Q)(simulation_result_t *result)
{
    printf("\n");
    printf("тип       1  \t   2\n");
    printf("вошло  " YEL "%4d" RESET "\t" YEL "%4d" RESET "\n", result->in_orders1, result->in_orders2);
    printf("вышло  " YEL "%4d" RESET "\t" YEL "%4d" RESET "\n", result->out_orders1, result->out_orders2);
    printf("Общее время: " YEL "%0.3lf" RESET "\n", result->time_sim);

}

double TMPL(process_order_using, Q)(TMPL(Q, coroutines_args_t) *args)
{
    LOG_DEBUG("start%s", "");

    if (args->order)
    {
        LOG_DEBUG("finish order%d #%d", args->order->type, args->order->number);

        args->prev = args->order->type;

        wfree(args->params->mem, args->order);

        args->order = NULL;

        if (args->prev == 1)
        {
            args->result->out_orders1++;

            if (args->result->out_orders1 % SIMULATION_SHOW_STATUS_EVERY == 0)
                TMPL(print_current_result_for, Q)(args->result);
        }
        else
            args->result->out_orders2++;

        return 0;
    }

    LOG_DEBUG("check for new order%s", "");

    order_t *order = args->prev == 1 ? TMPL(pop, Q)(args->q1) : TMPL(pop, Q)(args->q2);

    if (!order)
        order = args->prev == 1 ? TMPL(pop, Q)(args->q2) : TMPL(pop, Q)(args->q1);

    if (!order)
    {
        LOG_DEBUG("nothing to process%s", "");

        double time_wait = TMPL(get_next_coroutine, Q)(args->coroutines, 2)->time - args->coroutines[2].time;

        args->result->time_wait += time_wait;
        return time_wait;
    }

    LOG_DEBUG("start process order%d #%d", order->type, order->number);

    double time_work = order->type == 1 ? \
        RAND_TIME(args->params->t3_from, args->params->t3_to) : \
        RAND_TIME(args->params->t4_from, args->params->t4_to);

    args->order = order;
    args->result->time_work += time_work;

    return time_work;
}

int TMPL(push_order_using, Q)(memwatch_t *mem, Q_T *q, int type, int number)
{
    LOG_DEBUG("push order%d #%d", type, number);

    order_t *order = wmalloc(mem, MEMPTR_DATA, sizeof(order_t));

    if (!order)
    {
        LOG_ERROR("не получилось выделить память для заявки%s", "");
        return -1;
    }

    order->type = type;
    order->number = number;

    if (TMPL(push, Q)(q, order))
    {
        LOG_ERROR("не получилось добавить заявку в очередь%s", "");
        wfree(mem, order);
        return -1;
    }

    return 0;
}

double TMPL(push_order1_using, Q)(TMPL(Q, coroutines_args_t) *args)
{
    LOG_DEBUG("start%s", "");

    if (TMPL(push_order_using, Q)(args->params->mem, args->q1, 1, args->result->in_orders1 + 1))
    {
        LOG_ERROR("не получилось добавить заявку в 1 очередь%s", "");
        args->result->error = -1;
        return 0;
    }

    args->result->in_orders1++;

    return RAND_TIME(args->params->t1_from, args->params->t1_to);
}

double TMPL(push_order2_using, Q)(TMPL(Q, coroutines_args_t) *args)
{
    LOG_DEBUG("start%s", "");

    if (TMPL(push_order_using, Q)(args->params->mem, args->q2, 2, args->result->in_orders2 + 1))
    {
        LOG_ERROR("не получилось добавить заявку в 2 очередь%s", "");
        args->result->error = -1;
        return 0;
    }

    args->result->in_orders2++;

    return RAND_TIME(args->params->t2_from, args->params->t2_to);
}

simulation_result_t TMPL(simulate_using, Q)(action_params_t params)
{
    simulation_result_t res = { 0 };

    Q_T *q1 = TMPL(create, Q)(params.mem);
    Q_T *q2 = TMPL(create, Q)(params.mem);

    if (!q1 || !q2)
    {
        LOG_ERROR("Не получилось создать одну из очередей%s", "");
        if (q1) TMPL(free, Q)(q1);
        if (q2) TMPL(free, Q)(q2);
        res.error = -1;
        return res;
    }

    COROUTINE_T coroutines[COROUNTINES_N] = {
        {
            .callback = TMPL(push_order1_using, Q),
            .time = RAND_TIME(params.t1_from, params.t1_to),
        },
        {
            .callback = TMPL(push_order2_using, Q),
            .time = RAND_TIME(params.t2_from, params.t2_to),
        },
        {
            .callback = TMPL(process_order_using, Q),
            .time = 0,
        },
    };

    TMPL(Q, coroutines_args_t) cor_args = {
        .params = &params,
        .result = &res,
        .q1 = q1,
        .q2 = q2,
        .prev = 1,
        .coroutines = coroutines,
        .order = NULL,
    };

    while (res.out_orders1 < SIMULATION_LIMIT_Q1 && !res.error)
    {
        COROUTINE_T *min_cor = TMPL(get_next_coroutine, Q)(coroutines, COROUNTINES_N);
        double min_time = min_cor->time;

        res.time_sim = min_time;

        double cor_time = min_cor->callback(&cor_args);
        LOG_DEBUG("cor_time = %lf", cor_time);

        min_cor->time += cor_time;
        LOG_DEBUG("min_cor->time = %lf", min_cor->time);
    }

    LOG_DEBUG("free rest orders%s", "");

    for (order_t *order = TMPL(pop, Q)(q1); order; order = TMPL(pop, Q)(q1))
        wfree(params.mem, order);

    for (order_t *order = TMPL(pop, Q)(q2); order; order = TMPL(pop, Q)(q2))
        wfree(params.mem, order);

    if (cor_args.order)
        wfree(params.mem, cor_args.order);

    TMPL(free, Q)(q1);
    TMPL(free, Q)(q2);

    return res;
}

#undef COROUNTINES_N
#undef RAND_TIME
#undef COROUTINE_T
#undef Q_T
