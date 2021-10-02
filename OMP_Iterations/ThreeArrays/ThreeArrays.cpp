#include <iostream>
#include <time.h>
#include <omp.h>

const int ARR_SIZE = 12;
const int ARR_MAX_VAL = 100;

int main()
{
    srand(time(NULL));
    int a[ARR_SIZE];
    int b[ARR_SIZE];
    int c[ARR_SIZE];

    omp_set_num_threads(3);
#pragma omp parallel for schedule(static, 4)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        int rank = omp_get_thread_num();
        a[i] = rand() % ARR_MAX_VAL;
        b[i] = rand() % ARR_MAX_VAL;
        printf("(%d) %d\n", rank, i);
    }

    printf("\na and b filled\n\n");

    omp_set_num_threads(4);
#pragma omp parallel for schedule(dynamic, 2)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        int rank = omp_get_thread_num();
        c[i] = a[i] + b[i];
        printf("(%d) %d\n", rank, i);
    }
}