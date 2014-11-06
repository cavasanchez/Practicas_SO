#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024

void imprimir (char**);
int longitud (char**);
int menor (char**,int);
void insertar (char**,char*,int);

int main (int argc, char *argv[]) {
	int i;	// i: contador
	int n;	// n: numero de strings a guardar
	int tam_menor;	// tam_menor: longitud del string menor del array

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
	tam_menor=0;
	while(fgets(entrada, TAM_MAX, stdin) != NULL){
		if (strlen(entrada)>tam_menor) {
				if(esta_lleno(solucion,n)==1){
					printf("ARRAY NO LLENO \n");

				}
				else{
					printf("ARRAY YA LLENO \n");
					int nueva_posicion = menor (solucion,strlen(entrada));
					insertar(solucion,entrada,nueva_posicion);
				}
		}
	}
	// 6) Imprimimos la solucion
	imprimir(solucion);

	// 7) Liberamos el espacio usado
	for (i=0;i<n;i++) {
		free(solucion[i]);
	}
	free(entrada);

	return 0;
}

// imprimir: metodo que imprime la solucion por pantalla
void imprimir (char** string) {
	printf("\n ##### La Solucion es: ##### \n");
	int i;
	for (i=0;i<longitud(string);i++) {
		printf("%s",string[i]);
	}
}

// longitud: metodo que devuelve la longitud de una lista de punteros
int longitud (char** string) {
	int i=0;
	while (string[i] != NULL) {
		i++;
	}
	return i;
}

// menor: metodo que devuelve la posicion donde insertar el nuevo
int menor (char** solucion, int nuevo){
	printf("entra a menor \n");
	int i;
	for (i=0;i<longitud(solucion);i++) {
		if (nuevo<strlen(solucion[i])) {break;}
	}
	return i;
}

// insertar: metodo que inserta el nuevo valor en el array
void insertar (char** solucion, char* entrada, int nueva_posicion) {
	printf("Intenta añadir en la posicion %i \n",nueva_posicion);
	int i;
	//if (esta_lleno) {
	for (i=longitud(solucion);i>nueva_posicion;i--) {
		strcpy(solucion[i-1],solucion[i]);
	}
	strcpy(solucion[nueva_posicion],entrada);
}

int esta_lleno(char** solucion,int tamanio_array){
int lleno=0;
int i;
int aux=0;
for(i=0;i<longitud(solucion);i++){
	if(solucion[i]!=NULL){
		aux++;
	}
}
if(aux==tamanio_array){
	lleno=1;
}
return lleno;
}
