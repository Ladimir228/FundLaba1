#include <stdio.h>
#include <math.h>

double sum_a(double x, double eps) {
    double sum = 1.0, term = 1.0;
    int n = 1;
    while (fabs(term) >= eps) {
        term *= x / n;
        sum += term;
        n++;
    }
    return sum;
}

double sum_b(double x, double eps) {
    double sum = 1.0, term = 1.0;
    double x2 = x * x;
    int n = 1;
    while (fabs(term) >= eps) {
        term *= -x2 / ((2.0 * n - 1.0) * (2.0 * n));
        sum += term;
        n++;
    }
    return sum;
}

double sum_c(double x, double eps) {
    double sum = 1.0, term = 1.0;
    double x2 = x * x;
    int n = 0;
    while (fabs(term) >= eps && n < 1000) {
        n++;
        term *= 27.0 * n * n * n * x2 / ((3.0 * n - 2.0) * (3.0 * n - 1.0) * (3.0 * n));
        sum += term;
    }
    return sum;
}

double sum_d(double x, double eps) {
    double x2 = x * x;
    double term = -x2 / 2.0;
    double sum = term;
    int n = 1;
    while (fabs(term) >= eps && n < 1000000) {
        n++;
        term *= -(2.0 * n - 1.0) * x2 / (2.0 * n);
        sum += term;
    }
    return sum;
}

double f_a(double x) {
    if (fabs(x) < 1e-15) return 1.0;
    return log(1.0 + x) / x;
}

double f_b(double x) {
    return exp(-x * x / 2.0);
}

double f_c(double t) {
    return t * exp(-t);
}

double f_d(double x) {
    if (x < 1e-15) return 1.0;
    return pow(x, x);
}

double simpson(double (*f)(double), double a, double b, double eps) {
    int n = 2;
    double prev = 0.0;
    int iter = 0;
    while (iter < 25) {
        double h = (b - a) / n;
        double sum = f(a) + f(b);
        for (int i = 1; i < n; i++) {
            double x = a + i * h;
            sum += (i % 2 == 0) ? 2.0 * f(x) : 4.0 * f(x);
        }
        double curr = sum * h / 3.0;
        if (iter > 0 && fabs(curr - prev) < eps) return curr;
        prev = curr;
        n *= 2;
        iter++;
    }
    return prev;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: usage: %s <eps> [x]\n", argv[0]);
        return 1;
    }

    double eps;
    if (sscanf(argv[1], "%lf", &eps) != 1 || eps <= 0) {
        printf("Error: epsilon must be positive.\n");
        return 1;
    }

    double x = 0.5;
    if (argc >= 3) {
        if (sscanf(argv[2], "%lf", &x) != 1 || fabs(x) >= 1.0) {
            printf("Error: x must satisfy |x| < 1.\n");
            return 1;
        }
    }

    printf("Sums (x = %g, eps = %g)\n", x, eps);
    printf("a) sum x^n/n!                        = %.10f\n", sum_a(x, eps));
    printf("b) sum (-1)^n x^(2n)/(2n)!           = %.10f\n", sum_b(x, eps));
    printf("c) sum 3^(3n)(n!)^3 x^(2n)/(3n)!     = %.10f\n", sum_c(x, eps));
    printf("d) sum (-1)^n (2n-1)!! x^(2n)/(2n)!! = %.10f\n", sum_d(x, eps));

    printf("\nIntegrals (eps = %g)\n", eps);
    printf("a) integral_0^1 ln(1+x)/x dx         = %.10f\n", simpson(f_a, 0.0, 1.0, eps));
    printf("b) integral_0^1 e^(-x^2/2) dx        = %.10f\n", simpson(f_b, 0.0, 1.0, eps));
    printf("c) integral_0^1 ln(1/(1-x)) dx       = %.10f\n", simpson(f_c, 0.0, 50.0, eps));
    printf("d) integral_0^1 x^x dx               = %.10f\n", simpson(f_d, 0.0, 1.0, eps));

    return 0;
}