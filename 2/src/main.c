#include "../inc/main.h"

int main(int argc, char const **argv)
{
    car_t *car_table = NULL;

    int rc = run_menu(&car_table);

    vec_free(car_table);

    return rc == OK ? EXIT_SUCCESS : EXIT_FAILURE;
}
