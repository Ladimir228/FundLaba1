#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef unsigned long long ull;
typedef long long ll;

int char_to_digit(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'Z') return c - 'A' + 10;
    if (c >= 'a' && c <= 'z') return c - 'a' + 10;
    return -1;
}

char digit_to_char(int d) {
    if (d < 10) return '0' + d;
    return 'A' + d - 10;
}

int parse_base(const char *s, int base, ll *out) {
    int i = 0, neg = 0;
    if (s[0] == '-') { neg = 1; i++; }
    else if (s[0] == '+') { i++; }
    if (s[i] == '\0') return 0;

    ull val = 0;
    for (; s[i]; i++) {
        int d = char_to_digit(s[i]);
        if (d < 0 || d >= base) return 0;
        val = val * base + d;
    }

    if (neg) {
        if (val > (ull)LLONG_MAX + 1) return 0;
        *out = (val == (ull)LLONG_MAX + 1) ? LLONG_MIN : -(ll)val;
    } else {
        if (val > (ull)LLONG_MAX) return 0;
        *out = (ll)val;
    }
    return 1;
}

void print_base(ll value, int base) {
    if (value == 0) { putchar('0'); return; }
    ull v;
    if (value < 0) {
        putchar('-');
        v = (ull)(-(value + 1)) + 1;
    } else {
        v = (ull)value;
    }
    char buf[128];
    int i = 0;
    while (v > 0) {
        int d = v % base;
        buf[i++] = digit_to_char(d);
        v /= base;
    }
    for (int j = i - 1; j >= 0; j--) putchar(buf[j]);
}

int main(void) {
    int base;
    printf("Enter base (2..36): ");
    if (scanf("%d", &base) != 1 || base < 2 || base > 36) {
        printf("Invalid base\n");
        return 1;
    }

    char buf[1024];
    ll max_val = 0;
    ull max_abs = 0;
    int first = 1;
    ll sum = 0;

    printf("Enter numbers in base %d (type Stop to finish):\n", base);
    while (scanf("%1023s", buf) == 1) {
        if (strcmp(buf, "Stop") == 0) break;
        ll val;
        if (!parse_base(buf, base, &val)) {
            printf("Invalid number: %s\n", buf);
            continue;
        }
        ull abs_val = (val < 0) ? (ull)(-(val + 1)) + 1 : (ull)val;
        if (first || abs_val > max_abs) {
            max_abs = abs_val;
            max_val = val;
            first = 0;
        }
        sum += val;
    }

    printf("\nMax by absolute value: ");
    print_base(max_val, 10);
    printf("\nIn bases 9, 18, 27, 36:\n");
    for (int b = 9; b <= 36; b += 9) {
        printf("base %2d: ", b);
        print_base(max_val, b);
        printf("\n");
    }

    printf("\nSum: ");
    print_base(sum, 10);
    printf("\nIn bases 9, 18, 27, 36:\n");
    for (int b = 9; b <= 36; b += 9) {
        printf("base %2d: ", b);
        print_base(sum, b);
        printf("\n");
    }

    return 0;
}