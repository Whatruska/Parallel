#include <iostream>
#include <omp.h>

using namespace std;

int thread_num = 0;

void set_threads_num(int val)
{
	omp_set_dynamic(0);
	omp_set_num_threads(val);
	thread_num = val;
}

int main()
{
	set_threads_num(3);
#pragma omp parallel if (thread_num > 2)
	{
		int rank = omp_get_thread_num();
		printf("FIRST BLOCK: %d of %d\n", rank, omp_get_num_threads());
	}

	set_threads_num(2);
#pragma omp parallel if (thread_num > 1)
	{
		int rank = omp_get_thread_num();
		printf("SECOND BLOCK: %d of %d\n", rank, omp_get_num_threads());
	}
}
