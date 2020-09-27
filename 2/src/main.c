#include "../inc/main.h"

int main(int argc, char const **argv)
{
    car_t car = {
        .brand = "",
        .country = "",
        .price = 10,
        .color = "red",
        .is_new = 1,
        .state = {
            .new_info = {
                .guarantee = 20,
            },
        },
    };
    //
    // if (read_car(stdin, stdout, &car) != OK)
    // {
    //     printf("Неправильный ввод");
    //     return EXIT_FAILURE;
    // }

    print_car_table_header();
    print_car_table_row(0, &car);

    return EXIT_SUCCESS;
}
