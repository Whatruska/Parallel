#include "Header.h"
#include <mpi.h>
#include <iostream>

const int N = 8;
const int MAX = 10;

void print_matrix(float matrix[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%.2f", matrix[i][j]);
			if (j < N - 1) {
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
}

void print_int_matrix(int matrix[N][N]) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%d", matrix[i][j]);
			if (j < N - 1) {
				printf(" ");
			}
		}
		printf("\n");
	}
	printf("\n");
}

void fifteenth_task() {
	float A[N][N];
	float B[N][N];

	MPI_Status st;
	int rank, size = 0;

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int displ[N][N];

	if (rank == 0) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				A[i][j] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / (MAX)));
				B[i][j] = static_cast <float> (rand()) / (static_cast <float>(RAND_MAX / (MAX)));
				displ[i][j] = -1;
			}
		}

		for (int i = 0; i < size; i++) {
			int freeIndex = 0;
			int count = i;
			while (count < N)
			{
				displ[i][freeIndex++] = count;
				count += size;
			}
		}
	}

	if (rank == 0) {
		for (int i = 1; i < size; i++) {
			MPI_Send(&displ, N*N, MPI_INT, i, 0, MPI_COMM_WORLD);
		}
	}
	else {
		MPI_Recv(&displ, N*N, MPI_INT, 0, 0, MPI_COMM_WORLD, &st);
	}

	float A_proc_matrix[N][N];
	float B_proc_matrix[N][N];

	int rows_num = 0;

	if (rank == 0) {
		for (int proc = 0; proc < size; proc++) {
			float buff_a[N][N];
			float buff_b[N][N];
			int buff_num = 0;
			for (int row = 0; row <= N / size; row++) {
				int row_index = displ[proc][row];
				if (row_index == -1) {
					if (proc == 0) {
						rows_num = row;
					}
					else {
						buff_num = row;
					}
					break;
				}
				else {
					for (int i = 0; i < N; i++) {
						if (proc == 0) {
							A_proc_matrix[row][i] = A[row_index][i];
							B_proc_matrix[row][i] = B[row_index][i];
						}
						else {
							buff_a[row][i] = A[row_index][i];
							buff_b[row][i] = B[row_index][i];
						}
					}
				}
			}
			if (proc > 0) {
				MPI_Send(&buff_a, N * N, MPI_FLOAT, proc, 0, MPI_COMM_WORLD);
				MPI_Send(&buff_b, N * N, MPI_FLOAT, proc, 1, MPI_COMM_WORLD);
				MPI_Send(&buff_num, 1, MPI_INT, proc, 2, MPI_COMM_WORLD);
			}
		}
	}
	else {
		MPI_Recv(&A_proc_matrix, N * N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &st);
		MPI_Recv(&B_proc_matrix, N * N, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &st);
		MPI_Recv(&rows_num, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, &st);
	}

	int* displ_data = displ[rank];
	for (int i = 0; i < rows_num; i++) {
		int col = displ_data[i];
		float coef = A_proc_matrix[i][col];
		for (int j = 0; j < N; j++) {
			A_proc_matrix[i][j] = A_proc_matrix[i][j] / coef;
		}
		coef = B_proc_matrix[i][col];
		for (int j = 0; j < N; j++) {
			B_proc_matrix[i][j] = B_proc_matrix[i][j] / coef;
		}
		for (int k = i + 1; k < rows_num; k++) {
			coef = -A_proc_matrix[k][col];
			for (int j = 0; j < N; j++) {
				A_proc_matrix[k][j] = A_proc_matrix[k][j] + A_proc_matrix[i][j] * coef;
			}
			coef = -B_proc_matrix[k][col];
			for (int j = 0; j < N; j++) {
				B_proc_matrix[k][j] = B_proc_matrix[k][j] + B_proc_matrix[i][j] * coef;
			}
		}
	}

	for (int i = 0; i < size; i++) {
		if (i != rank) {
			MPI_Send(&A_proc_matrix, N * N, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
			MPI_Send(&B_proc_matrix, N * N, MPI_FLOAT, i, 1, MPI_COMM_WORLD);
			MPI_Send(&rows_num, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
		}
	}

	float recv_a[N][N];
	float recv_b[N][N];
	int recv_rows_num = 0;
	for (int proc = 0; proc < size; proc++) {
		if (proc != rank) {
			MPI_Recv(&recv_a, N * N, MPI_FLOAT, proc, 0, MPI_COMM_WORLD, &st);
			MPI_Recv(&recv_b, N * N, MPI_FLOAT, proc, 1, MPI_COMM_WORLD, &st);
			MPI_Recv(&recv_rows_num, 1, MPI_INT, proc, 2, MPI_COMM_WORLD, &st);

			int* recv_displ_data = displ[proc];
			for (int i = 0; i < recv_rows_num; i++) {
				int recv_row_num = recv_displ_data[i];
				for (int j = 0; j < rows_num; j++) {
					int row_num = displ_data[j];
					//if (recv_row_num < row_num) {
						float coef = -A_proc_matrix[j][recv_row_num];
						for (int k = 0; k < N; k++) {
							A_proc_matrix[j][k] = A_proc_matrix[j][k] + coef * recv_a[i][k];
						}
						coef = -B_proc_matrix[j][recv_row_num];
						for (int k = 0; k < N; k++) {
							B_proc_matrix[j][k] = B_proc_matrix[j][k] + coef * recv_b[i][k];
						}
					//}
				}
			}
		}
	}

	displ_data = displ[rank];
	for (int i = 0; i < rows_num; i++) {
		int col = displ_data[i];
		float coef = A_proc_matrix[i][col];
		for (int j = 0; j < N; j++) {
			A_proc_matrix[i][j] = A_proc_matrix[i][j] / coef;
		}
		coef = B_proc_matrix[i][col];
		for (int j = 0; j < N; j++) {
			B_proc_matrix[i][j] = B_proc_matrix[i][j] / coef;
		}
		for (int k = i + 1; k < rows_num; k++) {
			coef = -A_proc_matrix[k][col];
			for (int j = 0; j < N; j++) {
				A_proc_matrix[k][j] = A_proc_matrix[k][j] + A_proc_matrix[i][j] * coef;
			}
			coef = -B_proc_matrix[k][col];
			for (int j = 0; j < N; j++) {
				B_proc_matrix[k][j] = B_proc_matrix[k][j] + B_proc_matrix[i][j] * coef;
			}
		}
	}
	

	float result_matrix_a[N][N];
	float result_matrix_b[N][N];
	if (rank != 0) {
		MPI_Send(&A_proc_matrix, N * N, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
		MPI_Send(&B_proc_matrix, N * N, MPI_FLOAT, 0, 1, MPI_COMM_WORLD);
		MPI_Send(&rows_num, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
	}
	else {
		int* displ_data = displ[0];
		for (int i = 0; i < rows_num; i++) {
			int row_num = displ_data[i];
			for (int j = 0; j < N; j++) {
				result_matrix_a[row_num][j] = A_proc_matrix[i][j];
				result_matrix_b[row_num][j] = B_proc_matrix[i][j];
			}
		}
		for (int proc = 1; proc < size; proc++) {
			MPI_Recv(&recv_a, N * N, MPI_FLOAT, proc, 0, MPI_COMM_WORLD, &st);
			MPI_Recv(&recv_b, N * N, MPI_FLOAT, proc, 1, MPI_COMM_WORLD, &st);
			MPI_Recv(&recv_rows_num, 1, MPI_INT, proc, 2, MPI_COMM_WORLD, &st);

			int* recv_displ_data = displ[proc];
			for (int i = 0; i < recv_rows_num; i++) {
				int recv_row_num = recv_displ_data[i];
				for (int j = 0; j < N; j++) {
					result_matrix_a[recv_row_num][j] = recv_a[i][j];
					result_matrix_b[recv_row_num][j] = recv_b[i][j];
				}
			}
		}
	}

	if (rank == 0) {
		print_matrix(result_matrix_a);
		print_matrix(result_matrix_b);
	}
	
	MPI_Finalize();
}