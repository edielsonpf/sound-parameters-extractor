/*
 * MelParameters.c
 *
 *  Created on: 19/11/2012
 *      Author: unifei
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

double MelParamenters_roundx(double value);

/*
Função: filtros_mel.c
Passa o sinal após a FFT no banco de filtros Mel

Edielson Prevato Frigieri
Data de atualização: 20/08/07
------------------------------------------------------------
Parâmetros de entrada:
double *mel_DATA => dados após os filtros mel que serão retornados para a função chamadora
double *fft_DATA => dados após a FFT
int fft_SIZE => tamanho da FFT
int filtros_SIZE => tamanho do banco de filtros mel

Parâmetros de Saída:
void
*/

void MelParameters_melFilters(double *mel_DATA, double *fft_DATA, int fft_SIZE, int filtros_SIZE)
{
	double* pdCenterFreq; 		//Frequencias Centrais dos Filtros Mel
	double f1,f2,f3; 	//frequências para cálculo da equação da reta
	double aC, bC; 		//coeficientes da equação da reta crescente
	double aD, bD; 		//coeficientes da equação da reta decrescente
	double Hz_DATA;		//converte dado para Hz
	double dfft_SIZE;	//tamanho da fft em formato double
	int i,j;		//contadores


	/*--------------------------------------------------------------*/
	//Aloca memória para dados double
	pdCenterFreq = (double *) malloc (sizeof(double) * (filtros_SIZE+1));
	if(!pdCenterFreq)
	{
		printf("\nError allocating memory");
		return;
	}
	/*--------------------------------------------------------------*/

	//Converte fft_SIZE para double
	/*--------------------------------------------------------------*/
	dfft_SIZE=fft_SIZE;
	/*--------------------------------------------------------------*/

	//Inicializa frequências centrais
	/*--------------------------------------------------------------*/
	pdCenterFreq[0]=100;
	for(i=1;i<filtros_SIZE+1;i++)
	{
		if(i<10)
			pdCenterFreq[i]=pdCenterFreq[i-1]+100;
		else
			pdCenterFreq[i]=MelParamenters_roundx(pdCenterFreq[i-1]*pow(2,0.2));
	}
	/*--------------------------------------------------------------*/

	//Calcula os filtros
	/*--------------------------------------------------------------*/
	for (i=0;i<filtros_SIZE;i++)
	{
		//============================
		//Coeficientes Equaçao da Reta
		if(i==0)
		{
			f1=0;
			f2=pdCenterFreq[i];
			f3=pdCenterFreq[i+1];
		}
	/*	else if(i==(filtros_SIZE-1))
		{
			f1=pdCenterFreq[i-1];
			f2=pdCenterFreq[i];
			f3=5278;
		}	*/
		else
		{
			f1=pdCenterFreq[i-1];
			f2=pdCenterFreq[i];
			f3=pdCenterFreq[i+1];
		}

		aC=1/(f2-f1);
		bC=-f1/(f2-f1);
		aD=1/(f3-f2);
		bD=f3/(f3-f2);
   		//============================

		//Zera mel_DATA
		//----------------------------
		mel_DATA[i]=0.0;
		//----------------------------

		for(j=0;j<(fft_SIZE/2);j++)
		{
			Hz_DATA=j*(5512/((dfft_SIZE/2)-1));

			if(f1<=Hz_DATA && Hz_DATA<=f2)
				mel_DATA[i]=mel_DATA[i]+fft_DATA[j]*(aC*Hz_DATA+bC);
			else if(f2<Hz_DATA && Hz_DATA<=f3)
				mel_DATA[i]=mel_DATA[i]+fft_DATA[j]*(-aD*Hz_DATA+bD);
		}
	}
	/*--------------------------------------------------------------*/
	free(pdCenterFreq);
}


void MelParameters_melCepstrals(double* mel_DATA, double* mel_ceps_DATA, int P, int filtros_SIZE)
{
	int i,m;	//contador
	double aux;	//variável auxiliar para calculo do mel ceps

	//Zera mel_ceps_DATA
	/*--------------------------------------------------------------*/
	for(i=0;i<P;i++)
	{
		mel_ceps_DATA[i]=0.0;
	}
	/*--------------------------------------------------------------*/

	//Calculo do Mel Cepstrais
	/*--------------------------------------------------------------*/
	for(m=0;m<P;m++)
	{
		aux=0.0;

		for(i=0;i<filtros_SIZE;i++)
		{
			aux=aux+log(mel_DATA[i])*cos(((M_PI*m)/(2*filtros_SIZE))*(2*i+1));
		}
		mel_ceps_DATA[m]=aux;
	}
}

double MelParamenters_roundx(double value)
{
    if (value < 0)
        return -(floor(-value + 0.5));
    else
        return   floor( value + 0.5);
}
