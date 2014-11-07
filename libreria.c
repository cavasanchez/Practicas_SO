#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define TAM_MAX 1024

void limpiarEspacio(char**,int);
void imprimir_normal(char**,int);
char** reservarEspacio(int);
void imprimirTail (char**,int,int);

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


void imprimirTail (char** string, int j, int tam) {
  printf("\n ##### La Solucion es: ##### \n");
  int i=0;
  for (i=0;i<tam;i++) {
    int aux = (j+i)%tam;
    printf("%s",string[aux]);
  }
}


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
  solucion= (char**) malloc (n*sizeof(char*));
  for (i=0;i<n;i++) {
  solucion[i]=(char*) malloc (TAM_MAX*sizeof(char));
  }
  return solucion;
}
