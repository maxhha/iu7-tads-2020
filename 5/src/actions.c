#include "../inc/actions.h"

action_params_t init_action_params(void)
{
    action_params_t p = {
        .mem = NULL,
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
    free(buf);

    if ((x == 1 && params->mem) || (x == 2 && !params->mem))
        return;

    if (x == 1)
        params->mem = create_memory_watch();
    else
    {
        free_memory_watch(params->mem);
        params->mem = NULL;
    }
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
    printf("Чтение: %lu тиков\n", push_ticks);
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
    printf("Вошло заявок 1 типа: " YEL "%d" RESET "\n", result.in_orders1);
    printf("Вошло заявок 2 типа: " YEL "%d" RESET "\n", result.in_orders2);
    printf("Вышло заявок 1 типа: " YEL "%d" RESET "\n", result.out_orders1);
    printf("Вышло заявок 2 типа: " YEL "%d" RESET "\n", result.out_orders2);
    printf("Время ожидания аппарата: " YEL "%.0lf" RESET "\n", result.time_wait);
    printf("Время работы аппарата:   " YEL "%.0lf" RESET "\n", result.time_work);
    printf("Время моделирования:     " YEL "%.0lf" RESET "\n", result.time_sim);
}
