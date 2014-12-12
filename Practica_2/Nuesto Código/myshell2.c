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

int** crearPipes (int n){ // crearPipes -> reserva el espacio de la lista de pipes y crea los pipes
	int i;
	int num = (n % 2 == 0)? n+1:n;
	int** pipes = (int**) malloc ((num-1)*sizeof(int*));

	for (i=0;i<n;i++){
		pipes[i] = (int*) malloc (2*sizeof(int)); // A cada pipe se le reservan 2 espacios de tamaño int
		pipe(pipes[i]); // Creamos la pipe i
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
	descriptorFichero=creat(nombreFichero, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWOTH|S_IXOTH);
	if (descriptorFichero!=-1){
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
	int descriptor=0;
	int ncomandos;
	int i;


	// INICIALIZACIÓN
	entrada = (char*) malloc (TAM*sizeof(char));

	// CUERPO
	senal(0);		// Desactivamos señales
	printf("$ ");		// Imprimimos prompt

	while (fgets(entrada,TAM,stdin)){
		linea=tokenize(entrada);		// Leemos la entrada
		ncomandos=linea->ncommands;
		if (ncomandos>0){		// Si hay comandos que leer
			printf("Comandos=%i\n",ncomandos);
			if(strcmp(linea->commands[0].argv[0], "cd")==0){
				comandoCD(linea->commands[0].argv[1]);
			}
			else {																										//NO es cd
				pipes=crearPipes(ncomandos);
				for (i=0;i<ncomandos;i++){
					pid = fork();

					if (pid==0){																					// PROCESO HIJO
						senal(1);
						if(i==linea->ncommands-1){													//ultimo comando
							printf("último mandato\n");
							if (linea->redirect_output != NULL) {
								printf("Hay redirección de salida\n");
								descriptor=redirecSalida(linea->redirect_output);
							}
						}
						else{
							close(pipes[i][0]);
							dup2(pipes[i][1],1);
						}

						if(descriptor !=-1){
							printf("SIN ERRORES\n");
							if (i==0){
								printf("primer mandato\n");												// Primer comando
								if (linea->redirect_input != NULL) {
									printf("Hay redirección de entrada\n");
									descriptor=redirecEntrada(linea->redirect_input);
								}
							}
							else {
								close(pipes[i-1][1]);
								dup2(pipes[i-1][0],0);
							}
							pid=execvp(linea->commands[i].argv[0],linea->commands[i].argv);
							if(pid==-1){
								fprintf(stderr, "%s:mandato: No se encuentra el mandato\n", linea->commands[0].argv[0]);
							}
						}
					}
				}
				if (!linea->background){														// Esperar Procesos hijos
					waitpid(pid,NULL,0);
				}
			}
			printf("$ ");
		}
	}
	return 0;
}
