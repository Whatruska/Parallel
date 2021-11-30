#include "Header.h"
#include <mpi.h>
#include <iostream>
#include <chrono>
#include <cmath>

using namespace std;

const int MAX_VAL = 1000;
const int N = 20;

void tenth_task() {
	MPI_Status status;

	srand(time(NULL));

	int rank = 0;
	int size = 0;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int arr[N];

	for (int i = 0; i < N; i++) {
		arr[i] = rand() % MAX_VAL;
	}

	auto start = MPI_Wtime();
	if (rank == 0) {
		int buf[N];
		MPI_Recv(&buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
	}
	else {
		MPI_Send(&arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}

	auto end = MPI_Wtime();
	auto t = end - start;
	if (rank == 0) {
		printf("Send: %f\n", abs(t));
	}

	start = MPI_Wtime();
	if (rank == 0) {
		int buf[N];
		MPI_Recv(&buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
	}
	else {
		MPI_Ssend(&arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	end = MPI_Wtime();
	t = end - start;
	if (rank == 0) {
		printf("Ssend: %f\n", abs(t));
	}

	start = MPI_Wtime();
	if (rank == 0) {
		int buf[N];
		MPI_Recv(&buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
	}
	else {
		MPI_Buffer_attach(malloc(N * 8), N * 8);
		MPI_Bsend(&arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	end = MPI_Wtime();
	if (rank == 0) {
		t = end - start;
		printf("Bsend: %f\n", abs(t));
	}

	start = MPI_Wtime();
	if (rank == 0) {
		int buf[N];
		MPI_Recv(&buf, N, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
	}
	else {
		MPI_Rsend(&arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	end = MPI_Wtime();
	t = end - start;
	if (rank == 0) {
		printf("Rsend: %f\n", abs(t));
	}

	MPI_Finalize();
}