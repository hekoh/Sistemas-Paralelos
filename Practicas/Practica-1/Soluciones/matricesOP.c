#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>

#define ORDENXFILAS 1
#define ORDENXCOLUMNAS 0

//Para calcular tiempo
double dwalltime();



int main(int argc,char*argv[]){
	float *a,*b,*c;
	int i,j,k, n;
	int check=1, print;
	double timetick;

 //controla los argumentos al programa
	if ((argc != 3) || ((n = atoi(argv[1])) <= 0) || ((print = atoi(argv[2])) < 0) || (print > 1)) {
		printf("\nUsar: %s n print\n  n: Dimension de la matriz (nxn X nxn)\n  print: 1 imprime matrices | 0 no imprime matrices (nxn X nxn)\n", argv[0]);
		exit(1);
	}

	//aloca memoria para las matrices
	a=(float*)malloc(sizeof(float)*n*n);
	b=(float*)malloc(sizeof(float)*n*n);
	c=(float*)malloc(sizeof(float)*n*n);

	// Inicializa la matriz a de forma tal que la celda (i,j) tiene valor i
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			a[i*n+j] = i;

	// Inicializa la matriz b de forma tal que la celda (i,j) tiene valor j
	for(i=0;i<n;i++)
		for(j=0;j<n;j++)
			b[j*n+i] = i;


	if (print == 1) {
		// Imprime matriz a en forma lógica
		printf("Matriz A\n");
		for(i=0;i<n;i++) {
			for(j=0;j<n;j++) 
				printf("%f\t",a[i*n+j]);
			printf("\n");
		}
		// Imprime matriz a en forma física (como está en la memoria)
		for(i=0;i<n*n;i++) 
			printf("%f\t",a[i]);
		// Imprime matriz b en forma lógica
		for(i=0;i<n;i++) {
			for(j=0;j<n;j++) 
				printf("%f\t",b[j*n+i]);
			printf("\n");
		}
		// Imprime matriz b en forma física (como está en la memoria)
		for(i=0;i<n*n;i++) 
			printf("%f\t",b[i]);
	}

	//Realiza la multiplicacion
	timetick = dwalltime();

	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			c[i*n+j] = 0;
			for(k=0;k<n;k++){
				c[i*n+j]+= a[i*n+k]*b[j*n+k];
			}
		}
	}   

	printf("Multiplicacion de matrices de %dx%d. Tiempo en segundos %f\n",n,n, dwalltime() - timetick);

	//Verifica el resultado. Con la inicialización realizada, la celda (i,j) de c debería tener valor i*j*n
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			check=check&&(c[i*n+j] == (i*j*n));
		}
	}   

	if(check)
		printf("Multiplicacion de matrices resultado correcto\n");
	else
		printf("Multiplicacion de matrices resultado erroneo\n");
	

	 free(a);
	 free(b);
	 free(c);
	 return(0);
}



//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}
