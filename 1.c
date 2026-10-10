#include <stdio.h>
#include <limits.h>

typedef enum {
    OK = 0,
    INVALID_NUMBER,
    OUT_OF_RANGE,
    INVALID_FLAG,
    OVERFLOW
} Status;

Status parse_number(const char *str, int *x) {
    int number = 0;
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

    for (; str[i] != '\0'; i++) {
        if (str[i] < '0' || str[i] > '9')
            return INVALID_NUMBER;

        int digit = str[i] - '0';

        if (number > (INT_MAX - digit) / 10)
            return OUT_OF_RANGE;

        number = number * 10 + digit;
    }

    if (negative || number == 0)
        return OUT_OF_RANGE;

    *x = number;
    return OK;
}

Status parse_flag(const char *str, char *flag) {
    if (str == NULL || flag == NULL)
        return INVALID_FLAG;

    if (str[0] != '-' && str[0] != '/')
        return INVALID_FLAG;

    if (str[1] == '\0' || str[2] != '\0')
        return INVALID_FLAG;

    switch (str[1]) {
        case 'h':
        case 'p':
        case 's':
        case 'e':
        case 'a':
        case 'f':
            *flag = str[1];
            return OK;

        default:
            return INVALID_FLAG;
    }
}

Status find_multiples(int x, int numbers[100], int *count) {
    if (numbers == NULL || count == NULL)
        return INVALID_NUMBER;

    if (x < 1)
        return OUT_OF_RANGE;

    *count = 0;

    for (int i = 1; i <= 100; i++) {
        if (i % x == 0) {
            numbers[*count] = i;
            (*count)++;
        }
    }

    return OK;
}

Status check_prime(int x, int *result) {
    if (result == NULL)
        return INVALID_NUMBER;

    if (x < 1)
        return OUT_OF_RANGE;

    if (x == 1) {
        *result = 0;
        return OK;
    }

    *result = 1;

    for (int i = 2; i <= x / i; i++) {
        if (x % i == 0) {
            *result = 2;
            break;
        }
    }

    return OK;
}

Status to_hex(int x, char *hex, int size) {
    if (hex == NULL || size < 2)
        return INVALID_NUMBER;

    if (x < 1)
        return OUT_OF_RANGE;

    const char digits[] = "0123456789ABCDEF";
    int count = 0;

    while (x > 0) {
        if (count >= size - 1)
            return OUT_OF_RANGE;

        hex[count] = digits[x % 16];
        count++;
        x /= 16;
    }

    hex[count] = '\0';

    for (int i = 0; i < count / 2; i++) {
        char temp = hex[i];
        hex[i] = hex[count - 1 - i];
        hex[count - 1 - i] = temp;
    }

    return OK;
}

Status calculate_powers(int x, long long table[10][10]) {
    if (table == NULL)
        return INVALID_NUMBER;

    if (x < 1 || x > 10)
        return OUT_OF_RANGE;

    for (int base = 1; base <= 10; base++) {
        long long power = 1;

        for (int exponent = 1; exponent <= x; exponent++) {
            if (power > LLONG_MAX / base)
                return OVERFLOW;

            power *= base;
            table[base - 1][exponent - 1] = power;
        }
    }

    return OK;
}

Status calculate_sum(int x, long long *result) {
    if (result == NULL)
        return INVALID_NUMBER;

    if (x < 1)
        return OUT_OF_RANGE;

    *result = (long long)x * (x + 1LL) / 2;

    return OK;
}

Status calculate_factorial(int x, long long *result) {
    if (result == NULL)
        return INVALID_NUMBER;

    if (x < 1)
        return OUT_OF_RANGE;

    long long factorial = 1;

    for (int i = 2; i <= x; i++) {
        if (factorial > LLONG_MAX / i)
            return OVERFLOW;

        factorial *= i;
    }

    *result = factorial;
    return OK;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Ошибка: необходимо два аргумента.\n");
        return 1;
    }

    int x = 0;
    char flag = '\0';
    Status status;

    long long result = 0;
    int numbers[100];
    int count = 0;
    int prime_result = 0;
    char hex[20];
    long long table[10][10];

    status = parse_number(argv[1], &x);

    if (status == OK)
        status = parse_flag(argv[2], &flag);

    if (status == OK) {
        switch (flag) {
            case 'h':
                status = find_multiples(x, numbers, &count);
                break;

            case 'p':
                status = check_prime(x, &prime_result);
                break;

            case 's':
                status = to_hex(x, hex, sizeof(hex));
                break;

            case 'e':
                status = calculate_powers(x, table);
                break;

            case 'a':
                status = calculate_sum(x, &result);
                break;

            case 'f':
                status = calculate_factorial(x, &result);
                break;

            default:
                status = INVALID_FLAG;
                break;
        }
    }

    if (status != OK) {
        switch (status) {
            case INVALID_NUMBER:
                printf("Ошибка: некорректное число.\n");
                break;

            case OUT_OF_RANGE:
                printf("Ошибка: число вне допустимого диапазона.\n");
                break;

            case INVALID_FLAG:
                printf("Ошибка: неизвестный флаг.\n");
                break;

            case OVERFLOW:
                printf("Ошибка: переполнение.\n");
                break;

            default:
                printf("Неизвестная ошибка.\n");
                break;
        }

        return 1;
    }

    switch (flag) {
        case 'h':
            if (count == 0) {
                printf("Кратных чисел нет.\n");
            } else {
                printf("Числа, кратные %d:\n", x);

                for (int i = 0; i < count; i++)
                    printf("%d ", numbers[i]);

                printf("\n");
            }
            break;

        case 'p':
            if (prime_result == 0)
                printf("Число 1 не является ни простым, ни составным.\n");
            else if (prime_result == 1)
                printf("Число %d простое.\n", x);
            else
                printf("Число %d составное.\n", x);
            break;

        case 's':
            printf("Шестнадцатеричные цифры: ");

            for (int i = 0; hex[i] != '\0'; i++)
                printf("%c ", hex[i]);

            printf("\n");
            break;

        case 'e':
            printf("Base/Exp");

            for (int i = 1; i <= x; i++)
                printf("%13d", i);

            printf("\n");

            for (int base = 1; base <= 10; base++) {
                printf("%8d", base);

                for (int i = 0; i < x; i++)
                    printf("%13lld", table[base - 1][i]);

                printf("\n");
            }
            break;

        case 'a':
            printf("Сумма = %lld\n", result);
            break;

        case 'f':
            printf("Факториал = %lld\n", result);
            break;
    }

    return 0;
}