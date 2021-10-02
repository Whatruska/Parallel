#include <iostream>
#include <time.h>
#include <chrono>
#include <omp.h>

using namespace std::chrono;

const int VECTOR_LENGTH = 1500;
const int MATRIX_LENGTH = 100;
const int ARR_MAX_VAL = 10;

int main()
{
    srand(time(NULL));
    int matrix[MATRIX_LENGTH][VECTOR_LENGTH];
    int vector[VECTOR_LENGTH];
    int result[MATRIX_LENGTH];

    for (int i = 0; i < MATRIX_LENGTH; i++)
    {
        for (int j = 0; j < VECTOR_LENGTH; j++)
        {
            matrix[i][j] = rand() % ARR_MAX_VAL;
        }
    }

    for (int i = 0; i < VECTOR_LENGTH; i++)
    {
        vector[i] = rand() % ARR_MAX_VAL;
    }

    time_point<steady_clock> start_time = steady_clock::now();
    for (int i = 0; i < MATRIX_LENGTH; i++)
    {
        int sum = 0;
        for (int j = 0; j < VECTOR_LENGTH; j++)
        {
            sum += vector[j] * matrix[i][j];
        }
        result[i] = sum;
    }
    time_point<steady_clock> finish_time = steady_clock::now();
    auto diff = duration_cast<nanoseconds>(finish_time - start_time);
    printf("Sequence: %d\n", diff.count());

    start_time = steady_clock::now();
#pragma omp parallel for schedule(dynamic, 25)
    for (int i = 0; i < MATRIX_LENGTH; i++)
    {
        int sum = 0;
#pragma omp parallel for schedule(dynamic, 375)
        for (int j = 0; j < VECTOR_LENGTH; j++)
        {
            sum += vector[j] * matrix[i][j];
        }
        result[i] = sum;
    }
    finish_time = steady_clock::now();
    diff = duration_cast<nanoseconds>(finish_time - start_time);
    printf("Parallel: %d\n", diff.count());
}