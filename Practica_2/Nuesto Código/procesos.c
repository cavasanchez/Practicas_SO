#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MAX 1024

int** crearPipes (int );
void liberarPipes(int** ,int );
//int procesoHijo (int** ,int );
//void procesoPadre (int** ,int ,int );
void lectura (int* ,char* );
void escritura (int* ,char* );


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
			//exit(procesoHijo(pipes,i));
			printf("	PROCESO HIJO\n");
			if(i==0){		// Primogénito
				printf("		-Primogénito. ");
				printf("Soy el hijo %d y mi padre es %d\n",getpid(),getppid());
				printf("			Dame un mensaje y lo enviaré: ");
				fgets(linea,MAX,stdin);			// linea -> mensaje a enviar
				// escritura en el pipe actual (i==0)
				escritura(pipes[i],linea);
				//dup2(pipes[i][1],1);			// Redireccionamos la salida del proceso
				//close(pipes[i][0]);				// Cerramos el pipes[0][0] para escribir en pipes[0][1]
				//write(pipes[i][1],linea,MAX);
				exit(0);

			} else {		// Otros hijos
				printf("		-Número %i. ",i+1);
				printf("Soy el hijo con pid %d y mi padre es %d\n",getpid(),getppid());
				// Lectura del pipe anterior (i-1)
				lectura(pipes[i-1],buf);
				//dup2(pipes[i-1][0],0);			// Redireccionamos la entrada del proceso
				//close(pipes[i-1][1]);			// Cerramos el pipes[i-1][1] para leer de pipes[i-1][0]
				//read(pipes[i-1][0],buf,MAX);

				printf("			He recibido el mensaje: %s\n",buf); sleep(1);
				// Escritura en el pipe actual (i)
				escritura(pipes[i],buf);
				//dup2(pipes[i][1],1);			// Redireccionamos la salida del proceso
				//close(pipes[i][0]);				// Cerramos el pipes[i][0] para escribir en pipes[i][1]
				//write(pipes[i][1],buf,MAX);
				exit(0);
			}
		} else {		// PROCESO PADRE
			//procesoPadre(pipes,i,nhijos);/
			wait(NULL);	
			if (i==nhijos-1){			// Si es el último hijo, mostramos el mensaje
				printf("	PROCESO PADRE\n");
				// Lectura
				lectura(pipes[i],buf2);
				//dup2(pipes[i][0],0);			// Redireccionamos la entrada del proceso
				//close(pipes[i][1]);				// Cerramos el pipes[i][0] para escribir en pipes[i][1]
				//read(pipes[i][0],buf2,MAX);
				printf("MENSAJE: %s\n\n",buf2);	// Mostramos el mensaje
			}
		}
	}

	liberarPipes(pipes,nhijos);

	exit(0);
}

// MÉTODOS AUXILIARES
int** crearPipes (int n){	// crearPipes -> reserva el espacio de la lista de pipes y crea los pipes
	int i;
	int num = (n % 2 == 0)? n+1:n;
	int** pipes = (int**) malloc ((num-1)*sizeof(int*));

	for (i=0;i<n;i++){									
		pipes[i] = (int*) malloc (2*sizeof(int));		// A cada pipe se le reservan 2 espacios de tamaño int
		pipe(pipes[i]);									// Creamos la pipe i
	}
	return pipes;
}

void liberarPipes(int** pipes,int n) {	// liberarPipes -> libera el espacio reservado de la lista de pipes
	int i;
	for (i=0;i<n-1;i++){
		free(pipes[i]);
	}
	free(pipes);
}

/**
int procesoHijo (int** pipes,int i){	// procesoHijo -> proceso de un hijo (pid==0)
	char* buf;
	printf("	PROCESO HIJO\n");
	if (i==0){		// Primogénito
		printf("		-Primogénito. ");
		printf("Soy el hijo %d y mi padre es %d\n",getpid(),getppid());
		printf("			Dame un mensaje y lo enviaré: ");
		fgets(buf,MAX,stdin);		// buf -> mensaje a enviar
		escritura(pipes[i],buf);	// escritura en el pipe actual (i==0)
		return 0;
	} else {		// Otros hijos
		printf("		-Número %i. ",i+1);
		printf("Soy el hijo con pid %d y mi padre es %d\n",getpid(),getppid());
		lectura(pipes[i-1],buf);	// Lectura del pipe anterior (i-1)
		printf("			He recibido el mensaje: %s\n",buf); sleep(1);
		escritura(pipes[i],buf);	// Escritura en el pipe actual (i)
		return 0;
	}
}

void procesoPadre (int** pipes,int i,int n){	// procesoPadre -> proceso del padre (pid>0)
	char* buf;
	wait(NULL);
	if (i==n-1){			// Si es el último hijo, mostramos el mensaje
		printf("	PROCESO PADRE\n");
		lectura(pipes[i],buf);
		printf("MENSAJE: %s\n\n",buf);	// Mostramos el mensaje
	}
}
*/

void lectura (int* pipe,char* buffer){	// lectura -> leemos del pipe
	dup2(pipe[0],0);
	close(pipe[1]);
	read(pipe[0],buffer,MAX);
}

void escritura (int* pipe,char* buffer){	// escritura -> escribimos en el pipe 
	dup2(pipe[1],1);
	close(pipe[0]);
	write(pipe[1],buffer,MAX);
}