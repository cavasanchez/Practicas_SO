#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int main(int argc, int **argv){
  for(;;){
  int pipe_ida[2];
  int pipe_vuelta[2];
  int pid;
  char* entrada;
  entrada = (char *) malloc (5*sizeof(char));
  char buf[5];
  char mayus[5];
  char final[5];
  pipe(pipe_ida);
  pipe(pipe_vuelta);
  pid = fork();
  int a=0;

  if (pid==0){
    //*********RECIBE**********
    puts("HIJO Recibe\n");
    close(pipe_ida[1]);
    read(pipe_ida[0],buf, 5);

    while(a!=5){
        mayus[a]=toupper(buf[a]);
        a++;
      }

       //******ENVÍO DE VUELTA*******
   puts("HIJO REENVIA");
    close(pipe_vuelta[0]);
    write(pipe_vuelta[1],mayus,5);
   exit(0);
  }
  else{

      puts("Padre Manda\n");
      close(pipe_ida[0]);
      fgets(entrada,5,stdin);
      write(pipe_ida[1], entrada,5);
      wait(NULL);

      //RECIBIENDO
     puts("PADRE RECIBE");
      close(pipe_vuelta[1]);
      read(pipe_vuelta[0],final,5);
      //printf("EL PADRE RECIBE:%s \n", final);
      puts("***RESULTADO***");
      puts(final);

        }
  }
}
