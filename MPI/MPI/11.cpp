#include "Header.h"
#include <mpi.h>
#include <iostream>

void eleventh_task() {
	int rank = 0;
	int size = 0;

	MPI_Status status;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int a = 0;
	if (rank != 0) {
		MPI_Recv(&a, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
		printf("(%d) Received: %d\n", rank, a);
	}
	if (rank != size - 1) {
		a += 10;
		MPI_Send(&a, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
		printf("(%d) Sent: %d\n", rank, a);
	}

	MPI_Finalize();
}