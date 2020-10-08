#pragma once
#include <math.h>
#include <stdio.h>
int read_from_file(int dim, double *matr, FILE *input_file);
void create_from_formula(int dim, double *matr, int k);
double f(int k, int n, int i, int j);
void show(int l, int n, int m, double *matr);