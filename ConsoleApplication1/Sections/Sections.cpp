#include <iostream>
#include <omp.h>
#include <time.h>

using namespace std;

const int ARR_MAX_VAL = 100;
const int N = 6;
const int M = 8;

void print_matrix(int matrix[N][M])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            int val = matrix[i][j];
            printf("%d", val);
            if (j != M - 1)
            {
                printf(", ");
            }
            if (val < 10)
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main()
{
    int matrix[N][M];

    srand(time(NULL));

    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < M; j++)
        {
            matrix[i][j] = rand() % ARR_MAX_VAL;
        }
    }
    print_matrix(matrix);

#pragma omp parallel sections
    {
#pragma omp section
        {
            int sum = 0;
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < M; j++)
                {
                    sum += matrix[i][j];
                }
            }

            printf("(%d) Average: %lf\n", omp_get_thread_num(), sum / 48.0);
        }
#pragma omp section
        {
            int max = INT_MIN;
            int min = INT_MAX;

            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < M; j++)
                {
                    int val = matrix[i][j];
                    if (val < min) 
                    {
                        min = val;
                    }
                    else if (val > max)
                    {
                        max = val;
                    }
                }
            }

            printf("(%d) Min: %d, Max: %d\n", omp_get_thread_num(), min, max);
        }
#pragma omp section
        {
            int num = 0;
            for (int i = 0; i < N; i++)
            {
                for (int j = 0; j < M; j++)
                {
                    int val = matrix[i][j];
                    if (val % 3 == 0)
                    {
                        num++;
                    }
                }
            }

            printf("(%d) Num of elements, divided by 3: %d\n", omp_get_thread_num(), num);
        }
    }
}
