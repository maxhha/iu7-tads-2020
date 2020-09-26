#include "../inc/main.h"

int main(int argc, char const **argv)
{
    car_t car;

    if (read_car(stdin, stdout, &car) != OK)
    {
        printf("Неправильный ввод");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
