#include "Header.h"
#include <mpi.h>
#include <iostream>

const int N = 64;

const int MAX_VAL = 100;

void eighth_task() {
	MPI_Status status;
	srand(time(NULL));

	int rank = 0;
	int size = 0;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int* arr = new int[size];

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			arr[i] = rand() % MAX_VAL;
		}

		printf("Source: [");
		for (int i = 0; i < N; i++) {
			printf("%d", arr[i]);
			if (i != N - 1) {
				printf(", ");
			}
		}
		printf("]\n");

		for (int i = 1; i < size; i++) {
			int* local_arr = new int[N / size];
			for (int j = 0; j < N / size; j++) {
				int index = i * N / size + j;
				local_arr[j] = arr[index];
			}
			MPI_Send(local_arr, N / size, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}

	int* buff = new int[N / size];
	if (rank == 0) {
		for (int i = 0; i < N / size; i++) {
			buff[i] = arr[i];
		}
	}
	else {
		MPI_Recv(buff, N / size, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
	}

	printf("(%d): [", rank);
	for (int i = 0; i < N / size; i++) {
		printf("%d", buff[i]);
		if (i != N / size - 1) {
			printf(", ");
		}
	}
	printf("]\n");


	if (rank == 0) {
		int* collected_arr = new int[N];
		for (int i = 0; i < N / size; i++) {
			collected_arr[i] = buff[i];
		}
		for (int i = 1; i < size; i++) {
			int* recv_buf = new int[N / size];
			MPI_Recv(recv_buf, N / size, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
			for (int j = 0; j < N / size; j++) {
				int index = i * N / size + j;
				collected_arr[index] = recv_buf[j];
			}
		}

		printf("Collected: [");
		for (int i = 0; i < N; i++) {
			printf("%d", collected_arr[i]);
			if (i != N - 1) {
				printf(", ");
			}
		}
		printf("]\n");
	}
	else {
		MPI_Send(buff, N / size, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	MPI_Finalize();
}