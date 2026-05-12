#include<stdio.h>
#include<stdlib.h>
#include<float.h>
#include<pthread.h>
#include<sys/time.h>

#define MAX_THREADS 100


int N,T = MAX_THREADS;
int L,X;
int ocurrencias = 0;
int *V;

pthread_mutex_t LOCK;


double dwalltime();
void * vSync (void * ptr);
int recorrido(int inicio);




/*-------------------------MAIN--------------------------*/
int main(int argc, char*argv[]){


  double timetick;
  int ids[MAX_THREADS];
  pthread_attr_t attr;
  pthread_t threads[MAX_THREADS];
  int *status;


  if ((argc != 4) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <=0 ) || (T > N) || ((X = atoi(argv[3])) < 0) || X > 99) {
      printf("\nUsar: %s n\n  n: longitud del vector \n T: numero de nucleos\n X: numero a buscar del 0 al 99\n", argv[0]);
      exit(1);
      }


  pthread_attr_init(&attr);
  pthread_mutex_init(&LOCK, NULL);
  V=(int*)malloc(sizeof(int)*N);
  L = N/T; //asumo que N es divisible por T. Se puede aplicar una solucion
	   //sin asumir esto, pero es menos eficiente. NO aprovecha el
	   //principio de localidad espacial

  for(int i = 0; i < N; i++) //inicializo vector con valores del 0 - 99
      V[i] = rand() %100;


//creo los hilos
  timetick = dwalltime();
  for (int i = 0; i < T ; i++){
        ids[i] = i;
        pthread_create(&threads[i], &attr, vSync, &ids[i]);
  }
//espero a que retornen
  for (int i = 0; i < T; i++) {
        pthread_join(threads[i], (void*) &status);
  }

  double time = dwalltime() - timetick;

  printf("apareciones del elemento %i = %i. Tiempo en segundos %f\n",X, ocurrencias, time);

  free(V);
  return(0);
}

/*-------------------------------------------------------*/

void * vSync(void * ptr){
  int id = *(int *)ptr;
  int inicio = id*L;
  int cant = recorrido(inicio);

  pthread_mutex_lock(&LOCK);
    ocurrencias+= cant;
  pthread_mutex_unlock(&LOCK);
  pthread_exit((void*)ptr);
}

int recorrido(int inicio){
  int cant = 0;
  int fin = inicio + L;

  for (int i = inicio; i < fin; i++){
      if(V[i] == X)
          cant+= 1;
  }
  return cant;
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


