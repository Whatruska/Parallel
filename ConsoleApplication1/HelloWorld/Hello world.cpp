

#include <iostream>
#include <omp.h>

int main()
{
#pragma omp parallel 
	{
		int size = omp_get_num_threads();
		int rank = omp_get_thread_num();
		printf("Hello world! %d out of %d\n", rank, size);
	}
	return EXIT_SUCCESS;
}

/*
 Input changes due to output access
*/
