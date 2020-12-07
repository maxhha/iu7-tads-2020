#include "../inc/actions.h"

action_params_t init_action_params(void)
{
    action_params_t p = {
        .mem = create_memory_watch(8192),
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
    printf("Выводить адреса: " YEL "%s" RESET "\n", memwatch_get_show(p.mem) ? "да" : "нет");
    printf("Ограничение памяти: " YEL "%lu байт" RESET "\n", memwatch_get_free_space(p.mem));
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

void action_set_address(action_params_t *params)
{
    printf("Выводить адреса памяти:\n");
    printf("1" YEL " - да" RESET "\n");
    printf("2" YEL " - нет" RESET "\n");
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

    if (sscanf(buf, "%d", &x) != 1 || !(x == 1 || x == 2))
    {
        printf(RED "Неправильный выбор" RESET "\n");
        free(buf);
        return;
    }

    memwatch_set_show(params->mem, x == 1);
}

void action_set_free_mem(action_params_t *params)
{
    printf("Введите ограничение памяти в байтах:\n");
    printf("(-1 - нет ограничения)\n");

    char *buf = NULL;
    size_t buf_size = 0;

    if (getline(&buf, &buf_size, stdin) < 0)
    {
        if (buf)
            free(buf);

        printf(RED "Не получилось считать строку" RESET "\n");
        return;
    }

    size_t limit;

    if (sscanf(buf, "%lu", &limit) != 1)
    {
        printf(RED "Неправильный ввод" RESET "\n");
        free(buf);
        return;
    }

    free(buf);

    memwatch_set_free_space(params->mem, limit);
}

#define READ_TIME_RANGE(buf, buf_size, msg, from, to) do { \
    printf(msg); \
    if (getline(&buf, &buf_size, stdin) < 0) \
    { \
        if (buf) \
            free(buf); \
        printf(RED "Не получилось считать строку" RESET "\n"); \
        return; \
    } \
    if (sscanf(buf, "%lf%lf", &(from), &(to)) != 2) \
    { \
        printf(RED "Неправильный ввод" RESET "\n"); \
        continue; \
    } \
    if ((from) < 0 || (to) < 0) \
    { \
        printf(RED "Значения должны быть неотрицательными" RESET "\n"); \
        continue; \
    } \
    if ((from) > (to)) \
    { \
        printf(RED "Первое значение должно быть неменьше второго" RESET "\n"); \
        continue; \
    } \
    break; \
} while(1)

void action_set_time_ranges(action_params_t *params)
{
    char *buf = NULL;
    size_t buf_size = 0;

    printf("Вводите мин. и макс. значение времени через пробел:\n");

    READ_TIME_RANGE(buf, buf_size, "T1:", params->t1_from, params->t1_to);
    READ_TIME_RANGE(buf, buf_size, "T2:", params->t2_from, params->t2_to);
    READ_TIME_RANGE(buf, buf_size, "T3:", params->t3_from, params->t3_to);
    READ_TIME_RANGE(buf, buf_size, "T4:", params->t4_from, params->t4_to);
}

void action_measure_queue(action_params_t params)
{
    printf("Введите количество элементов:\n");

    char *buf = NULL;
    size_t buf_size = 0;

    if (getline(&buf, &buf_size, stdin) < 0)
    {
        if (buf)
            free(buf);

        printf(RED "Не получилось считать строку" RESET "\n");
        return;
    }

    int n;

    if (sscanf(buf, "%d", &n) != 1 || n <= 0)
    {
        printf(RED "Неправильный выбор" RESET "\n");
        free(buf);
        return;
    }
    free(buf);

    uint64_t pop_ticks, push_ticks;
    int rc = params.type == LIST ?
        measure_time_of_queue_list(n, &push_ticks, &pop_ticks) : \
        measure_time_of_queue_array(n, &push_ticks, &pop_ticks);

    if (rc)
    {
        printf(RED "Во время измерений произошла ошибка" RESET "\n");
        return;
    }

    printf("Добавление: %lu тиков\n", push_ticks);
    printf("Чтение: %lu тиков\n", pop_ticks);
}

void action_process(action_params_t params)
{
    simulation_result_t result = params.type == LIST ? \
        simulate_using_queue_list(params) : \
        simulate_using_queue_array(params);

    LOG_DEBUG("result->error = %d", result.error);

    if (result.error)
    {
        printf(RED "Произошла ошибка во время моделирования" RESET "\n");
        return;
    }

    printf("\n");
    printf("-------------------------------------------------------------\n");

    #define PRINTVAL(descr, type, val) \
        printf(descr ": " YEL type RESET "\n", val)

    #define NL printf("\n")

    PRINTVAL("Вошло заявок 1 типа", "%d", result.in_orders1);
    PRINTVAL("Вышло заявок 1 типа", "%d", result.out_orders1);
    NL;
    PRINTVAL("Вошло заявок 2 типа", "%d", result.in_orders2);
    PRINTVAL("Вышло заявок 2 типа", "%d", result.out_orders2);
    NL;
    PRINTVAL("Время простоя аппарата", "%.0lf", result.time_wait);
    PRINTVAL("Время работы аппарата", "%.0lf", result.time_work);
    PRINTVAL("Время моделирования", "%.0lf", result.time_sim);
    NL;

    #define MEAN(t) ((params. t ## _from + params. t ## _to) / 2)
    #define DELTA(x, target) (fabs((x) - (target)) / (target) * 100)
    #define MAX(a, b) ((a) > (b) ? (a) : (b))
    #define MIN(a, b) ((a) < (b) ? (a) : (b))

    double t1 = MEAN(t1);
    double t2 = MEAN(t2);
    double t3 = MEAN(t3);
    double t4 = MEAN(t4);

    double enter_time_target1 = t1 * SIMULATION_LIMIT_Q1;
    double work_time_target1 = t3 * SIMULATION_LIMIT_Q1;

    double sim_time_target = MAX(enter_time_target1, work_time_target1);

    PRINTVAL("Ожидаемое время моделирования", "%.0lf", sim_time_target);
    PRINTVAL("Погрешность времени моделирования", "%.1lf%%", DELTA(result.time_sim, sim_time_target));
    NL;

    double in_order1_target = sim_time_target / t1;
    double in_order2_target = sim_time_target / t2;

    double work_time_target2 = in_order2_target * t4;

    PRINTVAL("Рассчетное количество вошедших заявок 1 типа", "%.0lf", in_order1_target);
    PRINTVAL("Рассчетное количество вошедших заявок 2 типа", "%.0lf", in_order2_target);
    PRINTVAL("Погрешность вошедших заявок 1 типа", "%.1lf%%", DELTA(result.in_orders1, in_order1_target));
    PRINTVAL("Погрешность вошедших заявок 2 типа", "%.1lf%%", DELTA(result.in_orders2, in_order2_target));
    NL;

    PRINTVAL("Рассчетное время работы", "%.0lf", work_time_target1 + work_time_target2);
    PRINTVAL("Погрешность времени работы", "%.1lf%%", DELTA(result.time_work, work_time_target1 + work_time_target2));
    NL;

    double wait_target_time = enter_time_target1 - work_time_target2 - work_time_target1;

    PRINTVAL("Рассчетное время простоя", "%.0lf", wait_target_time);

    if (wait_target_time < sim_time_target * 0.03)
    {
        PRINTVAL("Погрешность времени простоя", "%s", "-");
    }
    else
    {
        PRINTVAL("Погрешность времени простоя", "%.1lf%%", DELTA(result.time_wait, wait_target_time));
    }
}
