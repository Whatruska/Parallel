/* Подключаем вывод, многопоточку и библиотеку работы со временем */
#include <iostream>
#include <omp.h>
#include <time.h>

const int ARR_SIZE = 10;
const int ARR_MAX_VAL = 100;

/* Делаем метод распечатки массива */
void print_arr (int arr[])
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
	/* 
		Задаем рандому сид связанный со временем
		Иначе - будут генерироваться одинаковые данные
	*/
	srand(time(NULL));

	int a[ARR_SIZE];
	int b[ARR_SIZE];
	for (int i = 0; i < ARR_SIZE; i++)
	{
		a[i] = rand() % ARR_MAX_VAL;
		b[i] = rand() % ARR_MAX_VAL;
	}

	int min = INT_MAX;
	int max = INT_MIN;

	/*
		Создаем 2 потока и даем им один и даем им один и тот же код
	*/
#pragma omp parallel num_threads(2)
	{
		int num = omp_get_thread_num();
		/*
			Если работаем с первым потоком - ищем минимум
		*/
		if (num == 0)
		{
			for (int i = 0; i < ARR_SIZE; i++)
			{
				int val = a[i];
				if (val < min)
				{
					min = val;
				}
			}
		}
		/*
			Если работаем со вторым потоком - ищем максимум
		*/
		else
		{
			for (int i = 0; i < ARR_SIZE; i++)
			{
				int val = b[i];
				if (val > max)
				{
					max = val;
				}
			}
		}
	}

	print_arr(a);
	print_arr(b);
	printf("min = %d, max = %d", min, max);
}