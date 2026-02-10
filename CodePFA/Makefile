# Exemple de Makefile envoyé par Nouha (utilisé en Spé)

CC = gcc
CFLAGS = -Wall -Wextra -lm -Werror

all: integration pfa

integration: test_integration.o integration.o
	$(CC) -o test_integration $^ $(CFLAGS)

test_integration.o: test_integration.c integration.h
	$(CC) -o $@ -c test_integration.c

integration.o: integration.c integration.h
	$(CC) -o $@ -c integration.c


pfa: test_pfa.o pfa.o integration.o
	$(CC) -o test_pfa $^ $(CFLAGS)

test_pfa.o: test_pfa.c pfa.h
	$(CC) -o $@ -c test_pfa.c

pfa.o: pfa.c pfa.h integration.h
	$(CC) -o $@ -c pfa.c


# .PHONY: clean

clean:
	$(RM) $(PROG) $(OBJ)

