// Several versions of serial codes for matrix-matrix multiplication

#include <stdio.h>   
#include <stdlib.h> 
#include <omp.h>
#include "2DArray.h"

// define sizes of matrices to be used
#define MM 1000
#define NN 1000
#define PP 1000

double dabs(double d){return (d<0.0?d:(-d));}

// Default triple-nested loop for matrix-matrix multiplication
void matmult1(int m, int n, int p, double **A, double **B, double **C)   
{   
	int i, j, k; 

	for (i = 0; i < m; i++)   
		for (j = 0; j < n; j++){          
			C[i][j]=0;   
			for (k = 0; k < p; k++)   
				C[i][j] += A[i][k]*B[k][j];   
		}   
}  


/*   
  Recursive code for matrix multiplication.   
  The recursion uses the formula  
  C00 = A00*B00 + A01*B10  
  C01 = A00*B01 + B01*B11  
  C10 = A10*B00 + A11*B10  
  C11 = A10*B01 + A11*B11  
*/  
  
void matmultleaf(int mf, int ml, int nf, int nl, int pf, int pl, double **A, double **B, double **C)    
/*  
  subroutine that uses the simple triple loop to multiply  
  a submatrix from A with a submatrix from B and store the  
  result in a submatrix of C.   
  (We could use a tiled version,for better performance)  
*/  
// mf, ml; /* first and last+1 i index */  
// nf, nl; /* first and last+1 j index */  
// pf, pl; /* first and last+1 k index */  
{
	int i,j,k;   
	for (i = mf; i < ml; i++)   
		for (j = nf; j < nl; j++)   
			for (k = pf; k < pl; k++)   
				C[i][j] += A[i][k]*B[k][j];   
}   
  
#define GRAIN  32768 /* product size below which matmultleaf is used */  
  
void matmultrec(int mf, int ml, int nf, int nl, int pf, int pl, double **A, double **B, double **C)
/*    
  recursive subroutine to compute the product of two  
  submatrices of A and B and store the result in C  
*/  
// mf, ml; /* first and last+1 i index */  
// nf, nl; /* first and last+1 j index */  
// pf, pl; /* first and last+1 k index */  

{     
//
// Check sizes of matrices; 
// if below threshold then compute product w/o recursion
//
	if ((ml-mf)*(nl-nf)*(pl-pf) < GRAIN)   
		matmultleaf(mf, ml, nf, nl, pf, pl, A, B, C);   
	else {   
//
// Apply OpenMP tasks to the eight recursive calls below
//   be sure to not create data races between tasks
//
   // C00 += A00 * B00
		matmultrec(mf, mf+(ml-mf)/2, nf, nf+(nl-nf)/2, pf, pf+(pl-pf)/2, A, B, C);   
   // C01 += A00 * B01
		matmultrec(mf, mf+(ml-mf)/2, nf+(nl-nf)/2, nl, pf, pf+(pl-pf)/2, A, B, C);   
   // C00 += A01 * B10
		matmultrec(mf, mf+(ml-mf)/2, nf, nf+(nl-nf)/2, pf+(pl-pf)/2, pl, A, B, C);   
   // C01 += A01 * B11
		matmultrec(mf, mf+(ml-mf)/2, nf+(nl-nf)/2, nl, pf+(pl-pf)/2, pl, A, B, C);   
   // C10 += A10 * B00
		matmultrec(mf+(ml-mf)/2, ml, nf, nf+(nl-nf)/2, pf, pf+(pl-pf)/2, A, B, C);   
   // C11 += A10 * B01
		matmultrec(mf+(ml-mf)/2, ml, nf+(nl-nf)/2, nl, pf, pf+(pl-pf)/2, A, B, C);   
   // C10 += A11 * B10
		matmultrec(mf+(ml-mf)/2, ml, nf, nf+(nl-nf)/2, pf+(pl-pf)/2, pl, A, B, C);   
   // C11 += A11 * B11
		matmultrec(mf+(ml-mf)/2, ml, nf+(nl-nf)/2, nl, pf+(pl-pf)/2, pl, A, B, C);   
	}   
}   
              

//
//  "Helper" function to intialize C and start recursive routine
//
void matmultr(int m, int n, int p, double **A, double **B, double **C)
{   
	int i,j; 

	for (i = 0; i < m; i++)   
		for (j=0; j < n; j++)   
			C[i][j] = 0;   

	matmultrec(0, m, 0, n, 0, p, A, B, C);   
}  

int CheckResults(int m, int n, double **C, double **C1)
{
#define ERR_THRESHOLD 0.001
   int code = 0;
//
//  May need to take into consideration the floating point roundoff error
//    due to parallel execution
//
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (dabs(C[i][j] - C1[i][j]) > ERR_THRESHOLD ) {
        printf("%f  %f at [%d][%d]\n", C[i][j], C1[i][j], i, j);
        code = 1;
      }
    }
  }
  return code;
}

  
int main(int argc, char* argv[])   
{      
	int i, j; 
	double start, time1, time2;

   int M = MM;
   int N = NN;
   int P = PP;
 
//
// If 3 values on command line, use those for matrix sizes
//
   if (argc != 4) {
      printf("Suggested Usage: %s <M> <N> <P> \n", argv[0]);
      printf("Using default values\n");
   }
   else {
      M = atoi(argv[1]);
      N = atoi(argv[2]);
      P = atoi(argv[3]);
   }

	double  **A = Allocate2DArray< double >(M, P);
	double  **B = Allocate2DArray< double >(P, N);

	double **C1 = Allocate2DArray< double >(M, N);
	double **C4 = Allocate2DArray< double >(M, N);

//
// Initialize with random values
//
	for (i = 0; i < M; i++) {   
		for (j = 0; j < P; j++) {   
			A[i][j] = (double)(rand()%100) / 10.0;   
		}      
	}   

	for (i = 0; i < P; i++) {   
		for (j = 0; j < N; j++) {   
			B[i][j] = (double)(rand()%100) / 10.0;   
		}      
	}   

   printf("Matrix Dimensions: M = %d  P = %d  N = %d\n\n", M, P, N);
	printf("Execute matmult1\n");
	start = omp_get_wtime();
	matmult1(M, N, P, A, B, C1);
	time1 = omp_get_wtime() - start;
	printf("Time = %f seconds\n\n",time1);

	printf("Execute matmultr\n");
	start = omp_get_wtime();
	matmultr(M, N, P, A, B, C4);
	time2 = omp_get_wtime() - start;
	printf("Time = %f seconds\n\n",time2);

   printf("Checking...");
   if (CheckResults(M, N, C1, C4))
     printf("Error in Recursive Matrix Multiplication\n\n");
   else {
     printf("OKAY\n\n");
     printf("Speedup = %5.1fX\n", time1/time2);
   }


	Free2DArray< double >(A);
	Free2DArray< double >(B);
	Free2DArray< double >(C1);
	Free2DArray< double >(C4);

	return 0;   
}  
