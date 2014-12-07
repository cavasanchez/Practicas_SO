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
  if (descriptorFichero==-1) {
    fprintf(stderr,"%s: Error.%s\n",nombreFichero,strerror(errno));
  }
  dup2(descriptorFichero,1);
  close(descriptorFichero);
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
      for(comandoNumero=0; comandoNumero<linea->ncommands; comandoNumero++){
        printf("ENTRA FORK \n");
        pid = fork();
        printf("SALE FORK\n");
        if(pid==0){
          senal(1);
          printf("SOY EL HIJO\n");
          if(comandoNumero == linea->ncommands-1){
            printf("*****ES EL ÚLTIMO\n");
            if (linea->redirect_output != NULL) {
              printf("HAY REDIRECCIÖN\n");
              descriptor=redirecSalida(linea->redirect_output);
            }
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
      }
    }
    printf("$ ");
  }
  return 0;
}
