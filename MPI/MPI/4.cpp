#include "Header.h"
#include <iostream>
#include <mpi.h>

void fourth_task() {
    int rank, size = 0;
    int sum, proc_sum = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int* counts = new int[size];

    int* recv_counts = new int[size];
    int* displ = new int[size];
    for (int i = 0; i < size; i++)
    {
        recv_counts[i] = 1;
        displ[i] = i;
    }

    const int N = 10000;
    const int MAX_VAL = 100;
    int a[N];

    srand(time(NULL));
    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            a[i] = (rand() % MAX_VAL) - MAX_VAL / 2;
        }
    }

    MPI_Bcast(a, N, MPI_INT, 0, MPI_COMM_WORLD);

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
    int proc_count = 0;

    MPI_Scatterv(a, len, ind, MPI_INT, proc_a, proc_size, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < proc_size; i++) {
        int value = proc_a[i];
        if (value > 0) {
            proc_sum += value;
            proc_count++;
        }
    }

    MPI_Reduce(&proc_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        int total_count = 0;
        MPI_Gatherv(&proc_count, 1, MPI_INT, counts, recv_counts, displ, MPI_INT, 0, MPI_COMM_WORLD);
        for (int i = 0; i < size; i++)
        {
            total_count += counts[i];
        }

        float average = static_cast <float> (static_cast <float> (sum) / total_count);
        printf("Average: %f", average);
    } else {
        MPI_Gatherv(&proc_count, 1, MPI_INT, NULL, NULL, NULL, MPI_INT, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}