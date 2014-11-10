.SUFFIXES: .o .c
.c.o:
	$(CC) -c $(CFLAGS) $<

CC = gcc
CFLAGS = -g -Wall	#Muestra las advertencias y errores del depurador
ENTRADA = test.c libreria.c libreria.h
SALIDA = test.o libreria.o
BOR = libreria.o

run: $(SALIDA)
	$(CC) $(CFLAGS) -o test $(SALIDA)
	$(RM) $(BOR)


libreria.o: libreria.c libreria.h
test.o: test.c libreria.h
