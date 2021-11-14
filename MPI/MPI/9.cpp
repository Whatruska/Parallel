#include "Header.h"
#include <mpi.h>
#include <iostream>

const int MAX_VAL = 1000;
const int N = 20;

void ninth_task() {
	MPI_Status status;

	srand(time(NULL));

	int arr[N];

	for (int i = 0; i < N; i++) {
		arr[i] = rand() % MAX_VAL;
	}

	int rank = 0;
	int size = 0;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Bcast(arr, N, MPI_INT, 0, MPI_COMM_WORLD);

	int block_size = N / size;
	int* rev_arr = new int[block_size];
	for (int i = 0; i < block_size; i++) {
		int index = block_size * rank + i;
		rev_arr[i] = arr[N - 1 - index];
	}

	printf("(%d): [", rank);
	for (int i = 0; i < block_size; i++) {
		printf("%d", rev_arr[i]);
		if (i != block_size - 1) {
			printf(", ");
		}
	}
	printf("]\n");

	if (rank == 0) {
		printf("Source: [");
		for (int i = 0; i < N; i++) {
			printf("%d", arr[i]);
			if (i != N - 1) {
				printf(", ");
			}
		}
		printf("]\n");

		int* col_arr = new int[N];
		for (int i = 0; i < block_size; i++) {
			col_arr[i] = rev_arr[i];
		}
		for (int i = 1; i < size; i++) {
			int* recv_arr = new int[block_size];
			MPI_Recv(recv_arr, block_size, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			for (int j = 0; j < block_size; j++) {
				int index = i * block_size + j;
				col_arr[index] = recv_arr[j];
			}
		}
		printf("Reversed: [");
		for (int i = 0; i < N; i++) {
			printf("%d", col_arr[i]);
			if (i != N - 1) {
				printf(", ");
			}
		}
		printf("]\n");
	}
	else {
		MPI_Send(rev_arr, block_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
}