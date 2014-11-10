#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libreria.h"


int main (int argc, char* argv[]){
    int N;  //Número de lineas que deseamos que guarde el programa.
    if(argc!=2 && argv[1]!=0){ //Si no se pasan argumentos, N será 10.
      printf("Entrada no váliva. Se usa por defecto 10\n");
        N=10;
    } else { //Para que se ejecute del modo ./test -X
        char *entrada = argv[1]+1;
        N = atoi (entrada);
    }
    printf("\n");
    printf("*****   Probando librería.c   *****\n");
    printf("Probando la funcion head para %d lineas...\n",N);
    (head(N)==0)?printf("\nHEAD FINALIZÓ CORRECTAMENTE.\n"):printf("head NO finalizó correctamente.\n");
    rewind(stdin);
    printf("\nVamos ahora con tail\n");
    (tail(N)==0)?printf("\nTAIL FINALIZÓ CORRECTAMENTE.\n"):printf("tail NO finalizó correctamente.\n");
    rewind(stdin);
    printf("\nY finalmente comprobamos la función longlines\n");
    (longLines(N)==0)?printf("\nLONGLINES FINALIZÓ CORRECTAMENTE.\n"):printf("longLines no finalizó correctamente.\n");
    return 0;
};
