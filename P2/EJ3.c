#include<stdio.h>
#include<stdlib.h> 
#include<float.h>
#include<pthread.h>
#include<sys/time.h>

#define MAX_THREADS 100
#define BS 64


int T = MAX_THREADS;
double *A,*B,*C;
int N,CB;


void initvalmat(double *mat, int n, double val, int transpose); 
 
void * matmulblks(void * ptr);

void blkmul(double *ablk, double *bblk, double *cblk);

double dwalltime();



/************** MAIN *************/
int main(int argc, char *argv[])
{
  int i, j;

  double timetick;

  
  if ( (argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) || ((N % T) != 0) || ((T > N)))
  {
    printf("\nError en los parámetros. Usage: ./%s N BS (N debe ser multiplo de BS)\n", argv[0]);
    exit(1);
  }
  CB = N / T; //asumo que BS es multiplo de N.
  int ids[T];
  pthread_attr_t attr;
  pthread_t threads[MAX_THREADS];
  int *status;
  pthread_attr_init(&attr);


  A = (double *) malloc(N*N*sizeof(double));
  B = (double *) malloc(N*N*sizeof(double));
  C = (double *) malloc(N*N*sizeof(double));


  initvalmat(A, N, 1.0, 0);
  initvalmat(B, N, 1.0, 1);
  initvalmat(C, N, 0.0, 0);

 // printf("Multiplying %d x %d matrices\n", n, n);
  
//creo los hilos
  timetick = dwalltime();
  for (i = 0; i < T ; i++){
     ids[i] = i;
     pthread_create(&threads[i], &attr, matmulblks, &ids[i]);
  }
  //espero a que retornen
  for (i = 0; i < T; i++) {
     pthread_join(threads[i], (void*) &status);
  }
  
  double workTime = dwalltime() - timetick;

  printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",N,N, dwalltime() - timetick);

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if (C[i*N + j] != N)
      {
        printf("Error at %d, %d, value: %f\n", i, j, C[i*N + j]);
      }
    }
  }

  printf("MMBLK-SEC;%d;%d;%lf;%lf\n",N,BS,workTime,((double)2*N*N*N)/(workTime*1000000000));

  return 0;
}

/*****************************************************************/


void initvalmat(double *mat, int n, double val, int transpose)
{
  int i, j;

	if (transpose == 0) {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[i*n + j] = val;
		}
	  }
	} else {
	  for (i = 0; i < n; i++)
	  {
		for (j = 0; j < n; j++)
		{
		  mat[j*n + i] = val;
		}
	  }
	}
}

/*****************************************************************/


void * matmulblks(void * ptr)
{
  int i, j, k;
  int *p, id;
  p = (int *) ptr;
  id = *p;
  int inicio = id * CB;
  int fin = inicio + CB;
  
  for (i = inicio; i < fin; i+=BS){
    for (j = 0; j < N; j += BS)
    {
      for  (k = 0; k < N; k += BS)
      {
        blkmul(&A[i*N + k], &B[j*N + k], &C[i*N + j]);
      }
    }
  }
  pthread_exit((void*)ptr);
}

/*****************************************************************/


void blkmul(double *ablk, double *bblk, double *cblk)
{
  int i, j, k;

  for (i = 0; i < BS; i++)
  {
    for (j = 0; j < BS; j++)
    {
      for  (k = 0; k < BS; k++)
      {
        cblk[i*N + j] += ablk[i*N + k] * bblk[j*N + k];
      }
    }
  }
}
    
/*****************************************************************/

#include <stdio.h>
#include <sys/time.h>

double dwalltime()
{
	double sec;
	struct timeval tv;

	gettimeofday(&tv,NULL);
	sec = tv.tv_sec + tv.tv_usec/1000000.0;
	return sec;
}
