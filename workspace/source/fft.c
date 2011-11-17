/*
Função: fft.c
Calcula a FFT de N pontos de um dados de entrada e retorna o quadrado do módulo da FFT

Edielson Prevato Frigieri
Data de atualização: 20/08/07
------------------------------------------------------------
Parâmetros de entrada:
float *temp_DATA => vetor para calculo da FFT
float *fft_DATA => resultado da FFT
DWORD fft_SIZE => tamanho da FFT
DWORD window_SIZE => tamanho do quadro de 20ms

Parâmetros de Saída:
void
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"


void fft(double *temp_DATA, double *fft_DATA, int M, int fft_SIZE, int window_SIZE)
{
	double Tr, Ti, Ur, Ui, Wr, Wi, b;   // var auxiliares para o calculo da FFT
  	int ip, n1, n2, n3;                 // var aux para o calculo da FFT
  	register int i, j, k;               // var auxiliares para loops
	double *Xr,*Xi;     				// partes real e imaginaria da FFT
  
	/*--------------------------------------------------------------*/
	//Aloca memória para dados float
	Xr = (double*) malloc (sizeof(double) * (fft_SIZE));
	if(!Xr)
	{
		printf("\nError");
		return;
	}
	/*--------------------------------------------------------------*/
	
	/*--------------------------------------------------------------*/
	//Aloca memória para dados float
	Xi = (double *) malloc (sizeof(double) * (fft_SIZE));			
	if(!Xi)
	{
		printf("\nError");
		return;
	}
	/*--------------------------------------------------------------*/
	
	Tr = Ti = Ur = Ui = Wr = Wi = b = 0.0;
  	ip = n1 = n2 = n3 = 0;
	
  	// Calculo da FFT de N pontos do vetor temp_DATA. Usa-se o algoritmo proposto no
  	// Prob 6.4, pagina 333, do livro "Digital Signal Processing" de Oppenheim &
  	// Schaffer. Nomenclatura usada no livro e aqui: LE= n1, LE1= n2, NV2= n3.
  	/*--------------------------------------------------------------*/
	for(k=0; k<fft_SIZE; k++) // inicializacao
  	{
		Xr[k] = 0.0;    
		Xi[k] = 0.0;
  	}
  	for(k=0; k<window_SIZE; k++)
  		Xr[k] = temp_DATA[k];

  	// Embaralhamento da sequencia de entrada
  	n3 = fft_SIZE / 2;
  	j = 0;
  	for(i=0; i<fft_SIZE-1; i++) // comeco do "for"
  	{
    		if(i < j)
    		{
	  		Tr = Xr[j];
      			Xr[j] = Xr[i];
      			Xr[i] = Tr;
   			/* Ti = Xi[j];      // para seq.complexas
      			Xi[j] = Xi[i];
      			Xi[i] = Ti;  */
		}
		k = n3;
		while(k <= j)
		{
			j -= k;
			k /= 2;
		}
		j += k;
  	}     // fim do "for" (e do embaralhamento)
	/*--------------------------------------------------------------*/
	
  	//-------------------------------
  	// Calculo da FFT
  	//-------------------------------
	/*--------------------------------------------------------------*/
	for(k=1; k<=M; k++) // "for 1"
  	{
    		n1 = 1;
		for(i=1; i<=k; i++) // equivale a fazer 2**k
      		n1 *= 2;
		n2 = n1 / 2;
		Ur = 1.0;
    		Ui = 0.0;
		b = M_PI / n2;                  // M_PI=3.1415... (definido em <math.h>)
		Wr = cos(b);
    		Wi = -sin(b);

		for(j=0; j<n2; j++) // "for 2"
		{
	  		for(i=j; i<fft_SIZE; i+=n1) // "for 3"
			{
				ip = i + n2;
			Tr = Xr[ip] * Ur - Xi[ip] * Ui;
				Ti = Xr[ip] * Ui + Xi[ip] * Ur;
			Xr[ip] = Xr[i] - Tr;
				Xi[ip] = Xi[i] - Ti;
			Xr[i] =  Xr[i] + Tr;
				Xi[i]  = Xi[i] + Ti;
			}     // fim "for 3"
	  		Tr = Ur;
	  		Ur = Ur * Wr - Ui * Wi;
      			Ui = Tr * Wi + Ui * Wr;
		}        // fim "for 2"
  	}           // fim "for 1"
  	// Fim do calculo da FFT. A seq. obtida esta na ordem sequencial
	/*--------------------------------------------------------------*/
	

  	// Calculo do quadrado do modulo da FFT (equivale a energia)
	/*--------------------------------------------------------------*/
//	for(k=0; k<fft_SIZE/2; k++)
//		fft_DATA[k] = Xr[k] * Xr[k] + Xi[k] * Xi[k];
	for(k=0; k<fft_SIZE; k++)
		fft_DATA[k] = sqrt(Xr[k] * Xr[k] + Xi[k] * Xi[k]);
  	/*--------------------------------------------------------------*/

  	// Desalocando ponteiros
  	free(Xr);
  	free(Xi);
}       // fim da funcao "fft()"
