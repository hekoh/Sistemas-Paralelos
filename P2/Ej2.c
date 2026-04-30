#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<float.h>
#include<pthread.h>

#define MAX_THREADS 100


int T = MAX_THREADS;
float *A, *B, *C;
int N;
int BS;

//Para calcular tiempo
double dwalltime();

void * mult(void * ptr);

int main (int argc, char *argv[]){

	if ((argc != 3) || ((N = atoi(argv[1])) <= 0) || ((T = atoi(argv[2])) <=0 ) || (T > N) || ((N % T) != 0)){
    printf("\nUsar: %s n\n  N: Dimension de la matriz (nxn X nxn)\n T: numero de nucleos\n N debe ser divisible por T\n", argv[0]);
    exit(1);
  }

        int i,j;
        int check=1;
        double timetick;

        int ids[T];
        pthread_attr_t attr;
        pthread_t threads[MAX_THREADS];
        int *status;

	
	BS = (int) (N/T);
	pthread_attr_init(&attr);


	//aloca memoria para las matrices
	A=(float*)malloc(sizeof(float)*N*N);
	B=(float*)malloc(sizeof(float)*N*N);
	C=(float*)malloc(sizeof(float)*N*N);

	// Inicializa la matriz a de forma tal que la celda (i,j) tiene valor i
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			A[i*N+j] = i;

	// Inicializa la matriz b de forma tal que la celda (i,j) tiene valor j
	for(i=0;i<N;i++)
		for(j=0;j<N;j++)
			B[i+j*N] = j;

	//creo los hilos
	timetick = dwalltime();
	for (i = 0; i < T ; i++){
        	ids[i] = i;
	       	pthread_create(&threads[i], &attr, mult, &ids[i]);
 	}
	//espero a que retornen
  	for (i = 0; i < T; i++) {
        	pthread_join(threads[i], (void*) &status);
 	}
		


	printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",N,N, dwalltime() - timetick);

	//Verifica el resultado. Con la inicialización realizada, la celda (i,j) de c debería tener valor i*j*n
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			check=check&&(C[i*N+j] == (float) (i*j*N));
		}
	}   

	if(check)
		printf("Multiplicacion de matrices resultado correcto\n");
	else
		printf("Multiplicacion de matrices resultado erroneo\n");
	

	 free(A);
	 free(B);
	 free(C);
	 return(0);
}



void * mult(void * ptr){
	int i,j,k;
	int * p, id;
	float suma;
	p = (int *) ptr;
        id = *p;
	int inicio = id*BS;
	int fin = inicio + BS;

	for(i = inicio; i < fin; i++){
		for(j = 0; j < N; j++){
			suma = 0;
			for(k = 0; k < N; k++){
				suma+= A[i*N+k] * B[k+j*N];
			}
			C[i*N+j] = suma;
		}
	}
	pthread_exit((void*)ptr);
}



//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}
