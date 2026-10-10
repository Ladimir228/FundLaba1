#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

typedef unsigned long long ull;

int is_convex(int count, ...) {
    if (count < 3) return 0;

    double *xs = malloc(count * sizeof(double));
    double *ys = malloc(count * sizeof(double));
    if (xs == NULL || ys == NULL) {
        free(xs);
        free(ys);
        return 0;
    }

    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; i++) {
        xs[i] = va_arg(args, double);
        ys[i] = va_arg(args, double);
    }
    va_end(args);

    int pos = 0, neg = 0;
    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;
        int k = (i + 2) % count;
        double dx1 = xs[j] - xs[i];
        double dy1 = ys[j] - ys[i];
        double dx2 = xs[k] - xs[j];
        double dy2 = ys[k] - ys[j];
        double cross = dx1 * dy2 - dy1 * dx2;
        if (cross > 1e-9) pos++;
        else if (cross < -1e-9) neg++;
    }
    free(xs);
    free(ys);
    return (pos == 0 || neg == 0) && (pos + neg > 0);
}

double poly_value(double x, int n, ...) {
    va_list args;
    va_start(args, n);
    double result = 0.0;
    for (int i = n; i >= 0; i--) {
        result = result * x + va_arg(args, double);
    }
    va_end(args);
    return result;
}

int is_kaprekar(ull n, int base) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    ull sq = n * n;
    ull pw = 1;
    while (pw <= sq / (ull)base) {
        pw *= (ull)base;
        ull right = sq % pw;
        ull left = sq / pw;
        if (right > 0 && left + right == n) return 1;
    }
    return 0;
}

int count_kaprekar(int base, int count, ...) {
    va_list args;
    va_start(args, count);
    int found = 0;
    for (int i = 0; i < count; i++) {
        const char *str = va_arg(args, const char *);
        ull n = strtoull(str, NULL, base);
        if (is_kaprekar(n, base)) {
            printf("  %s (base %d) is a Kaprekar number\n", str, base);
            found++;
        }
    }
    va_end(args);
    return found;
}

double geometric_mean(int count, ...) {
    if (count <= 0) return -1.0;
    va_list args;
    va_start(args, count);
    double log_sum = 0.0;
    for (int i = 0; i < count; i++) {
        double v = va_arg(args, double);
        if (v <= 0) {
            va_end(args);
            return -1.0;
        }
        log_sum += log(v);
    }
    va_end(args);
    return exp(log_sum / count);
}

double fast_pow(double base, int exp) {
    if (exp == 0) return 1.0;
    if (exp < 0) return 1.0 / fast_pow(base, -exp);
    if (exp % 2 == 0) {
        double half = fast_pow(base, exp / 2);
        return half * half;
    }
    return base * fast_pow(base, exp - 1);
}

typedef double (*func_t)(double);

double bisection(double a, double b, double eps, func_t f) {
    double fa = f(a);
    double fb = f(b);
    if (fa * fb > 0) return NAN;

    while ((b - a) / 2.0 > eps) {
        double mid = (a + b) / 2.0;
        double fm = f(mid);
        if (fm == 0.0) return mid;
        if (fa * fm < 0) {
            b = mid;
            fb = fm;
        } else {
            a = mid;
            fa = fm;
        }
    }
    return (a + b) / 2.0;
}

double f1(double x) { return x * x - 4.0; }
double f2(double x) { return cos(x) - x; }
double f3(double x) { return x * x * x - x - 2.0; }

int main(void) {
    printf("1. Convex polygon\n");
    printf("Square: %s\n",
           is_convex(4, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0) ? "convex" : "NOT convex");
    printf("Triangle: %s\n",
           is_convex(3, 0.0, 0.0, 2.0, 0.0, 0.0, 2.0) ? "convex" : "NOT convex");
    printf("Arrow:  %s\n",
           is_convex(5, 0.0, 0.0, 2.0, 0.0, 1.0, 1.0, 2.0, 2.0, 0.0, 2.0) ? "convex" : "NOT convex");

    printf("\n2. Polynomial\n");
    printf("2x^3 - 3x^2 + 4x - 5 at x = 2: %.4f\n",
           poly_value(2.0, 3, 2.0, -3.0, 4.0, -5.0));
    printf("x^2 + 1 at x = 3: %.4f\n",
           poly_value(3.0, 2, 1.0, 0.0, 1.0));

    printf("\n3. Kaprekar numbers\n");
    printf("Base 10:\n");
    count_kaprekar(10, 6, "9", "45", "55", "99", "297", "12");
    printf("Base 16:\n");
    count_kaprekar(16, 3, "FF", "AB", "10");

    printf("\n4. Geometric mean\n");
    printf("GM(1, 4, 16, 64) = %.4f\n", geometric_mean(4, 1.0, 4.0, 16.0, 64.0));
    printf("GM(2, 8)         = %.4f\n", geometric_mean(2, 2.0, 8.0));
    printf("GM(3, 27)        = %.4f\n", geometric_mean(2, 3.0, 27.0));

    printf("\n5. Fast exponentiation\n");
    printf("2^10  = %.4f\n", fast_pow(2.0, 10));
    printf("3^-3  = %.4f\n", fast_pow(3.0, -3));
    printf("1.5^7 = %.4f\n", fast_pow(1.5, 7));

    printf("\n6. Bisection\n");
    printf("x^2 - 4 = 0 on [0, 5]:     root = %.6f\n", bisection(0.0, 5.0, 1e-6, f1));
    printf("cos(x) - x = 0 on [0, 1]:  root = %.6f\n", bisection(0.0, 1.0, 1e-6, f2));
    printf("x^3 - x - 2 = 0 on [1, 2]: root = %.6f\n", bisection(1.0, 2.0, 1e-6, f3));

    return 0;
}