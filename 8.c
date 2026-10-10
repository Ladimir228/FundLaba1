#include <stdio.h>
#include <stdlib.h>

int digit_value(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Error: usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    FILE *in = fopen(argv[1], "r");
    if (in == NULL) {
        printf("Error: cannot open input file '%s'\n", argv[1]);
        return 1;
    }

    FILE *out = fopen(argv[2], "w");
    if (out == NULL) {
        printf("Error: cannot open output file '%s'\n", argv[2]);
        fclose(in);
        return 1;
    }

    char token[1024];
    while (fscanf(in, "%1023s", token) == 1) {
        int max_digit = 0;

        for (int i = 0; token[i] != '\0'; i++) {
            int d = digit_value(token[i]);
            if (d > max_digit) {
                max_digit = d;
            }
        }

        int base = max_digit + 1;
        if (base < 2) base = 2;

        char *p = token;
        while (*p == '0' && *(p + 1) != '\0') {
            p++;
        }

        unsigned long long value = 0;
        for (int i = 0; p[i] != '\0'; i++) {
            int d = digit_value(p[i]);
            value = value * base + d;
        }

        fprintf(out, "%s %d %llu\n", p, base, value);
    }

    fclose(in);
    fclose(out);
    return 0;
}