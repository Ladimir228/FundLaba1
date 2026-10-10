#include <stdio.h>
#include <string.h>

int is_latin(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int is_digit_char(char c) {
    return c >= '0' && c <= '9';
}

int FlagD(FILE *in, FILE *out) {
    int c;
    while ((c = fgetc(in)) != EOF) {
        if (!is_digit_char((char)c)) {
            fputc(c, out);
        }
    }
    return 0;
}

int FlagI(FILE *in, FILE *out) {
    int c;
    int count = 0;
    int has_data = 0;
    while ((c = fgetc(in)) != EOF) {
        has_data = 1;
        if (c == '\n') {
            fprintf(out, "%d\n", count);
            count = 0;
            has_data = 0;
        } else if (is_latin((char)c)) {
            count++;
        }
    }
    if (has_data) {
        fprintf(out, "%d\n", count);
    }
    return 0;
}

int FlagS(FILE *in, FILE *out) {
    int c;
    int count = 0;
    int has_data = 0;
    while ((c = fgetc(in)) != EOF) {
        has_data = 1;
        if (c == '\n') {
            fprintf(out, "%d\n", count);
            count = 0;
            has_data = 0;
        } else {
            char ch = (char)c;
            if (!is_latin(ch) && !is_digit_char(ch) && ch != ' ') {
                count++;
            }
        }
    }
    if (has_data) {
        fprintf(out, "%d\n", count);
    }
    return 0;
}

int FlagA(FILE *in, FILE *out) {
    int c;
    while ((c = fgetc(in)) != EOF) {
        unsigned char uc = (unsigned char)c;
        if (is_digit_char((char)uc)) {
            fputc(uc, out);
        } else {
            fprintf(out, "%02X", uc);
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: usage: %s <flag> <input_file> [output_file]\n", argv[0]);
        return 1;
    }

    if (argv[1][0] != '-' && argv[1][0] != '/') {
        printf("Error: flag must start with '-' or '/'.\n");
        return 1;
    }

    char flag_char;
    int has_n = 0;

    if (argv[1][1] == 'n') {
        has_n = 1;
        flag_char = argv[1][2];
    } else {
        flag_char = argv[1][1];
    }

    if (flag_char != 'd' && flag_char != 'i' &&
        flag_char != 's' && flag_char != 'a') {
        printf("Error: unknown flag '%c'. Available: -d, -i, -s, -a (optionally with 'n').\n", flag_char);
        return 1;
    }

    char output_path[512];

    if (has_n) {
        if (argc != 4) {
            printf("Error: flag with 'n' requires output file as third argument.\n");
            return 1;
        }
        strncpy(output_path, argv[3], sizeof(output_path) - 1);
        output_path[sizeof(output_path) - 1] = '\0';
    } else {
        if (argc != 3) {
            printf("Error: too many arguments.\n");
            return 1;
        }
        snprintf(output_path, sizeof(output_path), "out_%s", argv[2]);
    }

    FILE *in = fopen(argv[2], "r");
    if (in == NULL) {
        printf("Error: cannot open input file '%s'.\n", argv[2]);
        return 1;
    }

    FILE *out = fopen(output_path, "w");
    if (out == NULL) {
        printf("Error: cannot open output file '%s'.\n", output_path);
        fclose(in);
        return 1;
    }

    switch (flag_char) {
        case 'd': FlagD(in, out); break;
        case 'i': FlagI(in, out); break;
        case 's': FlagS(in, out); break;
        case 'a': FlagA(in, out); break;
    }

    fclose(in);
    fclose(out);

    printf("Done. Output written to '%s'.\n", output_path);
    return 0;
}