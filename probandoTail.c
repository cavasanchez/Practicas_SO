#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024

int main(int argc, char *argv[]) {
	int i = 0;
	int j=0;
	int numeroElementos;
	numeroElementos=atoi(argv[1]);
	if(numeroElementos==0) {
	printf("El parámetro introducido no es correcto. Por defecto se ejecuta head(10) \n");
	numeroElementos=10;
	}

	//guarda será un array de punteros que actuará como buffer. Se inicializa para que en cada posición haya espacio suficiente para la entrada

	char** guarda;
//guarda=inicializarArrayPunteros(numeroElementos);

	guarda= (char**) malloc (numeroElementos*sizeof(char*));
	for(i=0;i<numeroElementos;i++){
	guarda[i]=(char*) malloc (TAM_MAX*sizeof(char));
	}

	//cadenaEntrada será donde se almacene la entrada
	char* cadenaEntrada;
	cadenaEntrada = (char *) malloc (TAM_MAX*sizeof(char));

	while(fgets(cadenaEntrada, TAM_MAX, stdin) != NULL){
		printf("ENTRA WHILE");
		int aux=(j%numeroElementos);
		printf("El numero es :%i \n",aux);
		j++;
		strcpy(guarda[(j%numeroElementos)],cadenaEntrada);
}
	printf("FICHERO CERRADO \n");
	printf("*****EL RESULTADO ES:*****\n");
	imprimirArray(guarda,numeroElementos-1);
	printf("\n");
	free(guarda);
	free(cadenaEntrada);
	return 0;
};

void imprimirArray(char** array, int tamanio){
int i=0;
	for(i=0;i<=tamanio;i++){
		printf("%s", array[i]);
	}
};
char** inicializarArrayPunteros(int numero){
int i=0;
char** guarda;
guarda= (char**) malloc (numero*sizeof(char*));
	for(i=0;i<numero;i++){
	guarda[i]=(char*) malloc (TAM_MAX*sizeof(char));
	}
return guarda;
};

//MAKEFILE!!!!!
