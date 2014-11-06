#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024
/*
void imprimir (char**);
int longitud (char**);
//void limpiarEspacio(char*, char**);
void print(char**, int);
*/

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

	// 5) Leemos entrada y almacenamos en entrada

/*
	i=0;
	while (fgets(entrada,TAM_MAX,stdin) != NULL && i<n-1) {
			strcpy(solucion[i],entrada);
			i++;
	}
	*/
	for(i=0;i<n;i++){
		fgets(entrada,TAM_MAX,stdin);
		strcpy(solucion[i],entrada);
	}

	// 6) Imprimimos la solucion
	//imprimir(solucion);
	imprimir_normal(solucion,n);

	// 7) Liberamos el espacio usado

	for (i=0;i<n+1;i++) {
		free(solucion[i]);
	}
	free(entrada);
	return 0;
}

// imprimir: metodo que imprime la solucion por pantalla

void imprimir_normal(char** solucion, int n){
	printf("***LA SOLUCION ES***\n");
	int i=0;
	for(i=0;i<n;i++){
		printf("%s",solucion[i]);
	}
}
/*
void limpiarEspacio(char* entrada, char** solucion){
	int i=0;
	printf("VA A LIMPIAR %i \n",strlen(solucion));
	for (i=0;i<strlen(solucion);i++) {

		free(solucion[i]);
	}
	free(entrada);
}*/
