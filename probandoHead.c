#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024


void imprimir_normal(char**,int);
char** limpiarEspacio(char**,int);
char** reservarEspacio(int);

int main (int argc, char *argv[]) {
	int i;	// i: contador
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

	//Se guardan los valores hasta que se alcanza el tope
	for(i=0;i<n;i++){
		fgets(entrada,TAM_MAX,stdin);
		strcpy(solucion[i],entrada);
	}

	//Imprimir solución
	imprimir_normal(solucion,n);

	//Se libera espacio
	solucion=limpiarEspacio(solucion,n);
	free (entrada);
}

void imprimir_normal(char** solucion, int n){
	printf("\n ##### La Solución es: ##### \n");
	int i=0;
	for(i=0;i<n;i++){
		printf("%s",solucion[i]);
	}
};

char** limpiarEspacio(char** solucion,int n){
	int i=0;
	for (i=0;i<n;i++) {
		free(solucion[i]);
	}
	return solucion;
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
