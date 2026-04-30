#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<sys/time.h>


double dwalltime();


double getValor(double *matriz,int fila,int columna, int n, int orden);

void setValor(double *matriz, int fila,int columna, int n, int orden, double valor);

double maxMatriz(double *matriz,int n, int orden);

double promMatriz(double *matriz, int n, int orden);

int main(int argc,char *argv[]){
	double *a,*b,*c;
	int i,j,k, n;
	int check=1, print;
	double timetick,max,prom;

	if ((argc != 2) || ((n = atoi(argv[1])) <= 0)) {
		printf("\nUsar n: Dimension de la matriz (nxn X nxn)\n");
		exit(1);
	}
	
	a=(double*)malloc(sizeof(double)*n*n);
	b=(double*)malloc(sizeof(double)*n*n);
	c=(double*)malloc(sizeof(double)*n*n);
	
	//inicializo matrices
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			setValor(a,i,j,n,0,i);

	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			setValor(b,i,j,n,1,j);

	timetick = dwalltime();

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			setValor(c,i,j,n, 0,0);
			for(k=0;k<n;k++){
				setValor(c,i,j,n,0, getValor(c,i,j,n,0) + getValor(a,i,k,n,0)*getValor(b,k,j,n,1));
			}
		}
	}   

	printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",n,n, dwalltime() - timetick);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			check=check&&(getValor(c,i,j,n,0)==(i*j*n));
		}
	}   
  	if(check)
		printf("Multiplicacion de matrices resultado correcto\n");
	else
		printf("Multiplicacion de matrices resultado erroneo\n");
	
	timetick = dwalltime();
	max = maxMatriz(a,n,0);
	printf("tiempo: %f\t",dwalltime()-timetick);
	printf("valor Maximo: %f\t", max);
	timetick = dwalltime();
        prom = promMatriz(a,n,0);
        printf("tiempo: %f\t",dwalltime()-timetick);
        printf("valor Promedio: %f\t", prom);


	free(a);
	free(b);
	free(c);
	return(0);
}
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

//orden = 0: orden por columnas
//orden = 1: orden por filas
double getValor(double *matriz,int fila,int columna, int n, int orden){
	if(orden==0)
		return(matriz[fila*n+columna]);
	else
		return(matriz[fila+columna*n]);
 }


void setValor(double *matriz,int fila,int columna, int n, int orden,double valor){
	if(orden==0)
		matriz[fila*n+columna]=valor;
	else
		matriz[fila+columna*n]=valor;
}

//calculo el valor maximo de una matriz
double maxMatriz(double *matriz, int n, int orden){
	int i,j;
	double max=0 ,aux=0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(max < (aux = getValor(matriz,i,j,n,orden))){
				max = aux;
			}
		}
	}
	return max;
}

//calculo el promedio de los valores de una matriz
double promMatriz(double *matriz, int n, int orden){
	int i,j;
	double prom;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			prom += getValor(matriz,i,j,n,orden);
		 }
	}
	return ((double)prom / (n*n));
}


