#pragma once
#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "main.h"
double norm(int dim, const double *matr1, const double *matr2);
// int invert(int dim, double *matr, double *inversed_matr, int P, void **args);

// void *invert_parallel (void *args);
void *invert (void *args);
struct thread_args {
    int P;
    int dim;
    double *matr;
    double *inversed_matr;
    int p;
    pthread_barrier_t *barrier;
    int err;
};