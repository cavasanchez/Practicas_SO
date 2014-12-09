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

// MÉTODOS AUXILIARES

void senal (int s){
	if (s!=0){							//	ACTIVAR (1)
		signal(SIGINT,SIG_DFL);
		signal(SIGQUIT,SIG_DFL);
	} else {							//	DESACTIVAR (0)
		signal(SIGINT,SIG_IGN);
		signal(SIGQUIT,SIG_IGN);
	}
}

void prompt (void){
	printf("$ ");
}

void comandoCD(char *ruta){
	int a=0;
	char* nombreRuta;
	nombreRuta=(char*) malloc(TAM*sizeof(char));
	if (ruta==NULL){								//Si no pasas nada, a HOME
		a=chdir(getenv("HOME"));
	} else {										//Ir a ruta indicada. NO AUTOCOMPLETA
		a=chdir(ruta);
	}
	if(a==-1){                                    //Si hay error, lo dice
		printf("Error: No such file or directory");
	}else{                                        //Si no, muestra la nueva ruta
		printf("%s \n",getcwd(nombreRuta,TAM));
	}
}

int** crearPipes (int n){	// crearPipes -> reserva el espacio de la lista de pipes y crea los pipes
	int i;
	int** pipes = (int**) malloc ((n-1)*sizeof(int*));

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

int redirecSalida (char* nombreFichero){
	int descriptorFichero;
	descriptorFichero=open(nombreFichero, O_RDWR);
	if (descriptorFichero>0){
		dup2(descriptorFichero,1);
		close(descriptorFichero);
	}
	return(descriptorFichero);
}

int redirecEntrada (char* nombreFichero){
	int descriptorFichero;
	descriptorFichero=open(nombreFichero, O_RDWR);
	if (descriptorFichero>0){
		dup2(descriptorFichero,0);
		close(descriptorFichero);
	}
	return(descriptorFichero);
}

/**int redirectError (){

}*/

void lectura (int* pipe,char* buffer){	// lectura -> leemos del pipe
	dup2(pipe[0],0);
	close(pipe[1]);
	read(pipe[0],buffer,TAM);
}

void escritura (int* pipe,char* buffer){	// escritura -> escribimos en el pipe 
	dup2(pipe[1],1);
	close(pipe[0]);
	write(pipe[1],buffer,TAM);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MAIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main (void){
	// VARIABLES
	pid_t pid;
	tline* linea;
	char* entrada;
	int** pipes;
	int descriptor;
	int ncomandos;
	int i;

	// INICIALIZACIÓN
	entrada = (char*) malloc (TAM*sizeof(char));

	// CUERPO
	senal(0);		// Desactivamos señales
	prompt();		// Imprimimos prompt

	while (fgets(entrada,TAM,stdin)){
		linea=tokenize(entrada);		// Leemos la entrada
		ncomandos=linea->ncommands;
		if (ncomandos>0){		// Si hay comandos que leer
			printf("Comandos=%i\n",ncomandos);
			if (!strcmp(linea->commands[0].argv[0],"cd")){				// Comando cd
				comandoCD(linea->commands[0].argv[1]);
			} else if (!strcmp(linea->commands[0].argv[0],"exit")){		// Comando exit
				break;
			} else {
				pipes=crearPipes(ncomandos);
				for (i=0;i<ncomandos;i++){						// Otros comandos
					pid = fork();
					if (pid<0){				// ERROR
						exit(1);
					} else if (pid==0){		// PROCESO HIJO
						senal(1);
						if (i==0){					// Primer comando
							descriptor=redirecSalida(linea->redirect_output);
						} else if(i==ncomandos-1){	// Último comando
							descriptor=redirecEntrada(linea->redirect_input);
						} else {					// Otro comando
							descriptor=redirecEntrada(linea->redirect_input);
							descriptor=redirecSalida(linea->redirect_output);
						}
						pid=execvp(linea->commands[i].argv[0],linea->commands[i].argv);
						fprintf(stderr, "%s:mandato: No se encuentra el mandato\n", linea->commands[0].argv[0]);
					} else {				// PROCESO PADRE
						if (!linea->background){		// Procesos hijos en foreground
							waitpid(pid,NULL,0);
						} else {
							senal(1);					// Procesos hijos en background
						}
					}
				}
				liberarPipes(pipes,ncomandos);
			}
		}
		prompt();
	}
}