#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024

void imprimirTail (char**,int,int);
char** limpiarEspacio(char**,int);
char** reservarEspacio(int);

int main (int argc, char *argv[]) {
	int i;	// i: contador
	int j;	// j: valor auxiliar
	int n;	// n: numero de strings a guardar
	char** solucion;	// solucion: array de punteros que actuara como buffer.
	char* entrada;	//entrada: string donde se almacena la entrada


	// En caso de que no se haya pasado un parametro o este sea cero, default: head(10)
	if (argv[1] == NULL || argv[1] == 0) {
		printf("El parametro introducido no es correcto. Por defecto se ejecutara head(10)\n");
		n=10;
	} else {
		n=atoi(argv[1]);
	}

	//Se reserva el espacio necesario
	solucion=reservarEspacio(n);
	entrada = (char *) malloc (TAM_MAX*sizeof(char));

	//Se guardan los valores hasta que se cierra la entrada
	i=0;
	while (fgets(entrada,TAM_MAX,stdin) != NULL) {
		j=i%n;
		strcpy(solucion[j],entrada);
		i++;
	}

	// Imprimimos la solucion
	imprimirTail(solucion,j+1,n);

	//Se libera espacio
	solucion=limpiarEspacio(solucion,n);
	free (entrada);;
}


void imprimirTail (char** string, int j, int tam) {
	printf("\n ##### La Solucion es: ##### \n");
	int i=0;
	for (i=0;i<tam;i++) {
		int aux = (j+i)%tam;
		printf("%s",string[aux]);
	}
}

char** reservarEspacio(int n){
	int i=0;
	char** solucion;
	solucion= (char**) malloc (n*sizeof(char*));
	for (i=0;i<n;i++) {
	solucion[i]=(char*) malloc (TAM_MAX*sizeof(char));
	}
	return solucion;
}
char** limpiarEspacio(char** solucion,int n){
	int i=0;
	for (i=0;i<n;i++) {
		free(solucion[i]);
	}
	return solucion;
}
