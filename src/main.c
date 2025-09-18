#include <omp.h>
#include <stdint.h>
#include <stdio.h>

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
	return 0;
}
