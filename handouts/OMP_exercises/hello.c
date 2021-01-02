#include <stdio.h>
#include "omp.h"
int main ()  
{
  #pragma omp parallel
 {
  int ID = 0;
  printf("Hello World %d\n", ID);
 }
}
