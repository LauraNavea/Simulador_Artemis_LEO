CC = gcc
CFLAGS = -Wall -Wextra -std=c11
OBJETOS = main.o artemis.o validaciones.o

simulador: $(OBJETOS)
	$(CC) $(CFLAGS) -o simulador $(OBJETOS) -lm

main.o: main.c artemis.h validaciones.h
	$(CC) $(CFLAGS) -c main.c

artemis.o: artemis.c artemis.h
	$(CC) $(CFLAGS) -c artemis.c

validaciones.o: validaciones.c validaciones.h
	$(CC) $(CFLAGS) -c validaciones.c

clean:
	rm -f $(OBJETOS) simulador
