#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FIXED_SIZE 20

void process_fixed_array(int arr[], const int size) {
    int min_idx = 0, max_idx = 0;
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[min_idx]) min_idx = i;
        if (arr[i] > arr[max_idx]) max_idx = i;
    }
    int temp = arr[min_idx];
    arr[min_idx] = arr[max_idx];
    arr[max_idx] = temp;
}

int find_closest(const int target, const int arr[], const int size) {
    int closest = arr[0];
    int min_diff = abs(target - arr[0]);
    for (int i = 1; i < size; i++) {
        int diff = abs(target - arr[i]);
        if (diff < min_diff) {
            min_diff = diff;
            closest = arr[i];
        }
    }
    return closest;
}

void print_array(const char *label, const int arr[], const int size) {
    printf("%s: ", label);
    for (int i = 0; i < size && i < 10; i++) {
        printf("%d ", arr[i]);
    }
    if (size > 10) printf("...");
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: usage: %s <a> <b>\n", argv[0]);
        return 1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    if (a > b) {
        printf("Error: a must be <= b.\n");
        return 1;
    }

    srand(time(NULL));

    printf("Part 1: Fixed-size array\n");
    int fixed_arr[FIXED_SIZE];
    for (int i = 0; i < FIXED_SIZE; i++) {
        fixed_arr[i] = rand() % (b - a + 1) + a;
    }

    print_array("Before", fixed_arr, FIXED_SIZE);
    process_fixed_array(fixed_arr, FIXED_SIZE);
    print_array("After ", fixed_arr, FIXED_SIZE);

    printf("\nPart 2: Dynamic arrays\n");
    int n = rand() % (10000 - 10 + 1) + 10;
    printf("Random size N = %d\n", n);

    int *A = malloc(n * sizeof(int));
    int *B = malloc(n * sizeof(int));
    int *C = malloc(n * sizeof(int));

    if (A == NULL || B == NULL || C == NULL) {
        printf("Error: memory allocation failed.\n");
        free(A);
        free(B);
        free(C);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        A[i] = rand() % 2001 - 1000;
        B[i] = rand() % 2001 - 1000;
    }

    for (int i = 0; i < n; i++) {
        C[i] = A[i] + find_closest(A[i], B, n);
    }

    print_array("A", A, n);
    print_array("B", B, n);
    print_array("C", C, n);

    free(A);
    free(B);
    free(C);

    return 0;
}