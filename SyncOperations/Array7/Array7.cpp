#include <iostream>
#include <omp.h>
#include <time.h>

const int ARR_SIZE = 100;
const int ARR_MAX_VAL = 1000;

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

	int max = INT_MIN;

#pragma omp parallel for
	{
		for (int i = 0; i < ARR_SIZE; i++)
		{
			int val = a[i];
			if (val % 7 == 0 && val > max)
			{
#pragma omp critical
				{
					if (val > max)
					{
						max = val;
					}
				}
			}
		}
	}

	print_arr(a);
	printf("max = %d", max);
}