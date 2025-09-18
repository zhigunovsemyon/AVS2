#include <omp.h>
#include <stdio.h>

static void task2()
{
	double t = omp_get_wtime();
	#pragma omp parallel
	{

	}
	t -= omp_get_wtime();
	t *= -1;
	printf("t = %lf\n", t);
}

static void task1()
{

}

int main()
{
	task1();
	task2();
	return 0;
}
