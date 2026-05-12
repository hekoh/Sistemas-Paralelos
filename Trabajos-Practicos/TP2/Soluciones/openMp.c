#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<sys/time.h>
#include<omp.h>

#define ORDENXFILAS 0
#define ORDENXCOLUMNAS 1
#define BS 64

double dwalltime();

void initvalmat(float *mat, int n, float val, int transpose); 

void matmulblks(float *a, float *b,float*c, int n);

void blkmul(float *ablk, float *bblk, float *cblk, int n);


int main(int argc,char *argv[]){
	float *a,*b,*c,*bTrans,*r;
	int i,j,k,n;
	double timetick;
	float nro,promA = 0.0, promB = 0.0;
	float maxA = -1, maxB = -1, minA = 100000, minB = 100000, valActual;
	
	if ((argc != 2) || ((n = atoi(argv[1])) <= 0) || ((n % BS) != 0)){
		printf("\nError en los parametros. Usage: ./%s N BS (N debe ser multiplo de BS)\n", argv[0]);
		exit(1);
	}
	a=(float*)malloc(sizeof(float)*n*n);
	b=(float*)malloc(sizeof(float)*n*n);
	bTrans=(float*)malloc(sizeof(float)*n*n);
	c=(float*)malloc(sizeof(float)*n*n);
	r=(float*)malloc(sizeof(float)*n*n);

	//inicializo matrices
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			a[i*n+j]=i;  
		}
	}
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			b[i+j*n]=j;
		}
	}
	initvalmat(c,n,0,ORDENXFILAS);//inicalizamos c
	
	timetick = dwalltime();
	//zona critica
	
	#pragma omp parallel shared(a,b,c,r,bTrans) private(i,j)
	{
		#pragma omp for schedule(static) reduction(+:promA) nowait
		//maximo y promedio de A
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				valActual = a[i*n+j];
				promA += valActual;
				if(valActual > maxA){
					maxA =valActual;
				}		
				if(valActual < minA){
					minA= valActual;
				}
			}
		}
		
		//Min,Max,Prom de B,Btrans
		#pragma omp for schedule(static)reduction(+:promB) 
		for(i=0;i<n;i++){
			for(j=0;j<n;j++){
				valActual=b[i+j*n];
				promB += valActual;
				bTrans[i+j*n]=b[i*n+j];
				if(valActual > maxB){
					maxB = valActual;
				}		
				if(valActual < minB){
					minB= valActual;
				}
			}
		}
		matmulblks(a,b,c,n);
		matmulblks(c,bTrans,r,n);
		#pragma omp single
		{ 
			promA =((float)promA /(n*n));
			promB=((float)promB/(n*n));
			nro = ((maxA*maxB)-(minA*minB))/(promA*promB);
		}		
		
		//Multiplicacion manual
		#pragma omp for schedule(static)
		for (i=0;i<n;i++){
			for(j=0;j<n;j++){
				r[i*n+j] *= nro;
			}
		}
	}
	printf("Tiempo recorriendo por matris :%f\t",dwalltime() - timetick);
	printf("tamanio de matriz NxN:%d\n", n);
	
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

void initvalmat(float*mat,int n,float val, int transpose)
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

void matmulblks(float *a, float *b, float *c, int n){
  int i,j,k;
  #pragma omp for schedule(static) private(i,j,k)
  for (i=0;i<n;i+=BS){
    for(j=0;j<n;j+=BS){
      for(k=0;k<n;k+=BS){
        blkmul(&a[i*n+k], &b[j*n+k],&c[i*n+j],n);
      }
    }
  }
}

void blkmul(float*ablk,float*bblk,float*cblk,int n)
{
  int i,j,k;
  //se supone que defino 2 como un nro de hilos? 
  for (i=0; i<BS;i++){
    for (j=0;j<BS;j++){
      for  (k = 0; k < BS; k++){
        cblk[i*n+j]+=ablk[i*n+k] * bblk[j*n+k];
      }
    }
  }
}

