#include <iostream>
#include <omp.h>
#include <time.h>

const int ARR_SIZE = 30;
const int ARR_MAX_VAL = 100;

void print_arr(int arr[])
{
	for (int i = 0; i < ARR_SIZE; i++)
	{
		if (i == 0)
		{
			printf("[");
		}
		printf("%d", arr[i]);
		if (i != ARR_SIZE - 1)
		{
			printf(", ");
		}
		else
		{
			printf("]\n");
		}
	}
}

int main()
{
	srand(time(NULL));

	int a[ARR_SIZE];
	for (int i = 0; i < ARR_SIZE; i++)
	{
		a[i] = rand() % ARR_MAX_VAL;
	}

	int count = 0;

#pragma omp parallel for
	{
		for (int i = 0; i < ARR_SIZE; i++)
		{
			int val = a[i];
			if (val % 9 == 0)
			{
#pragma omp atomic
				count++;
			}
		}
	}

	print_arr(a);
	printf("count = %d", count);
}