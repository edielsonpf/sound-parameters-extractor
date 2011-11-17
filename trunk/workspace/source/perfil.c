/*
 * perfil.c
 *
 *  Created on: 14/11/2011
 *      Author: unifei
 */
#include <stdio.h>
#include "InterpLinear.h"


void perfil(double* pdModFFT, int iNumPoints , double* pdPerfil, int iNumPerfil, int iSampleFreq)
{
	if(!pdModFFT)	return;
	if(!pdPerfil)	return;

	int 	iCounterI;
	int 	iCounterJ;
	double	dTotalEnergy = 0;
	double	dStep;
	double	dLimiar;
	double	dPartialB;
	double	dPartial;
	double	dAux1;
	double	dAux2;

	dTotalEnergy = pdModFFT[0]+pdModFFT[iNumPoints/2];
	for(iCounterI=1; iCounterI < iNumPoints/2; iCounterI++)
	{
		dTotalEnergy += 2*pdModFFT[iCounterI];
	}

	dStep = dTotalEnergy / (double)(iNumPerfil + 1);
	dLimiar = 0.0;
	dPartialB = 0.0;
	dPartial = 2*pdModFFT[0];

	iCounterJ = 1;
	for (iCounterI = 0; iCounterI < iNumPerfil; iCounterI++)
	{
		dLimiar += dStep;
		while (dPartial <= dLimiar)
		{
			dPartialB = dPartial;
			dPartial += 2*pdModFFT[iCounterJ];
			iCounterJ++;
		}
		if(dPartial > dLimiar) // passou do ponto
		{

			dAux1 = ((double)iCounterJ - 2.0)*(double)iSampleFreq/(double)iNumPoints;	// em Hertz
			dAux2 = ((double)iCounterJ - 1.0)*(double)iSampleFreq/(double)iNumPoints; 	// em Hertz
			pdPerfil[iCounterI] = InterpLinear(dAux1,dPartialB,dAux2,dPartial,dLimiar);
		}
		else
		{
			pdPerfil[iCounterI] = ((double)iCounterJ - 1.0)*(double)iSampleFreq/(double)iNumPoints;; // em Hertz
		}
	}
}
