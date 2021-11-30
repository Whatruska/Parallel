#include "Header.h"
#include <mpi.h>
#include <iostream>

const int SIZE = 8;
const int MAX_VAL = 1000;

void sixth_task() {
        int rank = 0;
        int size = 0;

        int min = INT_MAX;
        int max = INT_MIN;

        int sum = 0;                
        int rec_buf[100];          

        srand(time(NULL));

        int data[SIZE][SIZE];

        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                data[i][j] = rand() % MAX_VAL;
            }
        }

        MPI_Init(NULL, NULL);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int rem = SIZE % size;

        int* sendcounts = new int[size];
        int* displs = new int[size];

        for (int i = 0; i < size; i++) {
            sendcounts[i] = (SIZE / size) * SIZE;
            if (rem > 0) {
                sendcounts[i] += SIZE;
                rem--;
            }

            displs[i] = sum;
            sum += sendcounts[i];
        }

        MPI_Scatterv(&data, sendcounts, displs, MPI_INT, &rec_buf, 100, MPI_INT, 0, MPI_COMM_WORLD);

        int elements_count = sendcounts[rank];
        int row_count = elements_count / SIZE;
        int minmax = INT_MIN;
        int maxmin = INT_MAX;
        for (int i = 0; i < row_count; i++) {
            int max = INT_MIN;
            int min = INT_MAX;
            for (int j = 0; j < SIZE; j++) {
                int index = i * SIZE + j;
                int val = rec_buf[index];
                if (val > max) {
                    max = val;
                }
                else if (val < min) {
                    min = val;
                }
            }
            if (max < maxmin) {
                maxmin = max;
            }
            else if (min > minmax) {
                minmax = min;
            }
        }

        MPI_Reduce(&minmax, &max, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        MPI_Reduce(&maxmin, &min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);

        if (rank == 0) {
            printf("minmax = %d; maxmin = %d", minmax, maxmin);
        }

        MPI_Finalize();
}
