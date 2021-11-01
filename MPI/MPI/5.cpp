#include "Header.h"
#include <iostream>
#include <mpi.h>

void fifth_task() {
    int rank, size = 0;
    int sum = 0;
    int proc_sum = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 100;
    const int MAX_VAL = 10;

    int a[N];
    int b[N];

    srand(time(NULL));

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            a[i] = rand() % MAX_VAL + 1;
            b[i] = rand() % MAX_VAL + 1;
        }
    }

    MPI_Bcast(a, N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, N, MPI_INT, 0, MPI_COMM_WORLD);

    int* len = new int[size];
    int* ind = new int[size];
    int rest = N;
    int k = N / size;
    ind[0] = 0;
    len[0] = k;

    for (int i = 1; i < size; i++) {
        rest -= k;
        k = rest / (size - i);
        len[i] = k;
        ind[i] = ind[i - 1] + len[i - 1];
    }

    int proc_size = len[rank];
    int* proc_a = new int[proc_size];
    int* proc_b = new int[proc_size];

    MPI_Scatterv(a, len, ind, MPI_INT, proc_a, proc_size, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(b, len, ind, MPI_INT, proc_b, proc_size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < proc_size; i++) {
        int a_value = proc_a[i];
        int b_value = proc_b[i];
        
        proc_sum += a_value * b_value;
    }

    MPI_Reduce(&proc_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        printf("Sum: %d", sum);
    }

    MPI_Finalize();
}