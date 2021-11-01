#include "Header.h"
#include <iostream>
#include <mpi.h>

void third_task() {
    MPI_Status st;
    int rank, size = 0;
    int sum, proc_sum = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 100000;
    const int MAX = 2;
    float x[N];
    float y[N];

    srand(time(NULL));

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            x[i] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / (MAX))) - MAX / 2;
            y[i] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / (MAX))) - MAX / 2;
        }
    }

    MPI_Bcast(x, N, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Bcast(y, N, MPI_FLOAT, 0, MPI_COMM_WORLD);

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
    float* proc_x = new float[proc_size];
    float* proc_y = new float[proc_size];

    MPI_Scatterv(x, len, ind, MPI_FLOAT, proc_x, proc_size, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(y, len, ind, MPI_FLOAT, proc_y, proc_size, MPI_FLOAT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < proc_size; i++) {
        float px = proc_x[i];
        float py = proc_y[i];
        
        float val = px * px + py * py;

        if (val <= static_cast <float> (MAX / 2)) {
            proc_sum++;
        }
    }

    MPI_Reduce(&proc_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        float pi = static_cast <float> (static_cast <float> (sum) / N) * 4;
        printf("Counted pi: %f\n", pi);
    }
    MPI_Finalize();
}