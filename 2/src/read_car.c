#include "../inc/read_car.h"

/*
    Считывает из потока информацию о состоянии новой машины
*/
int read_car_state_new(FILE *fin, FILE *fout, car_state_new_t *info)
{
    xfprintf(fout, "Гарантия (в целых годах): ");
    if (fscanf(fin, "%d", &info->guarantee) != 1 || info->guarantee < 0)
        return EREAD;


    return OK;
}

/*
    Считывает из потока информацию о состоянии старой машины
*/
int read_car_state_old(FILE *fin, FILE *fout, car_state_old_t *new_info)
{
    xfprintf(fout, "Год выпуска: ");
    if (fscanf(fin, "%d", &new_info->year) != 1 || new_info->year < 0)
        return EREAD;

    xfprintf(fout, "Пробег (в целых км): ");
    if (fscanf(fin, "%d", &new_info->mileage) != 1 || new_info->mileage < 0)
        return EREAD;

    xfprintf(fout, "Количество ремонтов: ");
    if (fscanf(fin, "%d", &new_info->repairs_n) != 1 || new_info->repairs_n < 0)
        return EREAD;

    xfprintf(fout, "Количество собственников: ");
    if (fscanf(fin, "%d", &new_info->owners_n) != 1 || new_info->owners_n < 0)
        return EREAD;

    return OK;
}

/*
    Заполняет поля car из потока fin.
    Пишет приглашения в fout если он не NULL

    @param fin файл ввода
    @param fout файл приглашений
    @return код ошибки OK EREAD
*/
int read_car(FILE *fin, FILE *fout, car_t *car)
{
    char c;

    xfprintf(fout, "Марка: ");
    if (fgetline(car->brand, CAR_BRAND_LEN + 1, fin) == NULL)
        return EREAD;

    xfprintf(fout, "Страна-производитель: ");
    if (fgetline(car->country, CAR_COUNTRY_LEN + 1, fin) == NULL)
        return EREAD;

    xfprintf(fout, "Цена: ");
    if (fscanf(fin, "%ld%c", &car->price, &c) != 2 || car->price < 0)
        return EREAD;

    if (c != '\n')
        wait_new_line();

    xfprintf(fout, "Цвет: ");
    if (fgetline(car->color, CAR_COLOR_LEN + 1, fin) == NULL)
        return EREAD;

    xfprintf(fout, "Состояние (0 - старая, 1 - новая): ");
    if (fscanf(fin, "%d", (int *) &car->is_new) != 1)
        return EREAD;

    if (car->is_new)
    {
        if (read_car_state_new(fin, fout, &car->state.new_info) != OK)
            return EREAD;
    }
    else
    {
        if (read_car_state_old(fin, fout, &car->state.old_info) != OK)
            return EREAD;
    }

    wait_new_line();

    return OK;
}
