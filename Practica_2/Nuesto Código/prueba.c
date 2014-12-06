#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

#include "parser.h"

//prompt() imprime el prompt
void prompt (){
	printf("msh> ");
}

//si sig = 0 desactiva las señales SIGINT y SIGQUIT. Si no las activa
void signals (int sig){
	if(sig==0){
		signal(SIGINT,SIG_IGN);
		signal(SIGQUIT,SIG_IGN);
	}else{
		signal(SIGINT,SIG_DFL);
		signal(SIGQUIT,SIG_DFL);
	}
}

//cd(path) cambia el directorio actual a path. Si path es NULL lo cambia a HOME.
void cd (char* path){
	char cwd[1024];
	int err;

	if(path!=NULL){
		err=chdir(path);
	}else{
		err=chdir(getenv("HOME"));
	}

	if(err==-1){
		fprintf(stderr,"Error: %s\n",strerror(errno));
	}else{

		printf("%s\n",getcwd(cwd,sizeof(cwd)));
	}
}

//redInput(red) redirecciona la entrada estándar a red
int redInput(char* red){
	int file;

	//Abrimos el fichero
	file=open(red, O_RDONLY);
		//Si hay un error:
		if (file==-1) {
			fprintf(stderr,"%s: Error.%s\n",red,strerror(errno));
		//Si no:
		}else{
			dup2(file,0);
			close(file);
		}
	return(file);
}

//redOutput(red) redirecciona la salida estándar a red
int redOutput(char* red){
	int file;

	//Creamos o sobreescribimos el fichero
	file=creat(red, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
		//Si hay un error:
		if (file==-1) {
			fprintf(stderr,"%s: Error.%s\n",red,strerror(errno));
		//Si no:
		}else{
			dup2(file,1);
			close(file);
		}
	return(file);
}

//redErr(red) redirecciona la salida de error a red
int redErr(char* red){
	int file;

	//Creamos o sobreescribimos el fichero
	file=creat(red, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH);
		//Si hay un error:
		if (file==-1) {
			fprintf(stderr,"%s: Error.%s\n",red,strerror(errno));
		//Si no:
		}else{
			dup2(file,2);
			close(file);
		}
	return(file);
}

//pipesList recibe el número de mandatos y devuelve una matriz de 2xn-1 para los pipes. Es hasta n-1 ya que el último mandato no tiene pipe para la salida, sino que su resultado saldrá por salida estándar o la redirección correspondiente.
int** pipesList (int n){

	int i;
	int** pl=(int**) malloc((n-1)*sizeof(int*));

	for(i=0;i<n-1;i++){
		pl[i]=(int*) malloc(2*sizeof(int));
	}
	return pl;
}

//ClosePipes recibe la matriz de pipes creada con pipesList así como el número de mandatos y los cierra.
void closePipes (int** pipes, int n){
	int i;
	for(i=0;i<n-1;i++){
		close(pipes[i][0]);
		close(pipes[i][1]);
	}
}


//////////////////////////////////////////
///              MAIN                 ////
//////////////////////////////////////////

int main (int argc, char* argv[]) {
printf("ENTRA MAIN \n");
	char buf[1024];
	tline * line;
	int i,j,error;
	int pid;
	int** pipes;

	//Desactivamos las señales
	signals(0);

	prompt();

	while (fgets(buf, 1024, stdin)) {
		line = tokenize(buf);
		printf("SE HAN METIDO %i MANDATOS \n",line->ncommands);
				for(i=0; i<line->ncommands; i++){
					printf("ENTRA FOR\n");
				    pid = fork();
						printf("CREA UN HIJO\n");
				    if(pid==0){
							printf("ENTRA AL HIJO");
				      signals(1);
							pid=execvp(line->commands[i].argv[0],line->commands[i].argv);
							if(pid==-1){
								fprintf(stderr, "%s: No se encuentra el mandato\n", line->commands[0].argv[0]);
								return 0;
							}
				    }
				}
					printf("SALE DEL FOR\n");
		//Mostramos el prompt
		printf("PROMP\n");
		prompt();
	}
return 0;
}
