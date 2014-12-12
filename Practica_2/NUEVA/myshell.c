#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#define TAM 1024

#include "parser.h"

void senal (int s){
	if (s!=0){
		signal(SIGINT,SIG_DFL);
		signal(SIGQUIT,SIG_DFL);
	} else {
		signal(SIGINT,SIG_IGN);
		signal(SIGQUIT,SIG_IGN);
	}
}

void comandoCD(char *ruta){
	int a=0;
	char* nombreRuta;
	nombreRuta=(char*) malloc(TAM*sizeof(char));
	if (ruta==NULL){																															//Si no pasas nada, a HOME
		a=chdir(getenv("HOME"));
	} else {																																			//Ir a ruta indicada. NO AUTOCOMPLETA
		a=chdir(ruta);
	}
	if(a==-1){                                    																//Si hay descriptor, lo dice
		printf("descriptor: No such file or directory");
	}else{                                       																	//Si no, muestra la nueva ruta
		printf("%s \n",getcwd(nombreRuta,TAM));
	}
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

int redirecSalida (char* nombreFichero){
	int descriptorFichero;
	descriptorFichero=creat(nombreFichero, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
	if (descriptorFichero!=-1){
		dup2(descriptorFichero,1);
		close(descriptorFichero);
	}
	return(descriptorFichero);
}

void cerrarTodasPipes (int** pipes, int numero){
	int i;
	for(i=0;i<numero-1;i++){
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}

int** crearPipes (int n){ // crearPipes -> reserva el espacio de la lista de pipes y crea los pipes
	int i;
	int num = (n % 2 == 0)? n+1:n;
	int** pipes = (int**) malloc ((num-1)*sizeof(int*));
	for (i=0;i<n-1;i++){
		pipes[i] = (int*) malloc (2*sizeof(int)); // A cada pipe se le reservan 2 espacios de tamaño int
		pipe(pipes[i]); // Creamos la pipe i
	}
	return pipes;
}

//////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////***MAIN***/////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
int main (void) {

	char* entrada;
	tline* linea;
	int i,j,descriptor, ncomandos;
	pid_t pid;
	int** pipes;																																	//Lista de pipes
	entrada = (char*) malloc (TAM*sizeof(char));
	senal(0);																																			//Señales desactivadas

	printf("$ ");
	while (fgets(entrada, TAM, stdin)) {
		descriptor=0;
		linea = tokenize(entrada);
		ncomandos=linea->ncommands;

		if (ncomandos!=0){																													//Se introdude algo
			if(strcmp(linea->commands[0].argv[0],"cd")==0){														//Se ha introducido CD
				comandoCD(linea->commands[0].argv[1]);
			}else{
				pipes=crearPipes(ncomandos);
				for(i=0; i<ncomandos; i++){
					pid = fork();

					if(pid==0){																														//HIJO
						senal(1);
						if(i==0){
							if (linea->redirect_input != NULL) {															//Hay redirección de entrada
								descriptor=redirecEntrada(linea->redirect_input);
							}
						}else{
							close(pipes[i-1][1]);
							dup2(pipes[i-1][0],0);																						//Salida del pipe anterior como entrada estándar.
						}

						if(descriptor!=-1){
							if(i==ncomandos-1){
								if(linea->redirect_output!=NULL){																//Hay redirección de salida
									redirecSalida(linea->redirect_output);
								}
							}else{
								close(pipes[i][0]);
								dup2(pipes[i][1],1);																						//Entrada del pipe como salida estándar.
							}
							for(j=0; j<i; j++){																								//Cerramos todas las tuberías por ambos extremos
								close(pipes[j][0]);
								close(pipes[j][1]);
							}
							pid=execvp(linea->commands[i].argv[0],linea->commands[i].argv);		//Ejecuta el mandato
							if(pid==-1){
								fprintf(stderr, "mandato: No se encuentra el mandato\n");
								return 0;
							}
						}//FIN descriptor!=-1
					}//FIN pid==0
				}//FIN for
				cerrarTodasPipes(pipes,ncomandos);
				if (linea->background) {
					printf("[%d]\n",pid);
				}else{
					waitpid(pid,NULL,0);
					}
			}//FIN no es un "cd"
		}//FIN else has introducido algo
		printf("$ ");
	}//FIN while
	return 0;
}
