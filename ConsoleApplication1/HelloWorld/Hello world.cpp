/* Подключаем вывод и многопоточку */
#include <iostream>
#include <omp.h>

int main()
{
/*
	Смотрит сколько ядер свободно на компьютере, 
	на каждое ядро создает поток,
	каждому потоку дает участок кода внутри скобок
	4 ядра - 4 потока - 4 раза выполнится один и тот же код
*/
#pragma omp parallel 
	{
		/* Получаем кол-во потоков */
		int size = omp_get_num_threads();
		/* Получаем номер текущего потока, нумерация с 0 */
		int rank = omp_get_thread_num();
		printf("Hello world! %d out of %d\n", rank, size);
	}
	/* Завершаем работу с кодом "Все окей" */
	return EXIT_SUCCESS;
}
