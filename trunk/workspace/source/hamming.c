/*
Função: hamming.c
Gera uma janela de hamming com o tamanho desejado

Edielson Prevato Frigieri
Data de atualização: 06/08/07
------------------------------------------------------------
Parâmetros de entrada:
float *wHAMMING => vetor para guardar os valores da janela de Hamming;
DWORD N => número de amostras em 20ms;
 
Parâmetros de Saída:
void
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"

void hamming(double* pdHamming, long N)
{
	long i;

	for(i=0;i<N;i++)
	{
		pdHamming[i] = 0.54 - 0.46*cos((2*M_PI*i)/(N-1));
	}	
}
