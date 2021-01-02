/*

NAME: PI SPMD ... a simple version.

This program will numerically compute the integral of

                  4/(1+x*x) 
				  
from 0 to 1.  The value of this integral is pi -- which 
is great since it gives us an easy way to check the answer.

The program was parallelized using OpenMP and an SPMD 
algorithm.  The following OpenMP specific lines were 
added: 

(1) A line to include omp.h -- the include file that 
contains OpenMP's function prototypes and constants.

(2) A pragma that tells OpenMP to create a team of threads
with an integer variable i being created for each thread.

(3) two function calls: one to get the thread ID (ranging
from 0 to one less than the number of threads), and the other
returning the total number of threads.

(4) A cyclic distribution of the loop by changing loop control
expressions to run from the thread ID incremented by the number 
of threads.  Local sums accumlated into sum[id].

Note that this program will show low performance due to 
false sharing.  In particular, sum[id] is unique to each
thread, but adfacent values of this array share a cache line
causing cache thrashing as the program runs.

History: Written by Tim Mattson, 11/99.

*/

#include <stdio.h>
#include <omp.h>

#define MAX_THREADS 4

static long num_steps = 100000000;
double step;
int main ()
{
	  int i,j;
	  double pi, full_sum = 0.0;
	  double start_time, run_time;
	  double sum[MAX_THREADS];

	  step = 1.0/(double) num_steps;


   for (j=1;j<=MAX_THREADS ;j++) {

      omp_set_num_threads(j);
      full_sum=0.0;
      start_time = omp_get_wtime();

      #pragma omp parallel
      {
        int i;
	  int id = omp_get_thread_num();
	  int numthreads = omp_get_num_threads();
	  double x;

	  sum[id] = 0.0;

        if (id == 0) 
             printf(" num_threads = %d",numthreads);

	  for (i=id;i< num_steps; i+=numthreads){
		  x = (i+0.5)*step;
		  sum[id] = sum[id] + 4.0/(1.0+x*x);
	  }
      }

	for(full_sum = 0.0, i=0;i<j;i++)
	    full_sum += sum[i];

      pi = step * full_sum;
      run_time = omp_get_wtime() - start_time;
      printf("\n pi is %f in %f seconds %d thrds \n",pi,run_time,j);
   }
}	  





