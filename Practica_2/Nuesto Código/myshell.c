#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "parser.h"
#define TAM 1024

int main(void){
  tline* linea;
  char* entrada;
  pid_t pid;
  int i;
  entrada=(char*) malloc(TAM*sizeof(char));
  for(;;){
    printf("$ ");
    fgets(entrada,TAM,stdin);
    linea=tokenize(entrada);
    for(i=0;i<linea->ncommands-1;i++){
      pid = fork();
      printf("*****");                                         //Creando hijos
      if (linea->commands[0].filename==NULL){
        printf("mandato: No se encuentra el mandato \n");
      }
      else{
        //system(linea->commands[0].argv[0]);
      }
    }
  }
}
