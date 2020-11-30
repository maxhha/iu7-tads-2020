#include "../inc/measure.h"

#ifndef __CURR_TICKS__
#define __CURR_TICKS__

extern __inline__ uint64_t curr_ticks() {
    uint64_t x;
    __asm__ volatile ("rdtsc\n\tshl $32, %%rdx\n\tor %%rdx, %%rax" : "=a" (x) : : "rdx");
    return x;
}

int check_add(uint64_t a, uint64_t b)
{
    uint64_t r = a + b;
    if (r < a || r < b)
        return -1;

    return 0;
}

#endif

#ifndef Q
#define Q queue_list
#endif

#define Q_T TMPL(Q, t)

int TMPL(measure_time_of, Q)(int n, uint64_t *push_ticks, uint64_t *pop_ticks)
{
    *push_ticks = 0;
    *pop_ticks = 0;

    int data = 0;
    uint64_t start, end;

    for (int r = 0; r < MEASURE_TIME_REPEATS; r++)
    {
        Q_T *q = TMPL(create, Q)(NULL);

        if (!q)
        {
            LOG_ERROR("не получилось создать очередь%s", "");
            return -1;
        }

        start = curr_ticks();

        for (int i = 0; i < n; i++)
        {
            if (TMPL(push, Q)(q, &data))
            {
                LOG_ERROR("не получилось добавить %d элемент в очередь", i);

                while (TMPL(pop, Q)(q))
                {
                }

                TMPL(free, Q)(q);

                return -1;
            }
        }

        end = curr_ticks();

        if (check_add(*push_ticks, end - start))
        {
            LOG_ERROR("переполнение сложения тиков\n");

            while (TMPL(pop, Q)(q))
            {
            }

            TMPL(free, Q)(q);

            return -1;
        }

        *push_ticks += end - start;

        start = curr_ticks();

        while (TMPL(pop, Q)(q))
        {
        }

        end = curr_ticks();

        if (check_add(*pop_ticks, end - start))
        {
            LOG_ERROR("переполнение сложения тиков\n");

            TMPL(free, Q)(q);

            return -1;
        }

        *pop_ticks += end - start;

        TMPL(free, Q)(q);
    }

    *push_ticks /= MEASURE_TIME_REPEATS;
    *pop_ticks /= MEASURE_TIME_REPEATS;

    return 0;
}

#undef Q_T
