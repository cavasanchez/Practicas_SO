#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "parser.h"
#define TAM 1024

void senal (int s){
  if(s!=0){                                       //ACTIVAR
    signal(SIGINT,SIG_DFL);
    signal(SIGQUIT,SIG_DFL);
  }
  else{                                           //DESACTIVAR
    signal(SIGINT,SIG_IGN);
    signal(SIGQUIT,SIG_IGN);
  }
}

int main(void){
  tline* linea;
  char* entrada;
  pid_t pid;
  int i=0;
  entrada=(char*) malloc(TAM*sizeof(char));

  //DESACTIVAMOS SEÑALES
  senal(0);
  printf("$ ");

  while(fgets(entrada,TAM,stdin)){
    printf("SE HAN METIDO %i MANDATOS \n",linea->ncommands);
    linea=tokenize(entrada);
    for(i=0; i<linea->ncommands; i++){
      printf("ENTRA FOR\n");
      pid = fork();
      printf("CREA UN HIJO\n");
      if(pid==0){
        printf("SOY EL HIJO\n");
        senal(1);
        pid=execvp(linea->commands[0].argv[0],linea->commands[0].argv);
        if (pid==-1){
          printf("mandato: No se encuentra el mandato \n");
          return 0;
        }
      }
    }
    printf("SALE DEL FOR\n");
  }
  return 0;
}
