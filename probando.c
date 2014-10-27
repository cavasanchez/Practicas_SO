
#include <ctype.h>
#include <stdio.h>
#define TAM_MAX=1024

int main(int N) {
	if (isdigit(N) == 0) {
		printf("No es váliddo");
		return 1;
	} else {
		char cadena[100];
		int i = 0;
		for (i=0; i <= N; i++) {
			fgets(cadena, 100, stdin);
			printf("%s", cadena);
		}
		return 0;
	}
}
char ** inicializarStrings(int num_lineas){
//char ** c=NULL;
char** c = (char**) malloc (N*sizeof(char*));
int i;
for(int i=0;i<num_lineas;i++){
	c[i] = (char*) malloc (TAM_MAX*sizeof(char));
}