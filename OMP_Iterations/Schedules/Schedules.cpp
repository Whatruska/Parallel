#include <iostream>
#include <time.h>
#include <chrono>
#include <omp.h>

using namespace std::chrono;

const int ARR_SIZE = 16000;
const int ARR_MAX_VAL = 1000;
const int CHUNK = 1000;

void init_a(int a[], int i)
{
    a[i] = rand() % ARR_MAX_VAL;
}

void count_b(int a[], int b[], int i)
{
    if (i != 0 && i != ARR_SIZE - 1)
    {
        b[i] = a[i - 1] + a[i] + a[i + 1];
    }
    else
    {
        b[i] = 0;
    }
}

int main()
{
    srand(time(NULL));
    int a[ARR_SIZE];
    int b[ARR_SIZE];

    omp_set_num_threads(4);

    time_point<steady_clock> start_time = steady_clock::now();
#pragma omp parallel for schedule(static)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        init_a(a, i);
    }
#pragma omp parallel for schedule(static)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        count_b(a, b, i);
    }
    time_point<steady_clock> finish_time = steady_clock::now();
    auto diff = duration_cast<nanoseconds>(finish_time - start_time);
    printf("Pure static: %d\n", diff.count());

    start_time = steady_clock::now();
#pragma omp parallel for schedule(static, CHUNK)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        init_a(a, i);
    }
#pragma omp parallel for schedule(static, CHUNK)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        count_b(a, b, i);
    }
    finish_time = steady_clock::now();
    diff = duration_cast<nanoseconds>(finish_time - start_time);
    printf("Static with chunk (%d): %d\n", CHUNK, diff.count());

    start_time = steady_clock::now();
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        init_a(a, i);
    }
#pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        count_b(a, b, i);
    }
    finish_time = steady_clock::now();
    diff = duration_cast<nanoseconds>(finish_time - start_time);
    printf("Pure dynamic: %d\n", diff.count());

    start_time = steady_clock::now();
#pragma omp parallel for schedule(dynamic, CHUNK)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        init_a(a, i);
    }
#pragma omp parallel for schedule(dynamic, CHUNK)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        count_b(a, b, i);
    }
    finish_time = steady_clock::now();
    diff = duration_cast<nanoseconds>(finish_time - start_time);
    printf("Dynamic with chunk (%d): %d\n", CHUNK, diff.count());

    start_time = steady_clock::now();
#pragma omp parallel for schedule(guided)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        init_a(a, i);
    }
#pragma omp parallel for schedule(guided)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        count_b(a, b, i);
    }
    finish_time = steady_clock::now();
    diff = duration_cast<nanoseconds>(finish_time - start_time);
    printf("Guided: %d\n", diff.count());

    start_time = steady_clock::now();
#pragma omp parallel for schedule(runtime)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        init_a(a, i);
    }
#pragma omp parallel for schedule(runtime)
    for (int i = 0; i < ARR_SIZE; i++)
    {
        count_b(a, b, i);
    }
    finish_time = steady_clock::now();
    diff = duration_cast<nanoseconds>(finish_time - start_time);
    printf("Auto: %d\n", diff.count());
}