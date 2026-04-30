#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<pthread.h>
#include<sys/time.h>

#define MAX_THREADS 100

int T = MAX_THREADS;
double *A,*B,*C;
int N;


double dwalltime();
void * tSync (void * ptr);


int main(int argc,char*argv[]){
int i;
double timetick;
int check = 1;
int ids[MAX_THREADS];
pthread_attr_t attr;
pthread_t threads[MAX_THREADS];

int *status;


//Controla los argumentos al programa
if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <=0 ) || T > N){
    printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n T: numero de nucleos\n", argv[0]);
    exit(1);
  }

pthread_attr_init(&attr);

//Aloca memoria para las matrices
A=(double*)malloc(sizeof(double)*N);
B=(double*)malloc(sizeof(double)*N);
C=(double*)malloc(sizeof(double)*N);


//Inicializa los vectores en 1, el resultado será una vector con todos sus valores en 2
  for(i=0;i<N;i++)
        A[i] = 1;
  for(i=0;i<N;i++)
        B[i] = 1;

//creo los hilos
timetick = dwalltime();
  for (i = 0; i < T ; i++){
  	ids[i] = i;
	pthread_create(&threads[i], &attr, tSync, &ids[i]);
  }
//espero a que retornen
  for (i = 0; i < T; i++) {
	pthread_join(threads[i], (void*) &status);
  }


  double time = dwalltime() - timetick;
  printf("Suma de vectores de dimensión %d. Tiempo en segundos %f\n",N,time);
 

 //Verifica el resultado
  for(i=0;i<N;i++)
        check=check&&(C[i]==2);

  if(check){
   printf("Suma de vectores resultado correcto\n");
  }else{
   printf("Suma de vectores resultado erroneo\n");
  }


  free(A);
  free(B);
  free(C);
  return(0);
}


void * tSync(void * ptr){
  int pos = 0;
  int * p, id;
  p = (int *) ptr;
  id = *p;
  printf("%i",id);
  while((id+T*pos) < N){
    C[id+T*pos] = A[id+T*pos] + B [id+T*pos];
    pos+=1;
  }
  pthread_exit((void*)ptr);
}


double dwalltime()
{
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}


