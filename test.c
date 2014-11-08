#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libreria.h"


int main (int argc, char* argv[]){
    int N;  //Número de lineas que deseamos que guarde el programa.

    if(argc!=2){ //Si no se pasan argumentos, N será 10.
      printf("Entrada no váliva. Se usa por defecto 10\n");
        N=10;
    } else { //Para que se ejecute del modo ./test -X
        char *entrada = argv[1]+1;
        N = atoi (entrada);
    }

    printf("\n");
    printf("*****   Probando librería.c   *****\n");
    printf("Probando la funcion head para %d lineas...\n",N);

    (head(N)==0)?printf("\nLa función head() ha funcionado correctamente.\n"):printf("¡ATENCIÓN! La función no terminó como debía.\n");
    rewind(stdin);

    printf("Vamos ahora con tail\n");
    (tail(N)==0)?printf("\nLa función tail() ha funcionado correctamente.\n"):printf("¡ATENCIÓN! La función no terminó como debía.\n");
    rewind(stdin);

    printf("Y finalmente comprobamos la función longlines\n");
    (longLines(N)==0)?printf("\nLa función tail() ha funcionado correctamente.\n"):printf("¡ATENCIÓN! La función no terminó como debía.\n");
    rewind(stdin);
    printf("\nFin de la ejecucion.\n\n");

    return 0;
};
