#include <stdio.h>
#include <omp.h>

#define NUM_THREADS 1
int main()
{
	int i, nthreads;
	double pi = 0, step, start, time;
	static long num_steps = 100000000;
	step = 1.0/(double)num_steps;
	omp_set_num_threads(NUM_THREADS);
	start = omp_get_wtime();
	#pragma omp parallel
	{
		int i, id, nthds;
		double x, fx, sum = 0;
		id = omp_get_thread_num();
		nthds = omp_get_num_threads();
		if (id == 0) nthreads = nthds;
		for (int i = 0; i < num_steps; i += nthds)
		{
			x = (i + 0.5) * step;
			sum += 4/(1+x*x);
		}
		sum *= step;
		#pragma omp atomic
			pi += sum;
	}
	time = omp_get_wtime() - start;
	printf("threads = %d, pi = %.5f, time = %.3f s\n", nthreads, pi, time);
}