#include <stdio.h>
#include <omp.h>
 
int main ()  
{
   int nthreads = 4;
   omp_set_num_threads(nthreads);

   #pragma omp parallel
   {
      int id = omp_get_thread_num();

      printf("Hello World from thread = %d", id);
      printf(" with %d threads\n",omp_get_num_threads());
   }  

   printf("all done, with hopefully %d threads\n",nthreads);

}

 

