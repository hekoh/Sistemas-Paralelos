#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<pthread.h>
#include<sys/time.h>
#include<semaphore.h>
#include<limits.h>

#define MAX_THREADS 100

int T = MAX_THREADS;
int *V;
int N,L;
double PROM = 0;
int MIN = INT_MAX;
int MAX = -1;
sem_t SEM;

double dwalltime();
void * tSync (void * ptr);
void * recorrido(int inicio);

int main(int argc,char*argv[]){
  double timetick;
  int ids[MAX_THREADS];
  pthread_attr_t attr;
  pthread_t threads[MAX_THREADS];

  int *status;

    if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <=0 ) || (T > N)) {
      printf("\nUsar: %s n\n  n: longitud del vector \n T: numero de nucleos\n", argv[0]);
      exit(1);
      }

  pthread_attr_init(&attr);
  sem_init(&SEM,NULL,1);
  V=(int*)malloc(sizeof(int)*N);
  L = N/T; //asumo que N es divisible por T. Se puede aplicar una solucion
           //sin asumir esto, pero es menos eficiente. NO aprovecha el
           //principio de localidad espacial



  for(int i = 0; i < N; i++) //inicializo vector con valores del 0 - N
    V[i] = rand() %N;


 //creo los hilos
  timetick = dwalltime();
  for (int i = 0; i < T ; i++){
        ids[i] = i;
        pthread_create(&threads[i], &attr, tSync, &ids[i]);
  }
//espero a que retornen
  for (int i = 0; i < T; i++) {
        pthread_join(threads[i], (void*) &status);
  }
  double time = dwalltime() - timetick;

  printf("Vector de tamaño %i. -  Promedio: %f. - Maximo: %i. - Minimo: %i.\n" 
  "Tiempo en segundos %f\n",N,PROM/N,MAX,MIN,time);

  free(V);
  return(0);
}


/*-------------------------------------------------------*/
void * tSync (void * ptr){
  int id = *(int *)ptr;
  int inicio = id*L;
  recorrido(inicio);
  pthread_exit((void*)ptr);
}

void * recorrido(int inicio){
  int cant = 0;
  int fin = inicio + L;
  int aux;
  int min = INT_MAX;
  int max = -1;
  for(int i = inicio; i < fin; i++){
      aux = V[i];
      if(aux < min)
          min = aux;
      if(aux > max)
	  max = aux;
      cant+= aux;
  }
  sem_wait(&SEM);
  if(min < MIN)
      MIN = min;
  if(max > MAX)
      MAX = max;
  PROM += cant;
  sem_post(&SEM);
}

/*-------------------------------------------------------*/

double dwalltime()
{
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}



