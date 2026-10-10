#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_LENGTH 256

typedef enum {
    OK = 0,
    INVALID_NUMBER,
    OUT_OF_RANGE,
    FILE_ERROR,
    TOO_LONG
} Status;

Status get_digit(char symbol, int *digit) {
    if (digit == NULL)
        return INVALID_NUMBER;

    if (symbol >= '0' && symbol <= '9')
        *digit = symbol - '0';
    else if (symbol >= 'A' && symbol <= 'Z')
        *digit = symbol - 'A' + 10;
    else if (symbol >= 'a' && symbol <= 'z')
        *digit = symbol - 'a' + 10;
    else
        return INVALID_NUMBER;

    return OK;
}

Status convert_number(const char *str, char *cleaned, int *base, long long *result) {
    if (str == NULL || cleaned == NULL || base == NULL || result == NULL)
        return INVALID_NUMBER;

    int start = 0;
    int negative = 0;
    int max_digit = 0;

    if (str[0] == '-' || str[0] == '+') {
        negative = (str[0] == '-');
        start = 1;
    }

    if (str[start] == '\0')
        return INVALID_NUMBER;

    for (int i = start; str[i] != '\0'; i++) {
        int digit;
        Status status = get_digit(str[i], &digit);

        if (status != OK)
            return status;

        if (digit > max_digit)
            max_digit = digit;
    }

    *base = max_digit + 1;

    if (*base < 2)
        *base = 2;

    unsigned long long number = 0;
    unsigned long long limit = (unsigned long long)LLONG_MAX;

    if (negative)
        limit++;

    for (int i = start; str[i] != '\0'; i++) {
        int digit;
        Status status = get_digit(str[i], &digit);

        if (status != OK)
            return status;

        if (number > (limit - digit) / *base)
            return OUT_OF_RANGE;

        number = number * *base + digit;
    }

    if (negative) {
        if (number == (unsigned long long)LLONG_MAX + 1)
            *result = LLONG_MIN;
        else
            *result = -(long long)number;
    } else {
        *result = (long long)number;
    }

    int i = start;

    while (str[i] == '0')
        i++;

    if (str[i] == '\0') {
        cleaned[0] = '0';
        cleaned[1] = '\0';
    } else {
        int j = 0;

        if (negative)
            cleaned[j++] = '-';

        while (str[i] != '\0')
            cleaned[j++] = str[i++];

        cleaned[j] = '\0';
    }

    return OK;
}

Status read_number(FILE *file, char *str, int *found) {
    if (file == NULL || str == NULL || found == NULL)
        return FILE_ERROR;

    int read = fscanf(file, "%255s", str);

    if (read == EOF) {
        *found = 0;

        if (ferror(file))
            return FILE_ERROR;

        return OK;
    }

    int next = fgetc(file);

    if (next != EOF && !isspace((unsigned char)next))
        return TOO_LONG;

    if (ferror(file))
        return FILE_ERROR;

    *found = 1;
    return OK;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Ошибка: необходимо указать два файла.\n");
        return 1;
    }

    if (strcmp(argv[1], argv[2]) == 0) {
        printf("Ошибка: входной и выходной файлы совпадают.\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");

    if (input == NULL) {
        printf("Ошибка открытия входного файла.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");

    if (output == NULL) {
        printf("Ошибка открытия выходного файла.\n");
        fclose(input);
        return 1;
    }

    char number[MAX_LENGTH];
    char cleaned[MAX_LENGTH];
    int base;
    int found;
    long long result;

    Status status = OK;

    while (status == OK) {
        status = read_number(input, number, &found);

        if (status != OK || !found)
            break;

        status = convert_number(number, cleaned, &base, &result);

        if (status != OK)
            break;

        if (fprintf(output, "%s %d %lld\n", cleaned, base, result) < 0)
            status = FILE_ERROR;
    }

    if (fclose(input) != 0 && status == OK)
        status = FILE_ERROR;

    if (fclose(output) != 0 && status == OK)
        status = FILE_ERROR;

    if (status != OK) {
        switch (status) {
            case INVALID_NUMBER:
                printf("Ошибка: некорректное число в файле.\n");
                break;

            case OUT_OF_RANGE:
                printf("Ошибка: число слишком большое.\n");
                break;

            case FILE_ERROR:
                printf("Ошибка работы с файлом.\n");
                break;

            case TOO_LONG:
                printf("Ошибка: слишком длинное число.\n");
                break;

            default:
                printf("Неизвестная ошибка.\n");
                break;
        }

        return 1;
    }

    printf("Данные успешно записаны в файл %s\n", argv[2]);

    return 0;
}
