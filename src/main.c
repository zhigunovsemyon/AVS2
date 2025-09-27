#include <omp.h>
#include <stdint.h>
#include <stdio.h>

static void task7()
{
	puts("\nЗадание 7");
	int var = 0;
#pragma omp parallel reduction(+:var)
	{
		var = 1;
	}
	printf("var = %d\n", var);
}

static void task6()
{
	puts("\nЗадание 6");
	int m[5] = {};

	puts("Перед параллельной областью:");
	for (size_t i = 0; i < sizeof(m) / sizeof(*m); ++i)
		printf("m[%lu] = %d; ", i, m[i]);
	putchar('\n');

#pragma omp parallel num_threads(2)
	{
		m[omp_get_thread_num()] = 1;
	}

	puts("После параллельной области:");
	for (size_t i = 0; i < sizeof(m) / sizeof(*m); ++i)
		printf("m[%lu] = %d; ", i, m[i]);
	putchar('\n');
}

static void task5()
{
	puts("\nЗадание 5");
	int n = 10;
	printf("Последовательная область. n = %d\n", n);
#pragma omp parallel num_threads(2) private(n)
	{
		// приватная для каждого потока
		int thread_num = omp_get_thread_num();
		printf("Параллельная область. Поток №%d, n = %d\n", thread_num,
		       n);

		n = thread_num;
		printf("Параллельная область. Поток №%d, n = %d\n", thread_num,
		       n);
	}
	printf("Последовательная область. n = %d\n", n);
}

static void task4()
{
	puts("\nЗадание 4");
#pragma omp parallel num_threads(3)
	{
		puts("Начало");

#pragma omp master
		printf("Главный поток №%d\n", omp_get_thread_num());

		puts("Середина");

#pragma omp master
		printf("Главный поток №%d\n", omp_get_thread_num());

		puts("Конец");
	}
}

static void task3()
{
	puts("\nЗадание 3");
#pragma omp parallel num_threads(3)
	{
		printf("Начало %d\n", omp_get_thread_num());

#pragma omp single
		printf("Один поток %d\n", omp_get_thread_num());

		printf("Окончание %d\n", omp_get_thread_num());
	}
}

static void task3_nowait()
{
	puts("\nЗадание 3");
#pragma omp parallel num_threads(3)
	{
		printf("Начало %d\n", omp_get_thread_num());

#pragma omp single nowait
		printf("Один поток nowait %d\n", omp_get_thread_num());

		printf("Окончание %d\n", omp_get_thread_num());
	}
}

static void task2()
{
	puts("\nЗадание 2");
	double t = omp_get_wtime();
#pragma omp parallel
	{
	}
	t -= omp_get_wtime();
	t *= -1;
	printf("t = %lf\n", t);
}

static double task1_serial(double a, double b, int64_t iter_count)
{
	double start_time = omp_get_wtime();
	double foo;
	for (int64_t i = 0; i < iter_count; ++i) {
		foo = a * b;
		(void)foo;
	}

	return omp_get_wtime() - start_time;
}

static double task1_parallel(double a, double b, int64_t iter_count)
{
	double start_time = omp_get_wtime();
	double foo;

#pragma omp parallel num_threads(2)
	{

// без private(foo) значительное замедление
#pragma omp for private(foo)
		for (int64_t i = 0; i < iter_count; ++i) {
			foo = a * b;
			(void)foo;
		}
	}

	return omp_get_wtime() - start_time;
}

static void task1(int64_t iter_count)
{
	puts("Задание 1");
	double a, b;
	printf("Введите число a: ");
	scanf("%lf", &a);
	printf("Введите число b: ");
	if (0 >= scanf("%lf", &b)) {
		putchar('\n');
		return;
	}
	printf("a = %lf\nb = %lf\nчисло итераций = %ld\n", a, b, iter_count);

	printf("Время последовательного умножения: %lfs\n",
	       task1_serial(a, b, iter_count));
	printf("Время параллельного умножения: %lfs\n",
	       task1_parallel(a, b, iter_count));
}

int main()
{
	task1(4 * (int64_t)UINT32_MAX);
	task2();
	task3();
	task3_nowait();
	task4();
	task5();
	task6();
	task7();
	return 0;
}
