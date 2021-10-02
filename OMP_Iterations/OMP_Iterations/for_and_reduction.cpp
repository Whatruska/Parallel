#include <iostream>
#include <omp.h>
#include <time.h>

const int ARR_SIZE = 100;
const int ARR_MAX_VAL = 1000;

int sum = 0;
int s = 0;

int main()
{
    int arr[ARR_SIZE];
    srand(time(NULL));
    for (int i = 0; i < ARR_SIZE; i++)
    {
        arr[i] = rand() % ARR_MAX_VAL;
    }

#pragma omp parallel for
    for (int i = 0; i < ARR_SIZE; i++)
    {
        sum += arr[i];
    }
    printf("For average: %f\n", sum / (float) ARR_SIZE);

#pragma omp parallel for reduction (+ : s)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        s += arr[i];
    }

    printf("Reduction average: %f\n", s / (float)ARR_SIZE);
}