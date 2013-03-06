/*
 * perfil.h
 *
 *  Created on: 14/11/2011
 *      Author: unifei
 */

#ifndef PERFIL_H_
#define PERFIL_H_

void PerfilCalc_extract(double* pdEnergy, int iVecSize , double* pdPerfil, int iNumPerfil, int iSampleFreq);
void PerfilCalc_mean(double** pdPerfil, FILE* pOutputFile, int iNumPerfil, int iNumQuadros, int iSampleFreq);
#endif /* PERFIL_H_ */
