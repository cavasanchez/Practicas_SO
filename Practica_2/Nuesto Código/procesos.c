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
	

	// LEEMOS nhijos
	printf("Dame un número de hijos: ");
	fgets(entrada,MAX,stdin);
	nhijos=atoi(entrada);

	// Creamos lista pipes
	pipes = crearPipes(nhijos);

	printf("Comienza el bucle de %i vueltas:\n",nhijos);
	for(i=0;i<nhijos;i++){
		printf("   Creamos hijo. Vuelta %i\n",i);
		// Creamos hijo
		pid=fork();
		if(pid==0){		// PROCESO HIJO
			printf("	PROCESO HIJO\n");
			if(i==0){		// Primogénito
				printf("		-Primogénito. ");
				printf("Soy el hijo %d y mi padre es %d\n",getpid(),getppid());
				printf("			Dame un mensaje y lo enviaré: ");
				fgets(linea,MAX,stdin);			// linea -> mensaje a enviar
				// Escritura
				//dup2(pipes[i][1],1);			// Redireccionamos la salida del proceso
				close(pipes[i][0]);				// Cerramos el pipes[0][0] para escribir en pipes[0][1]
				write(pipes[i][1],linea,MAX);
				exit(0);

			} else {		// Otros hijos
				printf("		-Número %i. ",i+1);
				printf("Soy el hijo con pid %d y mi padre es %d\n",getpid(),getppid());
				// Lectura
				dup2(pipes[i-1][0],0);			// Redireccionamos la entrada del proceso
				close(pipes[i-1][1]);			// Cerramos el pipes[i-1][1] para leer de pipes[i-1][0]
				read(pipes[i-1][0],buf,MAX);

				printf("			He recibido el mensaje: %s\n",buf); sleep(1);
				// Escritura
				//dup2(pipes[i][1],1);			// Redireccionamos la salida del proceso
				close(pipes[i][0]);				// Cerramos el pipes[i][0] para escribir en pipes[i][1]
				write(pipes[i][1],buf,MAX);
				exit(0);
			}
		} else {		// PROCESO PADRE
			wait(NULL);	
			if (i==nhijos-1){			// Si es el último hijo, mostramos el mensaje
				printf("	PROCESO PADRE\n");
				// Lectura
				close(pipes[i][1]);				// Cerramos el pipes[i][0] para escribir en pipes[i][1]
				read(pipes[i][0],buf2,MAX);
				printf("MENSAJE: %s\n\n",buf2);	// Mostramos el mensaje
			}
		}
	}
	exit(0);
}

int** crearPipes (int n){
	int i;
	int** pipes = (int**) malloc ((n-1)*sizeof(int*));	// Existiran nhijos-1 pipes

	for (i=0;i<n;i++){									
		pipes[i] = (int*) malloc (2*sizeof(int));		// A cada pipe se le reservan 2 espacios de tamaño int
		pipe(pipes[i]);									// Creamos la pipe i
	}
	return pipes;
}

