#Makefile projeto Reconhecimento de Fala
#Edielson Prevato Frigieri
#Atualização: 30/07/07

CC=gcc
CFLAGS=-c -Wall
LDFLAGS= -lm

SOURCES=main.c calc_nquadros.c fft.c hamming.c MelParameters.c InterpLinear.c PerfilCalc.c pre_enfase.c WaveCtrl.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=../Debug/parameters-extractor

all: $(SOURCES) $(EXECUTABLE)
	rm -rf *.o
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm -rf *.o
