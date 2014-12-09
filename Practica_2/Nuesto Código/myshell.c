#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
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

int redirecSalida(char* nombreFichero){
  int descriptorFichero;
  descriptorFichero=creat(nombreFichero, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
  if (descriptorFichero!=-1){
    dup2(descriptorFichero,1);
    close(descriptorFichero);
  }
  return(descriptorFichero);
}

int redirecEntrada(char* nombreFichero){
  int descriptorFichero;
  descriptorFichero=open(nombreFichero, O_RDWR);
  //Si hay un error:
  if (descriptorFichero!=-1){
    dup2(descriptorFichero,0);
    close(descriptorFichero);
  }
  return(descriptorFichero);
}

int main(void){
  tline* linea;
  char* entrada;
  pid_t pid;
  int comandoNumero;
  entrada=(char*) malloc(TAM*sizeof(char));
  int descriptor;

  //DESACTIVAMOS SEÑALES
  senal(0);
  printf("$ ");

  while(fgets(entrada,TAM,stdin)){
    linea=tokenize(entrada);
    if (linea->ncommands!=0){
      for(comandoNumero=0; comandoNumero < linea->ncommands; comandoNumero++){
        //printf("ENTRA FORK \n");
        pid = fork();
        //printf("SALE FORK\n");
        if(pid==0){
          senal(1);
          //printf("SOY EL HIJO\n");

          if(comandoNumero == linea->ncommands-1){      //Si es el ultimo comando
            if (linea->redirect_output != NULL) {
              descriptor=redirecSalida(linea->redirect_output);
            }
          }
          if(comandoNumero == linea->ncommands-1){      //Si es el primer comando
            descriptor=redirecEntrada(linea->redirect_input);
          }
          pid=execvp(linea->commands[comandoNumero].argv[0],linea->commands[comandoNumero].argv);
          if (pid==-1){
            fprintf(stderr, "%s:mandato: No se encuentra el mandato\n", linea->commands[0].argv[0]);
            return 0;
          }
        }
      }
      if (!linea->background) {
        waitpid(pid,NULL,0);
      } else {
        senal(1);
      }
    }
    printf("$ ");
  }
  return 0;
}
