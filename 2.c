#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define M_PI 3.14159265358979323846

double e_limit(const double eps) {
    double prev = 0.0, curr = 2.0;
    unsigned long long n = 1;
    while (fabs(curr - prev) > eps) {
        prev = curr;
        n *= 2;
        curr = pow(1.0 + 1.0 / n, (double)n);
    }
    return curr;
}

double e_series(const double eps) {
    double sum = 1.0, term = 1.0;
    int n = 1;
    while (term > eps) {
        term /= n;
        sum += term;
        n++;
    }
    return sum;
}

double e_equation(const double eps) {
    double x = 2.0, prev = 0.0;
    while (fabs(x - prev) > eps) {
        prev = x;
        x = x * (2.0 - log(x));
    }
    return x;
}

double pi_limit(const double eps) {
    double prod = 1.0, prev = 0.0;
    int n = 1;
    while (fabs(prod - prev) > eps) {
        prev = prod;
        prod *= (2.0 * n) / (2.0 * n - 1.0) * (2.0 * n) / (2.0 * n + 1.0);
        n++;
    }
    return 2.0 * prod;
}

double pi_series(const double eps) {
    double sum = 0.0;
    int n = 1, sign = 1;
    while (1.0 / (2.0 * n - 1.0) > eps) {
        sum += sign / (2.0 * n - 1.0);
        sign = -sign;
        n++;
    }
    return 4.0 * sum;
}

double pi_equation(const double eps) {
    double x = 3.0, prev = 0.0;
    while (fabs(x - prev) > eps) {
        prev = x;
        x = x + (cos(x) + 1.0) / sin(x);
    }
    return x;
}

double ln2_limit(const double eps) {
    double prev = 0.0, curr = 1.0;
    unsigned long long n = 1;
    while (fabs(curr - prev) > eps) {
        prev = curr;
        n *= 2;
        curr = n * (pow(2.0, 1.0 / n) - 1.0);
    }
    return curr;
}

double ln2_series(const double eps) {
    double sum = 0.0;
    int n = 1, sign = 1;
    while (1.0 / n > eps) {
        sum += sign / (double)n;
        sign = -sign;
        n++;
    }
    return sum;
}

double ln2_equation(const double eps) {
    double x = 0.5, prev = 0.0;
    while (fabs(x - prev) > eps) {
        prev = x;
        x = x - 1.0 + 2.0 * exp(-x);
    }
    return x;
}

double sqrt2_limit(const double eps) {
    double x = -0.5, prev = 0.0;
    while (fabs(x - prev) > eps) {
        prev = x;
        x = x - x * x / 2.0 + 1.0;
    }
    return x;
}

double sqrt2_series(const double eps) {
    double prod = 1.0, prev = 0.0;
    int k = 2;
    while (fabs(prod - prev) > eps) {
        prev = prod;
        prod *= pow(2.0, 1.0 / (double)(1 << k));
        k++;
    }
    return prod;
}

double sqrt2_equation(const double eps) {
    double x = 1.0, prev = 0.0;
    while (fabs(x - prev) > eps) {
        prev = x;
        x = (x + 2.0 / x) / 2.0;
    }
    return x;
}

double gamma_limit(const double eps) {
    double sum = 1.0;
    double prev = 0.0;
    unsigned long long n = 2;
    double curr = sum - log((double)n);
    while (fabs(curr - prev) > eps) {
        prev = curr;
        sum += 1.0 / n;
        n++;
        curr = sum - log((double)n);
    }
    return curr;
}

double gamma_series(const double eps) {
    double sum = -M_PI * M_PI / 6.0;
    for (int k = 2; k <= 10000000; k++) {
        int r = (int)sqrt((double)k);
        sum += 1.0 / (r * r) - 1.0 / k;
    }
    return sum;
}

double gamma_equation(const double eps) {
    int t = 1000000;
    char *sieve = (char *)calloc(t + 1, sizeof(char));
    if (sieve == NULL) return -1.0;

    for (int i = 2; i * i <= t; i++) {
        if (sieve[i] == 0) {
            for (int j = i * i; j <= t; j += i) {
                sieve[j] = 1;
            }
        }
    }

    double prod = 1.0;
    for (int p = 2; p <= t; p++) {
        if (sieve[p] == 0) {
            prod *= (1.0 - 1.0 / (double)p);
        }
    }

    free(sieve);

    double limit_value = log((double)t) * prod;
    return -log(limit_value);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Error: usage: %s <epsilon>\n", argv[0]);
        return 1;
    }

    double eps;
    if (sscanf(argv[1], "%lf", &eps) != 1 || eps <= 0) {
        printf("Error: epsilon must be a positive number.\n");
        return 1;
    }

    printf("Calculating constants with precision %.6f\n\n", eps);

    printf("E\n");
    printf("Limit:    %.10f\n", e_limit(eps));
    printf("Series:   %.10f\n", e_series(eps));
    printf("Equation: %.10f\n\n", e_equation(eps));

    printf("PI\n");
    printf("Limit:    %.10f\n", pi_limit(eps));
    printf("Series:   %.10f\n", pi_series(eps));
    printf("Equation: %.10f\n\n", pi_equation(eps));

    printf("LN 2\n");
    printf("Limit:    %.10f\n", ln2_limit(eps));
    printf("Series:   %.10f\n", ln2_series(eps));
    printf("Equation: %.10f\n\n", ln2_equation(eps));

    printf("SQRT 2\n");
    printf("Limit:    %.10f\n", sqrt2_limit(eps));
    printf("Series:   %.10f\n", sqrt2_series(eps));
    printf("Equation: %.10f\n\n", sqrt2_equation(eps));

    printf("GAMMA\n");
    printf("Limit:    %.10f\n", gamma_limit(eps));
    printf("Series:   %.10f\n", gamma_series(eps));
    printf("Equation: %.10f\n", gamma_equation(eps));

    return 0;
}