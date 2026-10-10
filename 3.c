#include <stdio.h>
#include <limits.h>
#include <math.h>

typedef enum {
    OK = 0,
    INVALID_NUMBER,
    OUT_OF_RANGE,
    INVALID_FLAG,
    INVALID_COUNT
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

    if (number == 0)
        return OUT_OF_RANGE;

    *x = negative ? (int)(-number) : (int)number;

    return OK;
}

Status parse_real(const char *str, double *x) {
    char extra;

    if (str == NULL || x == NULL || str[0] == '\0')
        return INVALID_NUMBER;

    if (sscanf(str, "%lf %c", x, &extra) != 1)
        return INVALID_NUMBER;

    if (!isfinite(*x))
        return OUT_OF_RANGE;

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
        case 'q':
        case 'm':
        case 't':
            *flag = str[1];
            return OK;

        default:
            return INVALID_FLAG;
    }
}

Status check_multiple(int a, int b, int *result) {
    if (result == NULL || a == 0 || b == 0)
        return INVALID_NUMBER;

    if (b == -1)
        *result = 1;
    else
        *result = (a % b == 0);

    return OK;
}

Status check_triangle(double eps, double a, double b, double c, int *result) {
    if (result == NULL)
        return INVALID_NUMBER;

    if (!isfinite(eps) || eps <= 0)
        return OUT_OF_RANGE;

    if (!isfinite(a) || !isfinite(b) || !isfinite(c))
        return OUT_OF_RANGE;

    if (a <= eps || b <= eps || c <= eps)
        return OUT_OF_RANGE;

    if (a + b <= c + eps ||
        a + c <= b + eps ||
        b + c <= a + eps) {
        *result = 0;
        return OK;
    }

    double a2 = a * a;
    double b2 = b * b;
    double c2 = c * c;

    if (!isfinite(a2 + b2) || !isfinite(a2 + c2) || !isfinite(b2 + c2))
        return OUT_OF_RANGE;

    *result = fabs(a2 + b2 - c2) <= eps || fabs(a2 + c2 - b2) <= eps || fabs(b2 + c2 - a2) <= eps;

    return OK;
}

Status solve_equation(double eps, double a, double b, double c, int *type, double roots[2]) {
    if (type == NULL || roots == NULL)
        return INVALID_NUMBER;

    if (!isfinite(eps) || eps <= 0)
        return OUT_OF_RANGE;

    if (!isfinite(a) || !isfinite(b) || !isfinite(c))
        return OUT_OF_RANGE;

    if (fabs(a) <= eps) {
        if (fabs(b) <= eps) {
            if (fabs(c) <= eps)
                *type = 3;
            else
                *type = 0;
        } else {
            roots[0] = -c / b;
            *type = 1;
        }
    } else {
        double d = b * b - 4 * a * c;

        if (!isfinite(d))
            return OUT_OF_RANGE;

        if (d < -eps)
            *type = 0;
        else if (fabs(d) <= eps) {
            roots[0] = -b / (2 * a);
            *type = 1;
        } else {
            roots[0] = (-b - sqrt(d)) / (2 * a);
            roots[1] = (-b + sqrt(d)) / (2 * a);
            *type = 2;
        }
    }

    if (*type == 1 && !isfinite(roots[0]))
        return OUT_OF_RANGE;

    if (*type == 2 && (!isfinite(roots[0]) || !isfinite(roots[1])))
        return OUT_OF_RANGE;

    return OK;
}

Status solve_permutations(double eps, const double numbers[3], double coefficients[6][3], int types[6], double roots[6][2], int *count) {
    if (numbers == NULL || coefficients == NULL ||
        types == NULL || roots == NULL || count == NULL)
        return INVALID_NUMBER;

    if (!isfinite(eps) || eps <= 0)
        return OUT_OF_RANGE;

    for (int i = 0; i < 3; i++) {
        if (!isfinite(numbers[i]))
            return OUT_OF_RANGE;
    }

    *count = 0;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (i == j)
                continue;

            for (int k = 0; k < 3; k++) {
                if (k == i || k == j)
                    continue;

                double a = numbers[i];
                double b = numbers[j];
                double c = numbers[k];

                int repeat = 0;

                for (int p = 0; p < *count; p++) {
                    if (fabs(coefficients[p][0] - a) <= eps && fabs(coefficients[p][1] - b) <= eps && fabs(coefficients[p][2] - c) <= eps) {
                        repeat = 1;
                        break;
                    }
                }

                if (repeat)
                    continue;

                coefficients[*count][0] = a;
                coefficients[*count][1] = b;
                coefficients[*count][2] = c;

                Status status = solve_equation(eps, a, b, c, &types[*count], roots[*count]);

                if (status != OK)
                    return status;

                (*count)++;
            }
        }
    }

    return OK;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Ошибка: необходимо указать флаг.\n");
        return 1;
    }

    char flag;
    Status status = parse_flag(argv[1], &flag);

    if (status != OK) {
        printf("Ошибка: неизвестный флаг.\n");
        return 1;
    }

    int required_count = 6;

    if (flag == 'm')
        required_count = 4;

    if (argc != required_count) {
        printf("Ошибка: неверное количество аргументов.\n");
        return 1;
    }

    switch (flag) {
        case 'm': {
            int a, b;
            int result;

            status = parse_number(argv[2], &a);

            if (status == OK)
                status = parse_number(argv[3], &b);

            if (status == OK)
                status = check_multiple(a, b, &result);

            if (status == OK) {
                if (result)
                    printf("%d кратно %d\n", a, b);
                else
                    printf("%d не кратно %d\n", a, b);
            }

            break;
        }

        case 't': {
            double eps, a, b, c;
            int result;

            status = parse_real(argv[2], &eps);

            if (status == OK)
                status = parse_real(argv[3], &a);

            if (status == OK)
                status = parse_real(argv[4], &b);

            if (status == OK)
                status = parse_real(argv[5], &c);

            if (status == OK)
                status = check_triangle(eps, a, b, c, &result);

            if (status == OK) {
                if (result)
                    printf("Треугольник прямоугольный.\n");
                else
                    printf("Треугольник не прямоугольный.\n");
            }

            break;
        }

        case 'q': {
            double eps;
            double numbers[3];
            double coefficients[6][3];
            double roots[6][2];
            int types[6];
            int count = 0;

            status = parse_real(argv[2], &eps);

            for (int i = 0; i < 3 && status == OK; i++)
                status = parse_real(argv[i + 3], &numbers[i]);

            if (status == OK)
                status = solve_permutations(eps, numbers, coefficients, types, roots, &count);

            if (status == OK) {
                for (int i = 0; i < count; i++) {
                    printf("\n%.10g*x^2 + %.10g*x + %.10g = 0\n", coefficients[i][0], coefficients[i][1], coefficients[i][2]);

                    switch (types[i]) {
                        case 0:
                            printf("Нет действительных корней.\n");
                            break;

                        case 1:
                            printf("x = %.10g\n", roots[i][0]);
                            break;

                        case 2:
                            printf("x1 = %.10g\n", roots[i][0]);
                            printf("x2 = %.10g\n", roots[i][1]);
                            break;

                        case 3:
                            printf("Бесконечно много решений.\n");
                            break;
                    }
                }
            }

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

            default:
                printf("Ошибка вычисления.\n");
                break;
        }

        return 1;
    }

    return 0;
}