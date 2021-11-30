#include "Header.h"
#include <mpi.h>
#include <iostream>

void twelveth_task() {
	int rank = 0;
	int size = 0;

	int new_comm_rank = 0;
	int new_comm_size = 0;

	MPI_Comm new_comm;

	MPI_Status status;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int color = rank / 2;

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

	MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Comm_rank(new_comm, &new_comm_rank);
	MPI_Comm_size(new_comm, &new_comm_size);
	a = 100;
	if (rank < size / 2) {
		if (new_comm_rank != 0) {
			MPI_Recv(&a, 1, MPI_INT, new_comm_rank - 1, 0, new_comm, &status);
			printf("(%d) Received: %d\n", new_comm_rank, a);
		}
		if (new_comm_rank != new_comm_size - 1) {
			a += 10;
			MPI_Send(&a, 1, MPI_INT, new_comm_rank + 1, 0, new_comm);
			printf("(%d) Sent: %d\n", new_comm_rank, a);
		}
	}

	MPI_Finalize();
}