#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024

int main(int argc,char *argv[]){
	int i=0;
	int j=0;
	int numeroElementos;
	numeroElementos=atoi(argv[1])+1;
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

	//metemos en guarda hasta que se supere el numeroElementos, en tal caso, eliminaremos
	while(fgets(cadenaEntrada, TAM_MAX, stdin) != NULL){
		strcpy(guarda[j],cadenaEntrada);
		j++;
		if(j>=numeroElementos) {
			free(guarda[eliminarMenor(&guarda,j)]);
			j--;
		}
	}

	imprimirArrayFinal(guarda,j,numeroElementos);
	printf("\n");

	//liberar espacio usado
	for(i=0;i<TAM_MAX;i++){
		free(guarda[i]);
	}

	free(cadenaEntrada);
	return 0;
};

int eliminarMenor (char** *guarda, int tam){
	int min=0;	//min = tamaño del string menor
	int n=0;	//n = posicion del string menor en el array
	int i=0;
	for (i=0;i<tam;i++){	//recorremos el string
		int aux=strlen(*guarda[i]);		// aux = tamaño actual string
		if(aux<min) {
			n=i;
			min=aux;
		}
	}
	return n;
};

void imprimirArrayFinal(char** array, int j, int numeroElementos){
	int i=0;
	for(i=j-numeroElementos;i<=j;i++){
		printf("%s", array[i]);
	}
};