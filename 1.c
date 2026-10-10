#include <stdio.h>

typedef unsigned long long ull;

int FlagH(const ull x) {
    int found = 0;
    printf("Multiples of %llu up to 100: ", x);
    for (ull i = x; i <= 100; i += x) {
        printf("%llu ", i);
        found = 1;
    }
    if (!found) {
        printf("No multiples found.");
    }
    printf("\n");
    return 0;
}

int FlagP(const ull x) {
    if (x < 2) {
        printf("%llu is neither prime nor composite.\n", x);
        return 0;
    }
    int prime = 1;
    if (x > 2 && x % 2 == 0) {
        prime = 0;
    } else {
        for (ull i = 3; i * i <= x; i += 2) {
            if (x % i == 0) {
                prime = 0;
                break;
            }
        }
    }
    if (prime) {
        printf("%llu is prime.\n", x);
    } else {
        printf("%llu is composite.\n", x);
    }
    return 0;
}

int FlagS(const ull x) {
    char buf[64];
    sprintf(buf, "%llX", x);
    printf("Hex digits: ");
    for (int i = 0; buf[i] != '\0'; i++) {
        printf("%c", buf[i]);
        if (buf[i + 1] != '\0') {
            printf(" ");
        }
    }
    printf("\n");
    return 0;
}

int FlagE(const ull x) {
    if (x > 10) {
        printf("Error: x must be <= 10 for flag -e.\n");
        return -1;
    }
    printf("Table of powers (bases 1-10, exponents 1-%llu):\n", x);
    for (int base = 1; base <= 10; base++) {
        printf("Base %d: ", base);
        for (ull exp = 1; exp <= x; exp++) {
            ull val = 1;
            for (ull k = 0; k < exp; k++) {
                val *= base;
            }
            printf("%llu ", val);
        }
        printf("\n");
    }
    return 0;
}

int FlagA(const ull x) {
    ull sum = 0;
    for (ull i = 1; i <= x; i++) {
        sum += i;
    }
    printf("Sum of 1 to %llu: %llu\n", x, sum);
    return 0;
}

int FlagF(const ull x) {
    if (x > 20) {
        printf("Error: x is too large for factorial (max 20).\n");
        return -1;
    }
    ull fact = 1;
    for (ull i = 1; i <= x; i++) {
        fact *= i;
    }
    printf("Factorial of %llu: %llu\n", x, fact);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: usage: %s <x> <flag>\n", argv[0]);
        return 1;
    }

    ull x;
    if (argv[1][0] == '-') {
        printf("Error: x must be positive\n");
        return 1;
    }

    if (sscanf(argv[1], "%llu", &x) != 1 || x == 0) {
        printf("Error: x must be a positive integer\n");
        return 1;
    }

    if (argv[2][0] != '-' && argv[2][0] != '/') {
        printf("Error: flag must start with '-' or '/'.\n");
        return 1;
    }

    char flag = argv[2][1];

    switch (flag) {
        case 'h': FlagH(x); break;
        case 'p': FlagP(x); break;
        case 's': FlagS(x); break;
        case 'e': FlagE(x); break;
        case 'a': FlagA(x); break;
        case 'f': FlagF(x); break;
        default:
            printf("Error: unknown flag '%c'. Available: -h, -p, -s, -e, -a, -f.\n", flag);
            return 1;
    }

    return 0;
}