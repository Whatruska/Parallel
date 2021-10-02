#include <iostream>
#include <omp.h>

int main()
{
	omp_set_num_threads(3);
#pragma omp parallel if (omp_get_num_threads() > 2) num_threads(3)
	{
		printf("AAA\n");
	}
	omp_set_num_threads(2);
#pragma omp parallel if (omp_get_num_threads() > 2)
	{
		printf("BBB\n");
	}
	return EXIT_SUCCESS;
}