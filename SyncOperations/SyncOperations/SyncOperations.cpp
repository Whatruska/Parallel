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
    srand(time(NULL));
    int matrix[N][M];
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < M; j++)
        {
            matrix[i][j] = rand() % ARR_MAX_VAL;
        }
    }

    int min = INT_MAX;
    int max = INT_MIN;

#pragma omp parallel for 
    {
        for (int i = 0; i < N; i++)
        {
#pragma omp parallel for
            {
                for (int j = 0; j < M; j++)
                {
                    int val = matrix[i][j];
                    if (val > max)
                    {
#pragma omp critical
                        if (val > max)
                        {
                            max = val;
                        }
                    }
                    else if (val < min)
                    {
#pragma omp critical
                        if (val < min)
                        {
                            min = val;
                        }
                    }
                }
            }
        }
    }
    print_matrix(matrix);
    printf("min = %d, max = %d", min, max);
}