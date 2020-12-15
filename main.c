#include "main.h"
#include "stdlib.h"
int main(int argc, char **argv)
{
    int n;
    int m;
    int k;
    int P;
    FILE *input_file;
    double *input_matr;
    double *copied_matr;
    double *inversed_matr;
    pthread_barrier_t barrier;
    pthread_t **threads;
    struct thread_args **args;
    
    
    unsigned long long time = 0;
    

    if (argc < 5) {
        printf("Недостаточно аргументов ./a.out n m k P\n");
        return -1;
    }
    if (sscanf(argv[1], "%d", &n) != 1) {
        printf("Некорректные аргументы ./a.out n m k P\n");
        return -1;
    }
    if (sscanf(argv[2], "%d", &m) != 1) {
        printf("Некорректные аргументы ./a.out n m k P\n");
        return -1;
    }
    if (sscanf(argv[3], "%d", &k) != 1) {
        printf("Некорректные аргументы ./a.out n m k P\n");
        return -1;
    }
    if (m > n || n <= 0 || m <= 0) {
        printf("Некорректные аргументы ./a.out n m k P\n");
        return -1;
    }
    if (sscanf(argv[4], "%d", &P) != 1) {
        printf("Некорректные аргументы ./a.out n m k P\n");
        return -1;
    }
    input_matr = (double *)malloc(n * n * sizeof(double));
    copied_matr = (double *)malloc(n * n * sizeof(double));
    inversed_matr = (double *)malloc(n * n * sizeof(double));
    
    args = (struct thread_args **)malloc(P * sizeof(struct thread_args *)); 
    threads = (pthread_t **)malloc(P * sizeof(pthread_t *)); 
    for (int p = 0; p < P; p++) {
        args[p] = (struct thread_args *)malloc(sizeof(struct thread_args));
        threads[p] = (pthread_t *)malloc(sizeof(pthread_t));
    }
    
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
            printf("Не указан файл ./a.out n m k P file_name\n");
            return -1;
        }
        input_file = fopen(argv[5], "r");
        if (input_file == NULL) {

            free(input_matr);
            free(inversed_matr);
            free(copied_matr);
            printf("Не удалось считать файл ./a.out n m k P file_name\n");
            return -3;
        }
        if (read_from_file(n, input_matr, input_file) != 0) {
            fclose(input_file);
            free(input_matr);
            free(inversed_matr);
            free(copied_matr);
            printf("Не удалось считать матрицу из файла ./a.out n m k P file_name\n");
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
        printf("Некорректные аргументы ./a.out n m k P\n");
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
    pthread_barrier_init(&barrier, NULL, P); 

    time = currentTimeNano();
    for (int p = 0; p < P; p++) {
        args[p]->dim = n;
        args[p]->matr = copied_matr;
        args[p]->inversed_matr = inversed_matr;
        args[p]->P = P;
        args[p]->p = p;
        args[p]->barrier = &barrier;
        args[p]->err = 0;
        pthread_create(threads[p], NULL, invert, args[p]);
    }
    for (int p = 0; p < P; p++) {
        pthread_join(*threads[p], NULL);
    }
    for (int p = 0; p < P; p++) {
        if (args[p]->err != 0) {
            free(input_matr);
            free(inversed_matr);
            free(copied_matr);
            for (int p = 0; p < P; p++) {
                free(args[p]);
                free(threads[p]);
            }
            free(args);
            free(threads);
            pthread_barrier_destroy(&barrier);
            printf("Невырожденная матрица\n");
            return -4;
        }
    }
    time = currentTimeNano() - time;
    printf("Inverse:\n");
    show(n, n, m, inversed_matr);
    printf("Time: %.3lf s\n", (double)time / 1e+9);
    
    printf("Residual:%10.3e\n", norm(n, input_matr, inversed_matr));
    printf("\n");
    

    free(input_matr);
    free(inversed_matr);
    free(copied_matr);
    for (int p = 0; p < P; p++) {
        free(args[p]);
        free(threads[p]);
    }
    free(args);
    free(threads);
    pthread_barrier_destroy(&barrier);
    return 0;
}
