#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024

int longitud (char**);
int menor (char**,int);
void insertar (char**,char*,int,int);
char** reservarEspacio(int);
char** limpiarEspacio(char**,int);
void imprimir_normal(char**,int);


int main (int argc, char *argv[]) {
	int i; // i: contador
	int n; // n: numero de strings a guardar
	int tam_menor=0; // tam_menor: longitud del string menor del array
	char** solucion;	// solucion: array de punteros que actuara como buffer.
	char* entrada;	//entrada: string donde se almacena la entrada

// En caso de que no se haya pasado un parametro o este sea cero, default: head(10)
	if (argv[1] == NULL || argv[1] == 0) {
		printf("El parametro introducido no es correcto. Por defecto se ejecutara head(10)\n");
		n=10;
	}
	 else {
		n=atoi(argv[1]);
	}

	//Se reserva el espacio necesario
	solucion=reservarEspacio(n);
	entrada = (char *) malloc (TAM_MAX*sizeof(char)+1);
	i=0;

while(fgets(entrada, TAM_MAX, stdin) != NULL){
	tam_menor=strlen(solucion[n-1]);
	if (strlen(entrada)>tam_menor) {
		int nueva_posicion = menor (solucion,strlen(entrada));
		insertar(solucion,entrada,nueva_posicion,n);
	}
	else{
		printf("La entrada no es suficientemente grande para guardarse \n");
	}
}

// Imprimimos la solucion
imprimir_normal(solucion,n);

//Se libera espacio
solucion=limpiarEspacio(solucion,n);
free (entrada);

return 0;
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
	int i;
	for (i=0;i<longitud(solucion);i++) {
		if (nuevo>strlen(solucion[i])) {break;}
		}
		return i;
}
// insertar: metodo que inserta el nuevo valor en el array
void insertar (char** solucion, char* entrada, int nueva_posicion, int n) {
	int i;
	for (i=longitud(solucion)-1;i>nueva_posicion;i--) {
	strcpy(solucion[i],solucion[i-1]);
	}
	strcpy(solucion[nueva_posicion],entrada);
};

char** reservarEspacio(int n){
	int i=0;
	char** solucion;
	solucion= (char**) malloc (n*sizeof(char*)+1);
	for (i=0;i<n;i++) {
	solucion[i]=(char*) malloc (TAM_MAX*sizeof(char));
	}
	return solucion;
};

char** limpiarEspacio(char** solucion,int n){
	int i=0;
	for (i=0;i<n;i++) {
		free(solucion[i]);
	}
	return solucion;
}

void imprimir_normal(char** solucion, int n){
	printf("\n ##### La Solución es: ##### \n");
	int i=0;
	for(i=0;i<n;i++){
		printf("%s",solucion[i]);
	}
};
