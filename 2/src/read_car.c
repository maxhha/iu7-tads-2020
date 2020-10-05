#include "../inc/read_car.h"

/*
    Считывает из потока информацию о состоянии новой машины
*/
int read_car_state_new(car_state_new_t *info)
{
    printf("Гарантия (в целых годах): ");
    if (scanf("%d", &info->guarantee) != 1 || info->guarantee < 0)
    {
        printf(RED "Неправильная гаранития.\n" RESET);
        return EREAD;
    }

    return OK;
}

/*
    Считывает из потока информацию о состоянии старой машины
*/
int read_car_state_old(car_state_old_t *new_info)
{
    printf("Год выпуска: ");
    if (scanf("%d", &new_info->year) != 1 || new_info->year < 0)
    {
        printf(RED "Неправильный год.\n" RESET);
        return EREAD;
    }

    printf("Пробег (в целых км): ");
    if (scanf("%d", &new_info->mileage) != 1 || new_info->mileage < 0)
    {
        printf(RED "Неправильный пробег.\n" RESET);
        return EREAD;
    }

    printf("Количество ремонтов: ");
    if (scanf("%d", &new_info->repairs_n) != 1 || new_info->repairs_n < 0)
    {
        printf(RED "Неправильное количество ремонтов.\n" RESET);
        return EREAD;
    }

    printf("Количество собственников: ");
    if (scanf("%d", &new_info->owners_n) != 1 || new_info->owners_n < 0)
    {
            printf(RED "Неправильное количество собственников.\n" RESET);
        return EREAD;
    }

    return OK;
}

/*
    Заполняет поля car из потока fin.
    Пишет приглашения в fout если он не NULL

    @param fin файл ввода
    @param fout файл приглашений
    @return код ошибки OK EREAD
*/
int read_car(car_t *car)
{
    char c;

    printf("Марка: ");
    if (fgetline(car->brand, CAR_BRAND_LEN + 1, stdin) == NULL)
    {
        printf(RED "Слишком длинная марка.\n" RESET);
        return EREAD;
    }

    printf("Страна-производитель: ");
    if (fgetline(car->country, CAR_COUNTRY_LEN + 1, stdin) == NULL)
    {
        printf(RED "Слишком длинная страна.\n" RESET);
        return EREAD;
    }

    printf("Цена: ");
    if (scanf("%d%c", &car->price, &c) != 2 || car->price < 0)
    {
        printf(RED "Неправильная цена.\n" RESET);
        return EREAD;
    }

    if (c != '\n')
        wait_new_line();

    printf("Цвет: ");
    if (fgetline(car->color, CAR_COLOR_LEN + 1, stdin) == NULL)
    {
        printf(RED "Слишком длинный цвет.\n" RESET);
        return EREAD;
    }

    printf("Состояние (0 - старая, 1 - новая): ");
    if (scanf("%d", (int *) &car->is_new) != 1 || (car->is_new != 0 && car->is_new != 1))
    {
        printf(RED "Неправильное состояние.\n" RESET);
        return EREAD;
    }

    if (car->is_new)
    {
        if (read_car_state_new(&car->state.new_info) != OK)
            return EREAD;
    }
    else
    {
        if (read_car_state_old(&car->state.old_info) != OK)
            return EREAD;
    }

    wait_new_line();

    return OK;
}
