#include "Header.h"
#include <mpi.h>
#include <iostream>

const int N = 8;
const int M = 4;
const int MAX_VAL = 100;

void seventh_task() {
	int matrix[N][M];
	int vector[M];

	srand(time(NULL));

	int rank = 0;
	int size = 0;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				matrix[i][j] = rand() % MAX_VAL;
			}
		}

		for (int i = 0; i < M; i++) {
			vector[i] = rand() % MAX_VAL;
		}
	}

	int result[N];

	MPI_Bcast(vector, M, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(matrix, N * M, MPI_INT, 0, MPI_COMM_WORLD);

	int* sendcounts = new int[size];
	int* displs = new int[size];

	int* res_count = new int[size];
	int* res_disp = new int[size];

	int rem = N % size;
	int sum = 0;
	for (int i = 0; i < size; i++) {
		sendcounts[i] = (N / size) * M;
		if (rem > 0) {
			sendcounts[i] += M;
			rem--;
		}

		res_count[i] = sendcounts[i] / M;
		if (i == 0) {
			res_disp[i] = 0;
		}
		else {
			res_disp[i] = res_disp[i - 1] + res_count[i - 1];
		}

		displs[i] = sum;
		sum += sendcounts[i];
	}

	int rec_buf[100];
	MPI_Scatterv(&matrix, sendcounts, displs, MPI_INT, &rec_buf, 100, MPI_INT, 0, MPI_COMM_WORLD);

	int elements_count = sendcounts[rank];
	int row_count = elements_count / M;
	int* proc_result = new int[row_count];
	for (int i = 0; i < row_count; i++) {
		int s = 0;
		for (int j = 0; j < M; j++) {
			int index = i * M + j;
			s += rec_buf[index] * vector[j];
		}
		proc_result[i] = s;
	}

	MPI_Gatherv(proc_result, row_count, MPI_INT, result, res_count, res_disp, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			printf("%d ", result[i]);
		}
	}

	MPI_Finalize();
}