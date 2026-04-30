#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<sys/time.h>

#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1

double dwalltime();


float getValor(float *matriz,int fila,int columna, int n, int orden);

void setValor(float *matriz, int fila,int columna, int n, int orden, float valor);

//float maxMatriz(float *matriz,int n, int orden);

//float promMatriz(float *matriz, int n, int orden);

//float getValorMinimo(float *matriz, int orden, int n);

int main(int argc,char *argv[]){
	float *a,*b,*c,*bTrans,*r;
	int i,j,k,n;
	double timetick;
	float nro,promA = 0.0, promB = 0.0;
	float maxA = -1, maxB = 1, minA = 100000, minB = 100000;
	
	if ((argc != 2) || ((n = atoi(argv[1])) <= 0)) {
		printf("\nUsar n: Dimension de la matriz (nxn X nxn)\n");
		exit(1);
	}
	a=(float*)malloc(sizeof(float)*n*n);
	b=(float*)malloc(sizeof(float)*n*n);
	bTrans=(float*)malloc(sizeof(float)*n*n);
	c=(float*)malloc(sizeof(float)*n*n);
	r=(float*)malloc(sizeof(float)*n*n);
	
	//inicializo matrices
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			matriz[i*n+j]=i;
			//setValor(a,i,j,n,ORDENXFILAS,i);

	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			matriz[i+n*j];
			//setValor(b,i,j,n,ORDENXCOLUMNAS,j);

	timetick = dwalltime();
	
	//a*b=c
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			setValor(c,i,j,n, 0,0);
			for(k=0;k<n;k++){
				setValor(c,i,j,n,ORDENXFILAS, getValor(c,i,j,n,ORDENXFILAS) + getValor(a,i,k,n,ORDENXFILAS)*getValor(b,k,j,n,ORDENXCOLUMNAS));
			}
		}
	} 
	
	//Tiempo para maximo y promedio de A
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			promA = promA + getValor(a,i,j,n,ORDENXFILAS);
			if(getValor(a,i,j,n,ORDENXFILAS) > maxA){
				maxA = getValor(a,i,j,n,ORDENXFILAS);
			}		
			if(getValor(a,i,j,n,ORDENXFILAS) < minA){
				minA= getValor(a,i,j,n,ORDENXFILAS);
			}
		}
	}
	promA =((float)promA /(n*n));
	//Min,Max,Prom de B,Btrans
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			promB = promB + getValor(b,i,j,n,ORDENXCOLUMNAS);
			setValor(bTrans,j,i,n,ORDENXCOLUMNAS,getValor(b,i,j,n,ORDENXCOLUMNAS));
			if(getValor(b,i,j,n,ORDENXCOLUMNAS) > maxB){
				maxB = getValor(b,i,j,n,ORDENXCOLUMNAS);
			}		
			if(getValor(b,i,j,n,ORDENXCOLUMNAS) < minB){
				minB= getValor(b,i,j,n,ORDENXCOLUMNAS);
			}
		}
	}
	promB=((float)promB/(n*n));
	
	//nro*(c*bTrans)
	nro = ((maxA*maxB)-(minA*minB))/(promA*promB);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			setValor(r,i,j,n, 0,0);
			for(k=0;k<n;k++){
				setValor(r,i,j,n,ORDENXFILAS, getValor(r,i,j,n,ORDENXFILAS) + getValor(c,i,k,n,ORDENXFILAS)*getValor(bTrans,k,j,n,ORDENXCOLUMNAS));
			}
			setValor(r,i,j,n,ORDENXFILAS,getValor(r,i,j,n,ORDENXFILAS) * nro);
		}
	}  
	
	printf("Tiempo recorriendo por matris :%f\t",dwalltime() - timetick);//en teoria te muestra el timepo de todos los calculos
	printf("tamaño de matriz NxN:%d\n", n);
	
	/*
	printf("valor nro A: %f\t", nro);
	printf("valor Maximo A: %f\t", maxA);
	printf("valor Maximo B: %f\t", maxB);
	printf("valor Promedio A: %f\t", promA);
	printf("valor Promedio B: %f\t", promB);
	printf("valor min A: %f\t", minA);
	printf("valor min B: %f\t", minB);
	*/
	free(a);
	free(b);
	free(c);
	free(bTrans);
	free(r);
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
/*
float getValor(float *matriz,int fila,int columna, int n, int orden){
	if(orden==0)
		return(matriz[fila*n+columna]);
	else
		return(matriz[fila+columna*n]);
 }


void setValor(float *matriz,int fila,int columna, int n, int orden,float valor){
	if(orden==0)
		matriz[fila*n+columna]=valor;
	else
		matriz[fila+columna*n]=valor;
}*/