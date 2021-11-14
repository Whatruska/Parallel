#include "Header.h"
#include <mpi.h>
#include <iostream>
#include <chrono>

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

	auto start = std::chrono::system_clock::now();
	if (rank = 0) {
		int buf[N];
		for (int i = 1; i < N; i++) {
			MPI_Recv(&buf, N, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
		}
	}
	else {
		MPI_Send(&arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	MPI_Barrier(MPI_COMM_WORLD);
	auto end = std::chrono::system_clock::now();
	auto t = std::chrono::duration_cast<std::chrono::milliseconds>(start - end).count();
	printf("Send: %d", t);

	start = std::chrono::system_clock::now();
	if (rank = 0) {
		int buf[N];
		for (int i = 1; i < N; i++) {
			MPI_Recv(buf, N, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
		}
	}
	else {
		MPI_Ssend(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	end = std::chrono::system_clock::now();
	t = std::chrono::duration_cast<std::chrono::milliseconds>(start - end).count();
	printf("Ssend: %d", t);

	start = std::chrono::system_clock::now();
	if (rank = 0) {
		int buf[N];
		for (int i = 1; i < N; i++) {
			MPI_Recv(buf, N, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
		}
	}
	else {
		MPI_Bsend(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	end = std::chrono::system_clock::now();
	t = std::chrono::duration_cast<std::chrono::milliseconds>(start - end).count();
	printf("Bsend: %d", t);

	start = std::chrono::system_clock::now();
	if (rank = 0) {
		int buf[N];
		for (int i = 1; i < N; i++) {
			MPI_Recv(buf, N, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
		}
	}
	else {
		MPI_Rsend(arr, N, MPI_INT, 0, 0, MPI_COMM_WORLD);
	}
	end = std::chrono::system_clock::now();
	t = std::chrono::duration_cast<std::chrono::milliseconds>(start - end).count();
	printf("Rsend: %d", t);

	MPI_Finalize();
}