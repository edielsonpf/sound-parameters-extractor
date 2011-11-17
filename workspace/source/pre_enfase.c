/*
Função: pre_enfase.c
Efetuar o pré-ênfase no sinal wave

Edielson Prevato Frigieri
Data de atualização: 07/08/07
------------------------------------------------------------
Parâmetros de entrada:
float *fDATA => dados wave para pré-ênfase
float *eDATA => dados após pré-ênfase
DWORD eSIZE => tamanho do vetor wave

Parâmetros de Saída:
void
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

void pre_enfase(double *fDATA, double *eDATA, long eSIZE)
{
	int i; //contador
	
	eDATA[0]=fDATA[0];
	for (i=1;i<eSIZE;i++)
	{
		eDATA[i]=fDATA[i]-0.95*fDATA[i-1];
	}
}
