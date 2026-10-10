#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#define SIZE 10

typedef enum {
    OK = 0,
    INVALID_NUMBER,
    OUT_OF_RANGE,
    MEMORY_ERROR,
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

Status fill_array(int arr[], int size, int a, int b) {
    if (arr == NULL || size <= 0)
        return INVALID_NUMBER;

    if (a > b)
        return OUT_OF_RANGE;

    long long range = (long long)b - a + 1;

    for (int i = 0; i < size; i++) {
        long long offset = (long long)((double)rand() / ((double)RAND_MAX + 1.0) * range);

        arr[i] = (int)((long long)a + offset);
    }

    return OK;
}

Status swap_min_max(int arr[], int size, int *min_value, int *max_value) {
    if (arr == NULL || min_value == NULL || max_value == NULL || size <= 0)
        return INVALID_NUMBER;

    int min_index = 0;
    int max_index = 0;

    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[min_index])
            min_index = i;

        if (arr[i] > arr[max_index])
            max_index = i;
    }

    *min_value = arr[min_index];
    *max_value = arr[max_index];

    int temp = arr[min_index];
    arr[min_index] = arr[max_index];
    arr[max_index] = temp;

    return OK;
}

Status create_array_c(const int A[], int size_a, const int B[], int size_b, int C[]) {
    if (A == NULL || B == NULL || C == NULL || size_a <= 0 || size_b <= 0)
        return INVALID_NUMBER;

    for (int i = 0; i < size_a; i++) {
        int closest = B[0];
        long long min_diff = (long long)A[i] - B[0];

        if (min_diff < 0)
            min_diff = -min_diff;

        for (int j = 1; j < size_b; j++) {
            long long diff = (long long)A[i] - B[j];

            if (diff < 0)
                diff = -diff;

            if (diff < min_diff) {
                min_diff = diff;
                closest = B[j];
            }
        }

        long long sum = (long long)A[i] + closest;

        if (sum < INT_MIN || sum > INT_MAX)
            return OVERFLOW;

        C[i] = (int)sum;
    }

    return OK;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Ошибка: необходимо два аргумента.\n");
        return 1;
    }

    int a, b;
    int arr[SIZE];
    int min_value, max_value;

    Status status = parse_number(argv[1], &a);

    if (status == OK)
        status = parse_number(argv[2], &b);

    if (status == OK && a > b)
        status = OUT_OF_RANGE;

    if (status != OK) {
        if (status == INVALID_NUMBER)
            printf("Ошибка: некорректное число.\n");
        else
            printf("Ошибка: неверный диапазон.\n");

        return 1;
    }

    srand((unsigned int)time(NULL));

    status = fill_array(arr, SIZE, a, b);

    if (status != OK) {
        printf("Ошибка заполнения массива.\n");
        return 1;
    }

    printf("Задание 9.1\n");
    printf("Исходный массив:\n");

    for (int i = 0; i < SIZE; i++)
        printf("%d ", arr[i]);

    printf("\n");

    status = swap_min_max(arr, SIZE, &min_value, &max_value);

    if (status != OK) {
        printf("Ошибка обработки массива.\n");
        return 1;
    }

    printf("Минимум: %d\n", min_value);
    printf("Максимум: %d\n", max_value);

    printf("Массив после обмена:\n");

    for (int i = 0; i < SIZE; i++)
        printf("%d ", arr[i]);

    printf("\n\n");

    int size_a = 10 + rand() % 9991;
    int size_b = 10 + rand() % 9991;

    int *A = malloc(size_a * sizeof(int));
    int *B = malloc(size_b * sizeof(int));
    int *C = malloc(size_a * sizeof(int));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Ошибка выделения памяти.\n");

        free(A);
        free(B);
        free(C);

        return 1;
    }

    status = fill_array(A, size_a, -1000, 1000);

    if (status == OK)
        status = fill_array(B, size_b, -1000, 1000);

    if (status == OK)
        status = create_array_c(A, size_a, B, size_b, C);

    if (status != OK) {
        printf("Ошибка создания массивов.\n");

        free(A);
        free(B);
        free(C);

        return 1;
    }

    printf("Задание 9.2\n");
    printf("Размер A: %d\n", size_a);
    printf("Размер B: %d\n", size_b);
    printf("Размер C: %d\n\n", size_a);

    printf("Первые 10 элементов A:\n");

    for (int i = 0; i < 10; i++)
        printf("%d ", A[i]);

    printf("\nПервые 10 элементов B:\n");

    for (int i = 0; i < 10; i++)
        printf("%d ", B[i]);

    printf("\nПервые 10 элементов C:\n");

    for (int i = 0; i < 10; i++)
        printf("%d ", C[i]);

    printf("\n\nПроверка первых 10 элементов C:\n");

    for (int i = 0; i < 10; i++)
        printf("%d + %d = %d\n", A[i], C[i] - A[i], C[i]);

    free(A);
    free(B);
    free(C);

    return 0;
}