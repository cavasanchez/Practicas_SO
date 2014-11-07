#######################################################
####                                               ####
####                  MAKEFILE                     ####
####      by J. Alcalde & J.I. Escribano           ####
####                                               ####
#######################################################

.SUFFIXES: .o .c
.c.o:
	$(CC) -c $(CFLAGS) $<


# Macros

CC = gcc
CFLAGS = -g -Wall	#Muestra las advertencias y errores del depurador
SRC = test.c libreria.c libreria.h
OBJ = test.o libreria.o


# Reglas explícitas

all: $(OBJ)
	$(CC) $(CFLAGS) -o test $(OBJ)

clean:
	$(RM) $(OBJ) test

# Reglas implícitas

libreria.o: libreria.c libreria.h
test.o: test.c libreria.h

