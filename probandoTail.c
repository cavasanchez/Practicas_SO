#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024

void imprimir (char**,int,int);

int main (int argc, char *argv[]) {
	int i;	// i: contador
	int j;	// j: valor auxiliar
	int n;	// n: numero de strings a guardar

	// 1) En caso de que no se haya pasado un parametro o este sea cero, default: head(10)

	if (argv[1] == NULL || argv[1] == 0) {
		printf("El parametro introducido no es correcto. Por defecto se ejecutara head(10)\n");
		n=10;
	} else {
		n=atoi(argv[1]);
	}

	// 2) solucion: array de punteros que actuara como buffer.

	char** solucion;	

	// 3) Inicialiamos solucion reservando espacio suficiente.

	solucion= (char**) malloc (n*sizeof(char*));
	for (i=0;i<n;i++) {
		solucion[i]=(char*) malloc (TAM_MAX*sizeof(char));
	}

	// 4) entrada: string donde se almacena la entrada

	char* entrada;
	entrada = (char *) malloc (TAM_MAX*sizeof(char));

	// 5) Leemos entrada y almacenamos en entrada

	i=0;
	while (fgets(entrada,TAM_MAX,stdin) != NULL) {
		j=i%n;
		printf("Metemos el valor %i en la posicion %i de %i \n",i,j,n);
		strcpy(solucion[j],entrada);
		i++;
	}

	// 6) Imprimimos la solucion

	imprimir(solucion,j+1,n);

	// 7) Liberamos el espacio usado

	i=0;
	for (i=0;i<n;i++) {
		free(solucion[i]);
	}
	free(entrada);

	return 0;
}

// imprimir: metodo que imprime la solucion por pantalla

void imprimir (char** string, int j, int tam) {
	printf("\n ##### La Solucion es: ##### \n");
	int i=0;
	for (i=0;i<tam;i++) {
		int aux = (j+i)%tam;
		printf("%s",string[aux]);
	}
}

// se le va la bola con los impares