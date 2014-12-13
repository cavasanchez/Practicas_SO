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
	int num = (n % 2 == 0)? n+1:n;
	int** pipes = (int**) malloc ((num-1)*sizeof(int*));

	for (i=0;i<n;i++){
		pipes[i] = (int*) malloc (2*sizeof(int));		// A cada pipe se le reservan 2 espacios de tamaño int
		pipe(pipes[i]);									// Creamos la pipe i
	}
	return pipes;
}

void cerrarPipes (int** pipes, int numero){	// cerrarPipes -> cierra todas las pipes de la lista de pipes
	int i;
	for(i=0;i<numero-1;i++){
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

void liberarPipes(int** pipes,int n){	// liberarPipes -> libera el espacio reservado de la lista de pipes
	int i;
	for (i=0;i<n-1;i++){
		free(pipes[i]);
	}
	free(pipes);
}

int redirecEntrada (tline* linea){
	int descriptorFichero;
	if (linea->redirect_input != NULL){
		descriptorFichero=open(linea->redirect_input, O_RDWR);
		if (descriptorFichero>0){
			dup2(descriptorFichero,0);
			close(descriptorFichero);
		}
	}
	return(descriptorFichero);
}

int redirecSalida (tline* linea){
	int descriptorFichero;
	if (linea->redirect_output != NULL){
		descriptorFichero=creat(linea->redirect_output, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
		if (descriptorFichero!=-1){
			dup2(descriptorFichero,1);
			close(descriptorFichero);
		}
	}
	return(descriptorFichero);
}

int redirecError (tline* linea){
	int descriptorFichero;
	if (linea->redirect_error != NULL){
		descriptorFichero=creat(linea->redirect_error, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
		if (descriptorFichero!=-1){
			dup2(descriptorFichero,2);
			close(descriptorFichero);
		}
	}
	return(descriptorFichero);
}

void falloEntrada (int descriptor,char* fichero){
	if (descriptor==-1){
		fprintf(stderr,"%s : ERROR : %s\n",fichero,strerror(errno));
	}
}

void lectura (int* pipe){	// lectura -> leemos del pipe
	close(pipe[1]);
	dup2(pipe[0],0);
}

void escritura (int* pipe){	// escritura -> escribimos en el pipe
	close(pipe[0]);
	dup2(pipe[1],1);
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MAIN ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

int main (void){
	// VARIABLES
	pid_t pid;
	tline* linea;
	char* entrada;
	int** pipes;
	int descriptor = 0;
	int ncomandos;
	int i;
	int j;

	// INICIALIZACIÓN
	entrada = (char*) malloc (TAM*sizeof(char));

	// CUERPO
	senal(0);		// Desactivamos señales
	printf("$ ");		// Imprimimos prompt

	while (fgets(entrada,TAM,stdin)){
		linea=tokenize(entrada);	// Leemos la entrada
		ncomandos=linea->ncommands;
		if (ncomandos>0){			// Si hay comandos que leer
			if (!strcmp(linea->commands[0].argv[0],"cd")){				// Comando cd
				comandoCD(linea->commands[0].argv[1]);
			} else if (!strcmp(linea->commands[0].argv[0],"exit")){		// Comando exit
				break;
			} else {													// Otros comandos
				pipes=crearPipes(ncomandos);
				for (i=0;i<ncomandos;i++){
					pid = fork();
					if (pid<0){				// ERROR
						fprintf(stderr,"ERROR\n");
						exit(1);
					} else if (pid==0){		// PROCESO HIJO
						senal(1);
						if(ncomandos == 1){				// Hijo único
							descriptor = redirecEntrada(linea);
							falloEntrada(descriptor,linea->redirect_input);
							descriptor = redirecSalida(linea);
						} else {						// Varios hijos
							if (i == 0){						// Primogénito
								descriptor = redirecEntrada(linea);
								falloEntrada(descriptor,linea->redirect_input);
								escritura(pipes[i]);
							} else if (i == ncomandos-1){		// Último hijo
								descriptor = redirecSalida(linea);
								lectura(pipes[i-1]);
							} else {					// Otro
								lectura(pipes[i-1]);
								escritura(pipes[i]);
							}
						}//FIN(1 o varios hijos)
						descriptor=redirecError(linea);
						cerrarPipes(pipes,i);
						pid=execvp(linea->commands[i].argv[0],linea->commands[i].argv);
						fprintf(stderr, "%s:mandato: No se encuentra el mandato\n", linea->commands[0].argv[0]);
						exit(0);
					}//FIN (Error,Hijo,Padre)
				}//FIN for
				cerrarPipes(pipes,ncomandos);
				if (linea->background){
					printf("[%d]\n",pid);
					senal(0);
				} else {
					waitpid(pid,NULL,0);
				}
				liberarPipes(pipes,ncomandos);
			}//FIN (cd,exit,otro)
		}//FIN ncomandos>0
		printf("$ ");
	}
}
