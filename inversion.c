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

void *invert (void *args)                     
{
    double c;
    double s;
    double buf1;
    double buf2;
    struct thread_args *A = (struct thread_args*)args;

    for (int i = 0; i < A->dim; i++) {
        for (int j = i + 1; j < A->dim; j++) {
            c = A->matr[i + i * A->dim] / sqrt(A->matr[i + i * A->dim] * A->matr[i + i * A->dim] +
                                         A->matr[i + j * A->dim] * A->matr[i + j * A->dim]);
            s = A->matr[i + j * A->dim] / sqrt(A->matr[i + i * A->dim] * A->matr[i + i * A->dim] +
                                         A->matr[i + j * A->dim] * A->matr[i + j * A->dim]);
            pthread_barrier_wait(A->barrier);                             
            if ((isnormal(c) == 0 && c != 0) || (isnormal(s) == 0 && s != 0)) {
                A->err = -1;
                return NULL;
            }
            for (int k = 0; k < A->dim; k++) {
                if (k%A->P == A->p) {
                    buf1 = A->inversed_matr[k + A->dim * i];
                    buf2 = A->inversed_matr[k + A->dim * j];
                    A->inversed_matr[k + A->dim * i] = c * buf1 + s * buf2;
                    A->inversed_matr[k + A->dim * j] = c * buf2 - s * buf1;
                }
                pthread_barrier_wait(A->barrier);
            }
            for (int k = 0; k < A->dim; k++) {
                if (k%A->P == A->p) {
                    buf1 = A->matr[k + A->dim * i];
                    buf2 = A->matr[k + A->dim * j];
                    A->matr[k + A->dim * i] = c * buf1 + s * buf2;
                    A->matr[k + A->dim * j] = c * buf2 - s * buf1;
                }
                pthread_barrier_wait(A->barrier);
            }
        }
    }
    
    for (int i = A->dim - 1; i > -1; i--) {
        for (int j = i + 1; j < A->dim; j++) {
            for (int k = 0; k < A->dim; k++) {
                if (k%A->P == A->p) {
                    A->inversed_matr[k + i * A->dim] =
                        A->inversed_matr[k + i * A->dim] -
                        A->matr[j + i * A->dim] * A->inversed_matr[k + j * A->dim];
                }
                pthread_barrier_wait(A->barrier);
            }
        }
        if (A->matr[i + i * A->dim] == 0) {
            A->err = -1;
            return NULL;
        }
        for (int k = 0; k < A->dim; k++) {
            if (k%A->P == A->p) {
                A->inversed_matr[k + i * A->dim] =
                    A->inversed_matr[k + i * A->dim] / A->matr[i + i * A->dim];
            }
        }
    }

    return NULL;
}