
#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX_LENGTH 256

typedef enum {
    OK = 0,
    INVALID_NUMBER,
    OUT_OF_RANGE,
    OVERFLOW
} Status;

Status parse_number(const char *str, int *x) {
    long long number = 0;
    int i = 0;
    int negative = 0;

    if (str == NULL || x == NULL || str[0] == '\0')
        return INVALID_NUMBER;

    if (str[0] == '-' || str[0] == '+') {
        negative = (str[0] == '-');
        i = 1;
    }

    if (str[i] == '\0')
        return INVALID_NUMBER;

    long long limit = negative ? -(long long)INT_MIN : INT_MAX;

    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9')
            return INVALID_NUMBER;

        int digit = str[i] - '0';

        if (number > (limit - digit) / 10)
            return OUT_OF_RANGE;

        number = number * 10 + digit;
    }

    *x = negative ? (int)(-number) : (int)number;

    return OK;
}

Status get_digit(char symbol, int *digit) {
    if (digit == NULL)
        return INVALID_NUMBER;

    if (symbol >= '0' && symbol <= '9')
        *digit = symbol - '0';
    else if (symbol >= 'A' && symbol <= 'Z')
        *digit = symbol - 'A' + 10;
    else
        return INVALID_NUMBER;

    return OK;
}

Status from_base(const char *str, int base, long long *result) {
    if (str == NULL || result == NULL)
        return INVALID_NUMBER;

    if (base < 2 || base > 36)
        return OUT_OF_RANGE;

    int i = 0;
    int negative = 0;

    if (str[0] == '-' || str[0] == '+') {
        negative = (str[0] == '-');
        i = 1;
    }

    if (str[i] == '\0')
        return INVALID_NUMBER;

    unsigned long long number = 0;
    unsigned long long limit = LLONG_MAX;

    if (negative)
        limit++;

    for (; str[i] != '\0'; i++) {
        int digit;

        if (get_digit(str[i], &digit) != OK)
            return INVALID_NUMBER;

        if (digit >= base)
            return INVALID_NUMBER;

        if (number > (limit - digit) / base)
            return OVERFLOW;

        number = number * base + digit;
    }

    if (negative && number == (unsigned long long)LLONG_MAX + 1)
        *result = LLONG_MIN;
    else if (negative)
        *result = -(long long)number;
    else
        *result = (long long)number;

    return OK;
}

Status update_results(long long number, long long *sum, long long *maximum, unsigned long long *max_abs, int *count) {
    if (sum == NULL || maximum == NULL || max_abs == NULL || count == NULL)
        return INVALID_NUMBER;

    if (number > 0 && *sum > LLONG_MAX - number)
        return OVERFLOW;

    if (number < 0 && *sum < LLONG_MIN - number)
        return OVERFLOW;

    unsigned long long absolute;

    if (number < 0)
        absolute = (unsigned long long)(-(number + 1)) + 1;
    else
        absolute = (unsigned long long)number;

    if (*count == 0 || absolute > *max_abs) {
        *max_abs = absolute;
        *maximum = number;
    }

    *sum += number;
    (*count)++;

    return OK;
}

Status to_base(long long number, int base, char *str, int size) {
    if (str == NULL || size < 2)
        return INVALID_NUMBER;

    if (base < 2 || base > 36)
        return OUT_OF_RANGE;

    const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned long long value;

    if (number < 0)
        value = (unsigned long long)(-(number + 1)) + 1;
    else
        value = (unsigned long long)number;

    int count = 0;

    do {
        if (count >= size - 1)
            return OUT_OF_RANGE;

        str[count++] = digits[value % base];
        value /= base;
    } while (value > 0);

    if (number < 0) {
        if (count >= size - 1)
            return OUT_OF_RANGE;

        str[count++] = '-';
    }

    str[count] = '\0';

    for (int i = 0; i < count / 2; i++) {
        char temp = str[i];
        str[i] = str[count - 1 - i];
        str[count - 1 - i] = temp;
    }

    return OK;
}

int main(void) {
    char str[MAX_LENGTH];
    int base;

    printf("Введите основание системы счисления (2-36):\n");

    if (scanf("%255s", str) != 1) {
        printf("Ошибка ввода.\n");
        return 1;
    }

    if (strlen(str) == MAX_LENGTH - 1) {
        printf("Ошибка: слишком длинное число.\n");
        return 1;
    }

    Status status = parse_number(str, &base);

    if (status != OK || base < 2 || base > 36) {
        printf("Ошибка: основание должно быть от 2 до 36.\n");
        return 1;
    }

    long long number;
    long long sum = 0;
    long long maximum = 0;
    unsigned long long max_abs = 0;
    int count = 0;
    int stopped = 0;

    printf("Введите числа. Для завершения введите Stop:\n");

    while (scanf("%255s", str) == 1) {
        if (strlen(str) == MAX_LENGTH - 1) {
            status = OUT_OF_RANGE;
            break;
        }

        if (strcmp(str, "Stop") == 0) {
            stopped = 1;
            break;
        }

        status = from_base(str, base, &number);

        if (status != OK)
            break;

        status = update_results(number, &sum, &maximum, &max_abs, &count);

        if (status != OK)
            break;
    }

    if (status != OK) {
        switch (status) {
            case INVALID_NUMBER:
                printf("Ошибка: некорректное число.\n");
                break;

            case OUT_OF_RANGE:
                printf("Ошибка: число вне диапазона.\n");
                break;

            case OVERFLOW:
                printf("Ошибка: переполнение.\n");
                break;

            default:
                printf("Ошибка выполнения.\n");
                break;
        }

        return 1;
    }

    if (!stopped) {
        printf("Ошибка: ввод должен завершаться словом Stop.\n");
        return 1;
    }

    if (count == 0) {
        printf("Ошибка: не введено ни одного числа.\n");
        return 1;
    }

    int bases[5] = {base, 9, 18, 27, 36};
    long long results[2] = {maximum, sum};

    char converted[70];

    for (int i = 0; i < 2; i++) {
        if (i == 0)
            printf("\nМаксимальное по модулю число:\n");
        else
            printf("\nСумма чисел:\n");

        for (int j = 0; j < 5; j++) {
            status = to_base(results[i], bases[j], converted, sizeof(converted));

            if (status != OK) {
                printf("Ошибка преобразования.\n");
                return 1;
            }

            printf("Основание %d: %s\n", bases[j], converted);
        }
    }

    return 0;
}
