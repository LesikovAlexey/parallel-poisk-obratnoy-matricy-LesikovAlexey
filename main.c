#include "main.h"
#include "stdlib.h"
int main(int argc, char **argv)
{
    int n;
    int m;
    int k;
    FILE *input_file;
    double *input_matr;
    double *copied_matr;
    double *inversed_matr;
    unsigned long long time = 0;

    if (argc < 4) {
        printf("Недостаточно аргументов\n");
        return -1;
    }
    if (sscanf(argv[1], "%d", &n) != 1) {
        printf("Некорректные аргументы\n");
        return -1;
    }
    if (sscanf(argv[2], "%d", &m) != 1) {
        printf("Некорректные аргументы\n");
        return -1;
    }
    if (sscanf(argv[3], "%d", &k) != 1) {
        printf("Некорректные аргументы\n");
        return -1;
    }
    if (m > n || n <= 0 || m <= 0) {
        printf("Некорректные аргументы\n");
        return -1;
    }
    input_matr = (double *)malloc(n * n * sizeof(double));
    copied_matr = (double *)malloc(n * n * sizeof(double));
    inversed_matr = (double *)malloc(n * n * sizeof(double));
    if (input_matr == NULL || inversed_matr == NULL || copied_matr == NULL) {
        free(input_matr);
        free(inversed_matr);
        free(copied_matr);
        printf("Не удалось выделить память\n");
        return -2;
    }

    switch (k) {
    case 0:
        if (argc == 4) {
            free(input_matr);
            free(inversed_matr);
            free(copied_matr);
            printf("Не указан файл\n");
            return -1;
        }
        input_file = fopen(argv[4], "r");
        if (input_file == NULL) {

            free(input_matr);
            free(inversed_matr);
            free(copied_matr);
            printf("Не удалось считать файл\n");
            return -3;
        }
        if (read_from_file(n, input_matr, input_file) != 0) {
            fclose(input_file);
            free(input_matr);
            free(inversed_matr);
            free(copied_matr);
            printf("Не удалось считать матрицу из файла\n");
            return -3;
        }
        fclose(input_file);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
        create_from_formula(n, input_matr, k);
        break;
    default:
        free(input_matr);
        free(inversed_matr);
        free(copied_matr);
        printf("Некорректные аргументы\n");
        return -1;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                inversed_matr[j + i * n] = 1;
            } else {
                inversed_matr[j + i * n] = 0;
            }
        }
    }
    show(n, n, m, input_matr);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            copied_matr[j + i * n] = input_matr[j + i * n];
        }
    }

    time = currentTimeNano();
    if (invert(n, copied_matr, inversed_matr) != 0) {
        free(input_matr);
        free(inversed_matr);
        free(copied_matr);
        printf("Невырожденная матрица\n");
        return -4;
    }
    time = currentTimeNano() - time;
    printf("Inverse:\n");
    show(n, n, m, inversed_matr);
    printf("Residual:%10.3e\n", norm(n, input_matr, inversed_matr));
    printf("\n");
    printf("Time: %.3lf s\n", (double)time / 1e+9);

    free(input_matr);
    free(inversed_matr);
    free(copied_matr);

    return 0;
}
