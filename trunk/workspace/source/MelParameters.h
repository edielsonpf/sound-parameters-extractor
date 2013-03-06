/*
 * MelParameters.h
 *
 *  Created on: 19/11/2012
 *      Author: unifei
 */

#ifndef MELPARAMETERS_H_
#define MELPARAMETERS_H_

void MelParameters_melFilters(double* mel_DATA, double* fft_DATA, int fft_SIZE, int filtros_SIZE);
void MelParameters_melCepstrals(double* mel_DATA, double* mel_ceps_DATA, int P, int filtros_SIZE);

#endif /* MELPARAMETERS_H_ */
