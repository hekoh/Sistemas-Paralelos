#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>
#include<float.h>
#include<pthread.h>
#include<semaphore.h>
#include<limits.h>


#define MAX_THREADS 100

#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1
#define BS 64


float *A,*B,*C,*BTrans,*R,NRO;
int N,T = MAX_THREADS;
float maxA = -1, maxB= -1, minA = FLT_MAX, minB = FLT_MAX;
float promA = 0.0, promB = 0.0;
int CB;
sem_t SEM;
pthread_barrier_t BAR;


double dwalltime();

void initvalmat(float *mat, int n, float val, int transpose); 

void matmulblks(int id, float *a, float *b, float *c);

void blkmul(float *ablk, float *bblk, float *cblk);

void * tSync(void * ptr); //algoritmo principal


int main(int argc,char *argv[]){
  double timetick;
	
   if ( (argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <= 0) || ((N % T) != 0) || ((T > N))){
    	printf("\nError en los parámetros. Usage: ./%s N BS (N debe ser multiplo de BS)\n", argv[0]);
   	 exit(1);
   }
	
  CB = (int) N / T; //asumo que N es divisible por T.
  int ids[T];
  pthread_attr_t attr;
  pthread_t threads[MAX_THREADS];
  int *status;
  int check = 1;

  pthread_attr_init(&attr);
  sem_init(&SEM,0,1);
  pthread_barrier_init(&BAR, NULL, T);

  A=(float*)malloc(sizeof(float)*N*N);
  B=(float*)malloc(sizeof(float)*N*N);
  BTrans=(float*)malloc(sizeof(float)*N*N);
  C=(float*)malloc(sizeof(float)*N*N);
  R=(float*)malloc(sizeof(float)*N*N);

/*-------------------- -inicializo matrices- --------------------*/

  initvalmat(C, N, 0.0, 0);

        for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                        A[i*N+j] = i;
                }
        }
        for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                        B[i+j*N] = j;
                }
        }

/*---------------------------------------------------------------*/


  timetick = dwalltime();
   for (int i = 0; i < T ; i++){
     ids[i] = i;
     pthread_create(&threads[i], &attr, tSync, &ids[i]);
  }
  //espero a que retornen
  for (int i = 0; i < T; i++) {
     pthread_join(threads[i], (void*) &status);
  }


  printf("Tiempo recorriendo por matris :%f\t",dwalltime() - timetick);
  printf("tamanio de matriz NxN:%d\n", N);
  printf("maxA: %f\n maxB: %f\n minA: %f\n minB: %f\n NUMERO: %f\n",maxA,maxB,minA,minB,NRO);


        for(int i = 0; i < N; i++){
                for(int j = 0; j < N; j++){
                        check=check&&(C[i*N+j] == (i*j*N));
                }
	}

        if(check)
                printf("Multiplicacion de matrices resultado correcto\n");
        else
                printf("Multiplicacion de matrices resultado erroneo\n");

	
  free(A);
  free(B);
  free(C);
  free(BTrans);
  free(R);
  pthread_barrier_destroy(&BAR);
  sem_destroy(&SEM);
  return(0);
}


/*****************************************************************/

void * tSync(void * ptr){
  int id = *(int *)ptr;
  int inicio = id*CB;
  int fin = inicio + CB;


/*------------------------ -recorrido A- -------------------------*/
  float Amin = FLT_MAX;
  float Amax = -1;
  float auxA;
  float cantA = 0;

  for(int i = inicio; i < fin; i++){
      for(int j = 0; j < N; j++){
        auxA = A[i*N+j];
	if(auxA < Amin)
           Amin = auxA;
      	if(auxA > Amax)
           Amax = auxA;
      	cantA+= auxA;
      }
  }
  sem_wait(&SEM);
  if(Amin < minA)
      minA = Amin;
  if(Amax > maxA)
      maxA = Amax;
  promA += cantA;
  sem_post(&SEM);

/*------------------------ -recorrido B- -------------------------*/
  float Bmin = FLT_MAX;
  float Bmax = -1;
  float auxB;
  float cantB = 0;

  for(int i = inicio; i < fin; i++){
      for(int j = 0; j < N; j++){ 
        auxB = B[i+j*N];
      	if(auxB < Bmin)
           Bmin = auxB;
      	if(auxB > Bmax)
            Bmax = auxB;
      	cantB+= auxB;
      }
  }
  sem_wait(&SEM);
  if(Bmin < minB)
      minB = Bmin;
  if(Bmax > maxB)
      maxB = Bmax;
  promB += cantB;
  sem_post(&SEM);


/*---------------------------------------------------------------*/

  matmulblks(id,A,B,C);
  pthread_barrier_wait(&BAR); //sincronizo los hilos
  matmulblks(id,C,BTrans,R);
  pthread_barrier_wait(&BAR);
  if(id == 0){
      promA /= (float) (N*N);
      promB /= (float) (N*N);
      NRO = ((maxA*maxB)-(minA*minB))/(promA*promB);
  }
  pthread_barrier_wait(&BAR);

  for (int i = inicio; i < fin; i++){ //calculo resultado final en paralelo
      for(int j = 0; j < N; j++){
              R[i*N+j] *= NRO;
      }
  }

  pthread_exit((void*)ptr);
}




/*****************************************************************/
//tranpose = 0: orden por filas
//transpose = 1: orden por columnas

void initvalmat(float *mat ,int n,float val, int transpose)
{
  int i, j; 
	if (transpose == 0){
	  for (i=0;i<n;i++){
		for(j=0;j<n;j++){
		  mat[i*n+j]=val;
		}
	  }
	} else{
	  for(i = 0; i < n; i++){
		for (j=0;j<n;j++){
		  mat[j*n+i]=val;
		}
	  }
	}
}


/*****************************************************************/
void matmulblks(int id, float *a, float *b, float *c)
{
  int i, j, k;
  int inicio = id * CB;
  int fin = inicio + CB;

  for (i = inicio; i < fin; i+=BS){
    for (j = 0; j < N; j += BS)
    {
      for  (k = 0; k < N; k += BS)
      {
        blkmul(&a[i*N + k], &b[j*N + k], &c[i*N + j]);
      }
    }
  }
}


void blkmul(float *ablk, float *bblk, float *cblk)
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

double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

