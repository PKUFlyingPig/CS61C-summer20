
#include <stdio.h>
#include <omp.h>
#include "random.h"


static long num_trials = 1000000;

int main ()
{
   long i;  long Ncirc = 0;
   double pi, x, y, test, time;
   double r = 1.0;   // radius of circle. Side of squrare is 2*r 

   time = omp_get_wtime();
   #pragma omp parallel
   {

      #pragma omp single
          printf(" %d threads ",omp_get_num_threads());

      seed(-r, r);  
      #pragma omp for reduction(+:Ncirc) private(x,y,test)
      for(i=0;i<num_trials; i++)
      {
         x = drandom(); 
         y = drandom();

         test = x*x + y*y;

         if (test <= r*r) Ncirc++;
       }
    }

    pi = 4.0 * ((double)Ncirc/(double)num_trials);

    printf("\n %ld trials, pi is %lf ",num_trials, pi);
    printf(" in %lf seconds\n",omp_get_wtime()-time);

    return 0;
}
