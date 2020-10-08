#include "inversion.h"
double norm(int dim, const double *matr1, const double *matr2)
{
    double norm = 0;
    double *matr3;
    matr3 = (double *)malloc(dim * dim * sizeof(double));
    if (matr3 == NULL) {
        free(matr3);
        return -1;
    }
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            matr3[j + i * dim] = 0;
            for (int k = 0; k < dim; k++) {
                matr3[j + i * dim] += matr1[k + i * dim] * matr2[j + k * dim];
            }
        }
    }
    for (int i = 0; i < dim; i++) {
        matr3[i + i * dim] -= 1;
    }
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            norm += fabs(matr3[j + dim * i]) * fabs(matr3[j + dim * i]);
        }
    }
    free(matr3);
    return sqrt(norm);
}
int invert(int dim, double *matr, double *inversed_matr)
{
    double c;
    double s;
    double buf1;
    double buf2;

    for (int i = 0; i < dim; i++) {
        for (int j = i + 1; j < dim; j++) {
            c = matr[i + i * dim] / sqrt(matr[i + i * dim] * matr[i + i * dim] +
                                         matr[i + j * dim] * matr[i + j * dim]);
            s = matr[i + j * dim] / sqrt(matr[i + i * dim] * matr[i + i * dim] +
                                         matr[i + j * dim] * matr[i + j * dim]);
            if ((isnormal(c) == 0 && c != 0) || (isnormal(s) == 0 && s != 0)) {
                return -1;
            }
            for (int k = 0; k < dim; k++) {
                buf1 = inversed_matr[k + dim * i];
                buf2 = inversed_matr[k + dim * j];
                inversed_matr[k + dim * i] = c * buf1 + s * buf2;
                inversed_matr[k + dim * j] = c * buf2 - s * buf1;
            }
            for (int k = 0; k < dim; k++) {
                buf1 = matr[k + dim * i];
                buf2 = matr[k + dim * j];
                matr[k + dim * i] = c * buf1 + s * buf2;
                matr[k + dim * j] = c * buf2 - s * buf1;
            }
            matr[i + dim * j] = 0;
        }
    }
    for (int i = dim - 1; i > -1; i--) {
        for (int j = i + 1; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                inversed_matr[k + i * dim] =
                    inversed_matr[k + i * dim] -
                    matr[j + i * dim] * inversed_matr[k + j * dim];
            }
        }
        if (matr[i + i * dim] == 0) {
            return -1;
        }
        for (int k = 0; k < dim; k++) {
            inversed_matr[k + i * dim] =
                inversed_matr[k + i * dim] / matr[i + i * dim];
        }
    }

    return 0;
}