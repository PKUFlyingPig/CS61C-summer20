#include <omp.h>
#include <stdlib.h>
#include <stdio.h>


#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif

struct node {
   int data;
   int fibdata;
   struct node* next;
};

struct node* init_list(struct node* p);
void processwork(struct node* p); 
int fib(int n); 

int fib(int n) 
{
   int x, y;
   if (n < 2) {
      return (n);
   } else {
      x = fib(n - 1);
      y = fib(n - 2);
	  return (x + y);
   }
}

void processwork(struct node* p) 
{
   int n, temp;
   n = p->data;
   temp = fib(n);

   p->fibdata = temp;

}

struct node* init_list(struct node* p) 
{
    int i;
    struct node* head = NULL;
    struct node* temp = NULL;
    
    head = malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i=0; i< N; i++) {
       temp  = malloc(sizeof(struct node));
       p->next = temp;
       p = temp;
       p->data = FS + i + 1;
       p->fibdata = i+1;
    }
    p->next = NULL;
    return head;
}

int main() 
{
     double start, end;
     struct node *p=NULL;
     struct node *temp=NULL;
     struct node *head=NULL;

     printf("Process linked list\n");
     printf("  Each linked list node will be processed by function 'processwork()'\n");
     printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n",N,FS);      

     p = init_list(p);
     head = p;

     start = omp_get_wtime();

	#pragma omp parallel 
	{
            #pragma omp master
                  printf("Threads:      %d\n", omp_get_num_threads());

		#pragma omp single
		{
			p=head;
			while (p) {
				#pragma omp task firstprivate(p) //first private is required
				{
					processwork(p);
				}
			  p = p->next;
		   }
		}
	}

     end = omp_get_wtime();
     p = head;
	 while (p != NULL) {
        printf("%d : %d\n",p->data, p->fibdata);
        temp = p->next;
        free (p);
        p = temp;
     }  
	 free (p);

     printf("Compute Time: %f seconds\n", end - start);

     return 0;
}

