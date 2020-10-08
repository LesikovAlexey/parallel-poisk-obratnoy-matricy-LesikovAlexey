#include "input.h"
int read_from_file(int dim, double *matr, FILE *input_file)
{
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            if (fscanf(input_file, "%lf", &matr[j + dim * i]) != 1) {
                return -1;
            }
        }
    }
    return 0;
}
double f(int k, int n, int i, int j)
{
    switch (k) {
    case 1:
        return (n - fmax(i, j) + 1);
        break;
    case 2:
        return (fmax(i, j));
        break;
    case 3:
        return (fabs(i - j));
        break;
    case 4:
        return (1 / ((double)i + (double)j - 1));
        break;
    }
    return 0;
}

void create_from_formula(int dim, double *matr, int k)
{
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matr[j + dim * i] = f(k, dim, i + 1, j + 1);
        }
    }
}

void show(int l, int n, int m, double *matr)
{

    for (int i = 0; i < fmin(l, m); i++) {
        for (int j = 0; j < fmin(n, m); j++) {
            printf("%10.3e", matr[j + n * i]);
        }
        printf("\n");
    }
    printf("\n");
}