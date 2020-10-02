#include "../inc/main.h"

int main(int argc, char const **argv)
{
    car_t car_table[MAX_TABLE_SIZE];
    size_t car_table_size = 0;

    int rc = run_menu(car_table, &car_table_size);

    return rc == OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
