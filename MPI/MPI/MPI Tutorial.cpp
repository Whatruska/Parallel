#include <iostream>
#include <mpi.h>
#include "Header.h"

int mpi()
{
    MPI_Status st;
    int sum, rank, size = 0;
    int proc_sum = 0;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int N = 10000;
    const int MAX_VAL = 10;
    int a[N];

    srand(time(NULL));

    if (rank == 0)
    {
        for (int i = 0; i < N; i++)
        {
            a[i] = rand() % MAX_VAL;
        }
    }

    MPI_Bcast(a, N, MPI_INT, 0, MPI_COMM_WORLD);

    int* len = new int[size];
    int* ind = new int[size];
    int rest = N;
    int k = N / size;
    ind[0] = 0;
    len[0] = k;

    for (int i = 1; i < size; i++) {
        rest -= k;
        k = rest / (size - i);
        len[i] = k;
        ind[i] = ind[i - 1] + len[i - 1];
    }

    int proc_size = len[rank];
    int* proc_a = new int[proc_size];

    //Справедливо делим у кого сколько элементов и рассылаем всем инфу об этом
    MPI_Scatterv(a, len, ind, MPI_INT, proc_a, proc_size, MPI_INT, 0, MPI_COMM_WORLD);

    //MPI_Gatherv - получаем по кусочку массива от компа и собираем общий массив на руте
    //MPI_Allgatherv - все получают собранный массив
    //MPI_Alltoallv - scatter + gather

    for (int i = 0; i < proc_size; i++) {
        proc_sum += proc_a[i];
    }

    /*
        int k = N / size;
    int i1 = k * rank;
    int i2 = k * (rank + 1);

    if (rank == size - 1)
    {
        i2 = N;
    }
    */

    //Делает все что закомменчено ниже
    MPI_Reduce(&proc_sum, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    //MPI_Allreduce - отдает сумм всем

    printf("(%d): ind=%d len=%d proc_sum=%d\n", rank, ind[rank], len[rank], proc_sum);

    if (rank == 0)
    {
        printf("Sum: %d\n", sum);
    }
   
    /*
    if (rank == 0)
    {
        sum = proc_sum;
#pragma parallel for thread_num(size - 1)
        {
            for (int i = 1; i < size; i++) {
                MPI_Recv(&proc_sum, k, MPI_INT, i, 0, MPI_COMM_WORLD, &st);
                sum += proc_sum;
            }
        }
        printf("Sum: %d", sum);
    }
    else
    {
        MPI_Send(&proc_sum, k, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    */

    MPI_Finalize();

    return EXIT_SUCCESS;
}