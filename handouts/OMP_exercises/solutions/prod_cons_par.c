/*
**  PROGRAM: A simple SPMD producer/consumer program
**
**  PURPOSE: this is just a stupid little program to play around
**  with different ways data is shared between threads.
**
**  HISTORY: Written by Tim Mattson, April 2007.
*/
#include "omp.h"
#ifndef APPLE
#include <malloc.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#define N        10000
#define Nthreads 2

/* Some random number constants from numerical recipies */
#define SEED       2531
#define RAND_MULT  1366
#define RAND_ADD   150889
#define RAND_MOD   714025
int randy = SEED;

/* function to fill an array with random numbers */
void fill_rand(int length, double *a)
{
   int i; 
   for (i=0;i<length;i++) {
     randy = (RAND_MULT * randy + RAND_ADD) % RAND_MOD;
     *(a+i) = ((double) randy)/((double) RAND_MOD);
   }   
}

/* function to sum the elements of an array */
double Sum_array(int length, double *a)
{
   int i;  double sum = 0.0;
   for (i=0;i<length;i++)  sum += *(a+i);  
   return sum; 
}
  
int main()
{
  double *A, sum, runtime;
  int numthreads, flag = 0;

  omp_set_num_threads(Nthreads);

  A = (double *)malloc(N*sizeof(double));

  #pragma omp parallel
  {
     #pragma omp master
     {
        numthreads = omp_get_num_threads();
        if(numthreads != 2)
        {
           printf("error: incorect number of threads, %d \n",numthreads);
           exit(-1);
        }
        runtime = omp_get_wtime();
     }
     #pragma omp barrier

     #pragma omp sections
     {
        #pragma omp section
        {
           fill_rand(N, A);
           #pragma omp flush
           flag = 1;
           #pragma omp flush (flag)
        }
        #pragma omp section
        {
           #pragma omp flush (flag)
           while (flag != 1){
              #pragma omp flush (flag)
           }

           #pragma omp flush 
           sum = Sum_array(N, A);
        }
      }
      #pragma omp master
         runtime = omp_get_wtime() - runtime;
   }  

   printf(" with %d threads and %lf seconds, The sum is %lf \n",numthreads,runtime,sum);
}
 
