#include <stdio.h>
#include <math.h>

int solve_quadratic(const double a, const double b, const double c, const double eps, double *x1, double *x2) {
    if (fabs(a) < eps) {
        if (fabs(b) < eps) {
            return 0;
        }
        *x1 = -c / b;
        return 1;
    }

    double d = b * b - 4 * a * c;

    if (d < -eps) {
        return 0;
    }
    if (fabs(d) <= eps) {
        *x1 = -b / (2 * a);
        return 1;
    }

    d = sqrt(d);
    *x1 = (-b + d) / (2 * a);
    *x2 = (-b - d) / (2 * a);
    return 2;
}

int FlagQ(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Error: flag -q requires 4 parameters (epsilon and 3 coefficients).\n");
        return 1;
    }

    double eps, a, b, c;
    if (sscanf(argv[2], "%lf", &eps) != 1 || eps <= 0 ||
        sscanf(argv[3], "%lf", &a) != 1 ||
        sscanf(argv[4], "%lf", &b) != 1 ||
        sscanf(argv[5], "%lf", &c) != 1) {
        printf("Error: invalid number format.\n");
        return 1;
    }

    double perms[6][3] = {
        {a, b, c}, {a, c, b}, {b, a, c},
        {b, c, a}, {c, a, b}, {c, b, a}
    };

    printf("Solving quadratic equations\n");
    for (int i = 0; i < 6; i++) {
        int is_duplicate = 0;
        for (int j = 0; j < i; j++) {
            if (fabs(perms[i][0] - perms[j][0]) < eps &&
                fabs(perms[i][1] - perms[j][1]) < eps &&
                fabs(perms[i][2] - perms[j][2]) < eps) {
                is_duplicate = 1;
                break;
            }
        }
        if (is_duplicate) continue;

        double x1, x2;
        int roots = solve_quadratic(perms[i][0], perms[i][1], perms[i][2], eps, &x1, &x2);

        printf("Coefficients: a=%g, b=%g, c=%g\n", perms[i][0], perms[i][1], perms[i][2]);
        if (roots == 0) {
            printf("  No real roots.\n");
        } else if (roots == 1) {
            printf("  One root: x = %g\n", x1);
        } else if (roots == 2) {
            printf("  Two roots: x1 = %g, x2 = %g\n", x1, x2);
        }
    }
    return 0;
}

int FlagM(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Error: flag -m requires 2 parameters (two integers).\n");
        return 1;
    }

    int num1, num2;
    if (sscanf(argv[2], "%d", &num1) != 1 || sscanf(argv[3], "%d", &num2) != 1) {
        printf("Error: invalid integer format.\n");
        return 1;
    }
    if (num1 == 0 || num2 == 0) {
        printf("Error: numbers must be non-zero.\n");
        return 1;
    }

    if (num1 % num2 == 0) {
        printf("%d is a multiple of %d.\n", num1, num2);
    } else {
        printf("%d is not a multiple of %d.\n", num1, num2);
    }
    return 0;
}

int FlagT(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Error: flag -t requires 4 parameters (epsilon and 3 sides).\n");
        return 1;
    }

    double eps, a, b, c;
    if (sscanf(argv[2], "%lf", &eps) != 1 || eps <= 0 ||
        sscanf(argv[3], "%lf", &a) != 1 ||
        sscanf(argv[4], "%lf", &b) != 1 ||
        sscanf(argv[5], "%lf", &c) != 1) {
        printf("Error: invalid number format.\n");
        return 1;
    }

    if (a <= 0 || b <= 0 || c <= 0) {
        printf("Error: sides must be positive numbers.\n");
        return 1;
    }

    double sides[3] = {a, b, c};

    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (sides[i] > sides[j]) {
                double temp = sides[i];
                sides[i] = sides[j];
                sides[j] = temp;
            }
        }
    }

    double sum_sq = sides[0] * sides[0] + sides[1] * sides[1];
    double hyp_sq = sides[2] * sides[2];

    if (fabs(sum_sq - hyp_sq) < eps) {
        printf("The numbers %g, %g, %g can be sides of a right-angled triangle.\n", a, b, c);
    } else {
        printf("The numbers %g, %g, %g cannot be sides of a right-angled triangle.\n", a, b, c);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Error: too few arguments.\n");
        return 1;
    }

    if (argv[1][0] != '-' && argv[1][0] != '/') {
        printf("Error: flag must start with '-' or '/'.\n");
        return 1;
    }

    char flag = argv[1][1];

    switch (flag) {
        case 'q': FlagQ(argc, argv); break;
        case 'm': FlagM(argc, argv); break;
        case 't': FlagT(argc, argv); break;
        default:
            printf("Error: unknown flag '%c'. Available: -q, -m, -t.\n", flag);
            return 1;
    }

    return 0;
}