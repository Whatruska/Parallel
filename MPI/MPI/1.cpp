#include "Header.h"
#include <iostream>
#include <mpi.h>

void first_task() {
    MPI_Status st;
    int sum, rank, size = 0;
    int proc_sum = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    printf("Hello world! (%d of %d)", rank, size);
    MPI_Finalize();
}