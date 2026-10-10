#include <stdio.h>
#include <string.h>

#define MAX_LEX 1024

int is_space(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

char* read_lexeme(FILE *f) {
    static char buf[MAX_LEX];
    int c;
    while ((c = fgetc(f)) != EOF && is_space(c));
    if (c == EOF) return NULL;
    int i = 0;
    buf[i++] = (char)c;
    while ((c = fgetc(f)) != EOF && !is_space(c)) {
        if (i < MAX_LEX - 1) buf[i++] = (char)c;
    }
    buf[i] = '\0';
    return buf;
}

void to_lower_str(char *s) {
    for (int i = 0; s[i]; i++) {
        if (s[i] >= 'A' && s[i] <= 'Z') s[i] += 32;
    }
}

void write_codes(FILE *out, const char *s, int base) {
    int first = 1;
    for (int i = 0; s[i]; i++) {
        unsigned char c = (unsigned char)s[i];
        char tmp[32];
        int idx = 0;
        if (c == 0) tmp[idx++] = '0';
        while (c > 0) {
            int d = c % base;
            tmp[idx++] = (d < 10) ? ('0' + d) : ('A' + d - 10);
            c /= base;
        }
        if (!first) fputc(' ', out);
        for (int j = idx - 1; j >= 0; j--) fputc(tmp[j], out);
        first = 0;
    }
}

int FlagR(const char *f1_path, const char *f2_path, const char *out_path) {
    FILE *f1 = fopen(f1_path, "r");
    if (f1 == NULL) { printf("Error: cannot open %s\n", f1_path); return 1; }
    FILE *f2 = fopen(f2_path, "r");
    if (f2 == NULL) { printf("Error: cannot open %s\n", f2_path); fclose(f1); return 1; }
    FILE *out = fopen(out_path, "w");
    if (out == NULL) { printf("Error: cannot open %s\n", out_path); fclose(f1); fclose(f2); return 1; }

    int from_first = 1;
    int first_out = 1;
    while (1) {
        char *lex;
        if (from_first) {
            lex = read_lexeme(f1);
            if (lex == NULL) {
                while ((lex = read_lexeme(f2)) != NULL) {
                    if (!first_out) fputc(' ', out);
                    fputs(lex, out);
                    first_out = 0;
                }
                break;
            }
        } else {
            lex = read_lexeme(f2);
            if (lex == NULL) {
                while ((lex = read_lexeme(f1)) != NULL) {
                    if (!first_out) fputc(' ', out);
                    fputs(lex, out);
                    first_out = 0;
                }
                break;
            }
        }
        if (!first_out) fputc(' ', out);
        fputs(lex, out);
        first_out = 0;
        from_first = !from_first;
    }

    fclose(f1);
    fclose(f2);
    fclose(out);
    return 0;
}

int FlagA(const char *in_path, const char *out_path) {
    FILE *in = fopen(in_path, "r");
    if (in == NULL) { printf("Error: cannot open %s\n", in_path); return 1; }
    FILE *out = fopen(out_path, "w");
    if (out == NULL) { printf("Error: cannot open %s\n", out_path); fclose(in); return 1; }

    int idx = 0;
    int first_out = 1;
    char *lex;
    while ((lex = read_lexeme(in)) != NULL) {
        idx++;
        if (!first_out) fputc(' ', out);
        first_out = 0;

        if (idx % 10 == 0) {
            to_lower_str(lex);
            write_codes(out, lex, 4);
        } else if (idx % 2 == 0) {
            to_lower_str(lex);
            fputs(lex, out);
        } else if (idx % 5 == 0) {
            write_codes(out, lex, 8);
        } else {
            fputs(lex, out);
        }
    }

    fclose(in);
    fclose(out);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: too few arguments\n");
        return 1;
    }
    if (argv[1][0] != '-' && argv[1][0] != '/') {
        printf("Error: flag must start with '-' or '/'\n");
        return 1;
    }
    char flag = argv[1][1];

    if (flag == 'r') {
        if (argc != 5) {
            printf("Error: flag -r requires 4 parameters (file1, file2, output)\n");
            return 1;
        }
        return FlagR(argv[2], argv[3], argv[4]);
    } else if (flag == 'a') {
        if (argc != 4) {
            printf("Error: flag -a requires 3 parameters (input, output)\n");
            return 1;
        }
        return FlagA(argv[2], argv[3]);
    } else {
        printf("Error: unknown flag '%c'. Available: -r, -a\n", flag);
        return 1;
    }
}