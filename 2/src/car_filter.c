#include "../inc/car_filter.h"

void filter_cars(car_t *car_table, size_t *car_table_size, char *brand, long int price_from, long int price_to)
{
    car_t *j = car_table;
    car_t *end = car_table + *car_table_size;
    for (car_t *i = car_table; i < end; i++)
    {
        if (strcmp(i->brand, brand) == 0 &&
            i->price >= price_from &&
            i->price <= price_to &&
            !i->is_new &&
            i->state.old_info.repairs_n == 0 &&
            i->state.old_info.owners_n == 1)
        {
            if (j != i)
                *j = *i;
            j++;
        }
    }
    *car_table_size = j - car_table;
}
