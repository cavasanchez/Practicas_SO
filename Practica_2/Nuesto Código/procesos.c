#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX 1024

int** crearPipes (int);

int main (void){
	// VARIABLES
	pid_t pid;
	char* buf;
	char* buf2;
	char* entrada;
	char* linea;
	int** pipes;
	int nhijos;
	int i;

	// INICIALIZACIÓN
	buf = (char*) malloc (MAX*sizeof(char));		// buf -> string para leer por entrada estándar
	buf2 = (char*) malloc (MAX*sizeof(char));		// buf2 -> string para leer por entrada estándar
	entrada = (char*) malloc (MAX*sizeof(char));	// entrada -> string para leer por entrada estándar
	linea = (char*) malloc (MAX*sizeof(char));		// linea -> string para leer por entrada estándar
	

	// Leemos nhijos
	printf("Dame un número de hijos: ");
	fgets(entrada,MAX,stdin);

	// Crear lista pipes
	nhijos=atoi(entrada);
	pipes = crearPipes(nhijos);

	printf("Comienza el bucle de %i vueltas:\n",nhijos);
	for(i=0;i<nhijos;i++){											
		printf("   Creamos hijo. Vuelta %i\n",i);
		pid=fork();
		if(pid==0){
			printf("	PROCESO HIJO\n");
			if(i==0){
				printf("		-Primogénito. ");
				printf("Soy el hijo %d y mi padre es %d\n",getpid(),getppid());
				printf("			Dame un mensaje y lo enviaré: ");
				fgets(linea,MAX,stdin);
				dup2(pipes[i][1],1);
				close(pipes[i][0]);							sleep(1);				printf("close(pipes[%i][0])\n",i);
				write(pipes[i][1],linea,MAX);								sleep(1);				printf("write(pipes[%i][1],%s)\n\n",i,linea);
				sleep(2);
				exit(0);

			} else {
				printf("		-Número %i. ",i+1);
				printf("Soy el hijo con pid %d y mi padre es %d\n",getpid(),getppid());
				dup2(pipes[i-1][0],0);
				close(pipes[i-1][1]);						sleep(1);				printf("close(pipes[%i][1])\n",i-1);
				read(pipes[i-1][0],buf,MAX);				sleep(1);				printf("read(pipes[%i][0],%s\n",i-1,buf);
				sleep(1);
				printf("			He recibido el mensaje: %s\n",buf);
				printf("			Me dispongo a enviar\n\n");
				dup2(pipes[i][1],1);
				close(pipes[i][0]);							sleep(1);				printf("close(pipes[%i][0])\n",i);
				write(pipes[i][1],buf,MAX);								sleep(1);				printf("write(pipes[%i][1],%s)\n\n",i,buf);
				sleep(2);
				exit(0);
			}
		} else {
			printf("	PROCESO PADRE\n");
			printf("		-Soy el proceso padre con pid %d\n\n",getpid());
			wait(NULL);
			if (i==nhijos-1){
				close(pipes[i][1]);							sleep(1);				printf("close(pipes[%i][1])\n",i);
				read(pipes[i][0],buf2,MAX);					sleep(1);				printf("read(pipes[%i][0],%s\n",i,buf2);
				sleep(2);
				printf("MENSAJE: %s\n\n",buf2);
			}
		}
	}
	exit(0);
}

int** crearPipes (int n){
	printf("Método crearPipes");
	int i;
	int** pipes = (int**) malloc ((n-1)*sizeof(int*));

	for (i=0;i<n;i++){
		pipes[i] = (int*) malloc (2*sizeof(int));
		pipe(pipes[i]);
	}
	return pipes;
}