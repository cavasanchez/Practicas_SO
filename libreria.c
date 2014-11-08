#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024

void limpiarEspacio(char**,int);
void imprimir_normal(char**,int);
char** reservarEspacio(int);
void imprimirTail (char**,int,int);
int longitud (char**);
int menor (char**,int);
void insertar (char**,char*,int,int);

int head (int n) {
  int i;	// i: contador
  //int n;	// n: numero de strings a guardar
  char** solucion;	// solucion: array de punteros que actuara como buffer.
  char* entrada;	//entrada: string donde se almacena la entrada

  solucion=reservarEspacio(n);
  entrada = (char *) malloc (TAM_MAX*sizeof(char));

  for(i=0;i<n;i++){
    fgets(entrada,TAM_MAX,stdin);
    strcpy(solucion[i],entrada);
  }

  imprimir_normal(solucion,n);
  limpiarEspacio(solucion,n);
  free(entrada);
  return 0;
}

int tail (int n) {
  int i;	// i: contador
  int j;	// j: valor auxiliar
  //int n;	// n: numero de strings a guardar
  char** solucion;	// solucion: array de punteros que actuara como buffer.
  char* entrada;	//entrada: string donde se almacena la entrada

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
  limpiarEspacio(solucion,n);
  free (entrada);
  return 0;
}

int longLines (int n) {
	int tam_menor=0; // tam_menor: longitud del string menor del array
	char** solucion;	// solucion: array de punteros que actuara como buffer.
	char* entrada;	//entrada: string donde se almacena la entrada

	//Se reserva el espacio necesario
	solucion=reservarEspacio(n);
	entrada = (char *) malloc (TAM_MAX*sizeof(char)+1);

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
limpiarEspacio(solucion,n);
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


void imprimirTail (char** string, int j, int tam) {
  printf("\n ##### La Solucion es: ##### \n");
  int i=0;
  for (i=0;i<tam;i++) {
    int aux = (j+i)%tam;
    printf("%s",string[aux]);
  }
};

void imprimir_normal(char** solucion, int n){
printf("\n ##### La Solución es: ##### \n");
  int i=0;
  for(i=0;i<n;i++){
    printf("%s",solucion[i]);
  }
};

void limpiarEspacio(char** solucion,int n){
  int i=0;
  for (i=0;i<n;i++) {
    free(solucion[i]);
  }
}
char** reservarEspacio(int n){
  int i=0;
  char** solucion;
  solucion= (char**) malloc (n*sizeof(char*)+1);
  for (i=0;i<n;i++) {
  solucion[i]=(char*) malloc (TAM_MAX*sizeof(char));
  }
  return solucion;
}
