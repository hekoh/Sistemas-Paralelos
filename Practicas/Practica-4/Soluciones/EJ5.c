#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <limits.h>
#include<sys/time.h>

#define COORDINATOR 0

int main(int argc, char* argv[]){
   int * V;
   int min = INT_MAX, max = INT_MIN, aux;
   int rank, N, numProcs, stripSize, check = 1;
   MPI_Status status;
   double tick[2], prom = 0;
   int minF, maxF;
   double promF;

   if ((argc !=2) || (N = atoi(argv[1])) <= 0){
	printf("\nUsar: %s size \n  size: Dimension del vector\n", argv[0]);
	exit(1);
   }


   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&numProcs);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   
   V=(int*)malloc(sizeof(int)*N);


   if (rank == COORDINATOR){
	for(int i = 0; i < N; i++)
	   V[i] = rand() % N;
   }



/*****************************************************************/
   MPI_Barrier(MPI_COMM_WORLD);
   tick[0] = MPI_Wtime();

//   MPI_Bcast(V, N, MPI_INT, COORDINATOR, MPI_COMM_WORLD);
   stripSize = (int) N / numProcs;
   MPI_Scatter(V, stripSize, MPI_INT, V, stripSize, MPI_INT, COORDINATOR, MPI_COMM_WORLD);

   for(int i = rank; i < N; i+= numProcs){
	aux = V[i];
	prom+= aux;	
	if(aux < min)
	   min = aux;
	if(aux > max)
	   max = aux;
   }
  
   
   MPI_Reduce(&min,&minF, 1, MPI_INT, MPI_MIN, COORDINATOR, MPI_COMM_WORLD);
   MPI_Reduce(&max, &maxF, 1, MPI_INT, MPI_MAX, COORDINATOR, MPI_COMM_WORLD);
   MPI_Reduce(&prom, &promF, 1, MPI_DOUBLE, MPI_SUM, COORDINATOR, MPI_COMM_WORLD);
	   
   MPI_Barrier(MPI_COMM_WORLD);

   tick[1] = MPI_Wtime();
   MPI_Finalize();

   if(rank ==  COORDINATOR) {
      promF = promF / N;

      printf("Resultados: \n Maximo: %d\n Minimo: %d \n Promedio: %f \n Tiempo total: %f \n",maxF, minF, promF, tick[1] - tick[0]);
   }

   free(V);
   return(0);
}


