#include<stdio.h>
#include<stdlib.h>

#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1

//Para calcular tiempo
//double dwalltime();


//Retorna el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
double getValor(double *matriz,int fila,int columna, int n, int orden);

//Establece el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
void setValor(double *matriz, int fila,int columna, int n, int orden, double valor);

double getValorMinimo(double *matriz, int orden);


int main(int argc,char*argv[]){
	double *a,*b;
	int i,j,k, n;
	int print;

 //controla los argumentos al programa
	if ((argc != 3) || ((n = atoi(argv[1])) <= 0) || ((print = atoi(argv[2])) < 0) || (print > 1)) {
		printf("\nUsar: %s n print\n  n: Dimension de la matriz (nxn X nxn)\n  print: 1 imprime matrices | 0 no imprime matrices (nxn X nxn)\n", argv[0]);
		exit(1);
	}

	//aloca memoria para las matrices
	a=(double*)malloc(sizeof(double)*n*n);
	
	// Inicializa la matriz a de forma tal que la celda (i,j) tiene valor i
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			setValor(a,i,j,n,ORDENXFILAS,i);

	// Inicializa la matriz b de forma tal que la celda (i,j) tiene valor j
	if (print == 1) {
		// Imprime matriz a en forma logica
		printf("Matriz A\n");
		for(i=0;i<n;i++) {
			for(j=0;j<n;j++) 
				printf("%f\t",getValor(a,i,j,n,ORDENXFILAS));
			printf("\n");
		}
		printf("\n");
		// Imprime matriz a en forma fisica (como esta en la memoria)
		for(i=0;i<n*n;i++) 
			printf("%f\t",a[i]);
		printf("\n\n");
		printf("Matriz B\n");
	}  
	 free(a);
	 return(0);
}
//Retorna el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
double getValor(double *matriz,int fila,int columna, int n, int orden){
	if(orden==ORDENXFILAS)
		return(matriz[fila*n+columna]);
	else
		return(matriz[fila+columna*n]);
 }

//Establece el valor de la matriz en la posicion fila y columna segun el orden que este ordenada
void setValor(double *matriz,int fila,int columna, int n, int orden,double valor){
	if(orden==ORDENXFILAS)
		matriz[fila*n+columna]=valor;
	else
		matriz[fila+columna*n]=valor;
}

double getValorMinimo(double *matriz, int orden){
	int i,j;
	double min = 10000, aux;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(aux = getValor(matriz,i,j,n,orden) < min){
				min = aux;
			}
		}
	}
}
