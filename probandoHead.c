#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024

void limpiarEspacio(char*, char**,int);
void imprimir_normal(char**,int);

int main (int argc, char *argv[]) {
	int i;	// i: contador
	int n;	// n: numero de strings a guardar
	char** solucion;	// solucion: array de punteros que actuara como buffer.
	char* entrada;	//entrada: string donde se almacena la entrada

	// 1) En caso de que no se haya pasado un parametro o este sea cero, default: head(10)
	if (argv[1] == NULL || argv[1] == 0) {
		printf("El parametro introducido no es correcto. Por defecto se ejecutara head(10)\n");
		n=10;
	} else {
		n=atoi(argv[1]);
	}

	// 3) Inicialiamos solucion reservando espacio suficiente.
	solucion= (char**) malloc (n*sizeof(char*));
	for (i=0;i<n;i++) {
		solucion[i]=(char*) malloc (TAM_MAX*sizeof(char));
	}

	// 4) entrada: string donde se almacena la entrada
	entrada = (char *) malloc (TAM_MAX*sizeof(char));

	for(i=0;i<n;i++){
		fgets(entrada,TAM_MAX,stdin);
		strcpy(solucion[i],entrada);
	}

	imprimir_normal(solucion,n);
	limpiarEspacio(entrada,solucion,n);
}
// imprimir: metodo que imprime la solucion por pantalla
void imprimir_normal(char** solucion, int n){
printf("\n ##### La Solución es: ##### \n");
	int i=0;
	for(i=0;i<n;i++){
		printf("%s",solucion[i]);
	}
};

void limpiarEspacio(char* entrada, char** solucion,int n){
	int i=0;
	for (i=0;i<n;i++) {
		free(solucion[i]);
	}
	free(entrada);
}
