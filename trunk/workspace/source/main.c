#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "diretivas.h"
#include "calc_nquadros.h"
#include "WaveCtrl.h"
#include "pre_enfase.h"
#include "hamming.h"
#include "PerfilCalc.h"
#include "fft.h"
#include "MelParameters.h"

#define MAX_STRING_SIZE			50

#ifdef USE_MEL
#define	OUTPUT_FILE_EXTENSION	".mel"
#else
#define	OUTPUT_FILE_EXTENSION	".perfil"
#endif

int main (int argc, char *argv[])
{
	int 	iNunQuadros;	//número de quadros de 20ms do sinal wave
	int 	iHammingSize;	//tamanho da janela de Hamming (nº de amostras em 20ms)
	
	int 	iCounter;		//contadores
	long 	lAudioSize;		//tamanho dos dados wave (número de amostras)
	int 	iSampleFreq; 	//frequência de amostragem do sinal wave
	double* pdAudioData;	//conteúdo dos dados wave modificados para float

	double*	pdPreEnfase;	//dados após pré-ênfase
	double*	pdHamming; 	//janela hamming
	double*	pdWindowedPre; //recebe conteúdo de um quadro de 20ms

	int 	iPointsFFT; 		//tamanho da FFT
	int		iMelSize;
	int 	M;				// M=9 ==> FFT de 512 pontos; M=10 ==> FFT de 1024 pontos

	int 	inicio, fim;	//variáveis auxiliares
	int 	quadro;		//variável auxiliar
	int 	passo; 		//intervalo entre cada janela de 20ms

	double*		pdEnergy;
#ifdef USE_MEL
	double* 	pdMelData;
	double**	pdMelCeps;
#else
	//	double* 	pdPerfil;
	double**	pdPerfilArray;
	//int		iNumPerfis;
#endif

	FILE*	pFileList;
	FILE*	pOutputFile;

	char	pcWaveFileNeme[MAX_STRING_SIZE]; 	//caminho e nome do arquivo que será aberto
	char	pcOutputFileNeme[MAX_STRING_SIZE]; 	//caminho e nome do arquivo que será aberto
	char	pcFileList[MAX_STRING_SIZE];
	char*	pcAux;

	/* testing the number of arguments in the command line */
	if (argc  < 1)
	{
		puts("\nEnter name of file with list of files to extract parameters: "
			 "parameters-extractor file_list.txt");
		exit(1);
	}

	//inicializa caminho e nome do arquivo que será aberto
	strncpy(pcFileList,argv[argc-1],MAX_STRING_SIZE);

	pFileList = fopen(pcFileList,"r");
	if(pFileList)
	{
		while(fscanf(pFileList,"%s",pcWaveFileNeme)!=EOF)
		{
			strncpy(pcOutputFileNeme,pcWaveFileNeme,MAX_STRING_SIZE);
			pcAux =  pcOutputFileNeme;

			//Changing .wav extension to .perfil
			for(iCounter=0;iCounter<MAX_STRING_SIZE;iCounter++)
			{
				if(strncmp(pcAux,".wav",MAX_STRING_SIZE)==0)
				{
					*pcAux = '\0';
					strncat(pcOutputFileNeme,OUTPUT_FILE_EXTENSION,MAX_STRING_SIZE);
					break;
				}
				else
					pcAux++;
			}

			//open file to save parameters
			pOutputFile = fopen(pcOutputFileNeme,"w+");
			if(!pOutputFile)
			{
				printf("\nError opening file %s", pcOutputFileNeme);
				exit(0);
			}
			//abre arquivo wave
			printf("\nReading wav file: %s", pcWaveFileNeme);
			pdAudioData = WaveCtrl_read(pcWaveFileNeme,&lAudioSize,&iSampleFreq);
			printf("\nNumber of samples: %ld",lAudioSize);

			// Definindo valores de variaveis que mudam com a frequencia de amostragem do sinal
			switch(iSampleFreq)
			{
				case 8000:
					M=9;
					iPointsFFT=512;
					iMelSize = 19;
					break;

				case 11025:
					M=9;
					iPointsFFT=512;
					iMelSize = 21;
					break;

				case 16000:
					M=10;
					iPointsFFT=1024;
					iMelSize = 24;
					break;
				case 44100:
					M=10;	//2^10 = 1024
					iPointsFFT=1024;
					iMelSize = 32;
				default:
					M=10;	//2^10 = 1024
					iPointsFFT=1024;
					iMelSize = 24;
					break;
			}

			if(pdAudioData)
			{
				//Aloca memória para dados pre-enfase
				pdPreEnfase = (double *) malloc (sizeof(double) * (lAudioSize));
				if(pdPreEnfase)
				{
					//aplica o pré-ênfase no sinal
					pre_enfase(pdAudioData,pdPreEnfase,lAudioSize);
				}
				else
				{
					printf("\nError allocating memory");
					return	-1;
				}

				//calcula tamanho da janela hamming;
				iHammingSize = iSampleFreq*DATA_WINDOW_SIZE*DATA_WINDOW_SCALE; //nº de amostras em 20ms
				printf("\nJanela de Hamming: %d samples", iHammingSize);

				//Calcula o número de quadros de 20ms do sinal wave
				iNunQuadros = calc_nquadros(iHammingSize,lAudioSize);
				printf("\nNúmero de quadros de %dms tomados em intervalos de %dms: %d\n", DATA_WINDOW_SIZE,DATA_WINDOW_SIZE/2,iNunQuadros);

				//Aloca memória para dados float
				pdHamming = (double*) malloc (sizeof(double) * (iHammingSize));
				if(pdHamming)
				{
					hamming(pdHamming,iHammingSize);
				}
				else
				{
					printf("\nError allocating memory");
					return	-1;
				}

				pdWindowedPre =(double*) malloc (sizeof(double) * (iHammingSize));
				if(!pdWindowedPre)
				{
					printf("\nError allocating memory");
					return	-1;
				}

#ifdef USE_MEL
				/*--------------------------------------------------------------*/
				//Aloca memória para dados double
				pdMelCeps = malloc(sizeof(double*)*iNunQuadros);
				if(!pdMelCeps)
				{
					printf("\nError allocating memory");
					return	-1;
				}

				/*--------------------------------------------------------------*/
				//Aloca memória para dados double
				pdMelData = (double *) malloc (sizeof(double) * (iMelSize));
				if(!pdMelData)
				{
					printf("\nError allocating memory");
					return	-1;
				}
#else
//				pdPerfil = (double*)malloc (sizeof(double) * (NUMBER_ENERGY_CONTOUR));
//				if(!pdPerfil)
//				{
//					printf("\nError allocating memory");
//					return	-1;
//				}
				pdPerfilArray = (double**)malloc (sizeof(double) * (iNunQuadros));
				if(!pdPerfilArray)
				{
					printf("\nError allocating memory");
					return	-1;
				}
#endif
				/*--------------------------------------------------------------*/

				pdEnergy = (double*)malloc (sizeof(double) * (iPointsFFT));
				if(!pdEnergy)
				{
					printf("\nError allocating memory");
					return	-1;
				}

				//=============================
				inicio=0;
				fim=iHammingSize;
				passo = iHammingSize/2; //passo de 10ms
				quadro=0;
				//iNumPerfis=NUMBER_ENERGY_CONTOUR;
				//fwrite(&iNumPerfis,sizeof(int),1,pOutputFile);
				//=============================

				while(fim<lAudioSize)
				{
#ifdef USE_MEL
					pdMelCeps[quadro] = (double*)malloc(sizeof(double)*NUMBER_COEFICIENTES_MEL);
					if(!pdMelCeps[quadro])
					{
						printf("\nError allocating memory");
						return	-1;
					}
#else
					pdPerfilArray[quadro] = (double*)malloc(sizeof(double)*NUMBER_ENERGY_CONTOUR);
					if(!pdPerfilArray[quadro])
					{
						printf("\nError allocating memory");
						return	-1;
					}
#endif
					//Pega um quadro de 20ms
					for(iCounter=inicio;iCounter<fim;iCounter++)
					{
						pdWindowedPre[iCounter-inicio]=pdPreEnfase[iCounter];
//						pdWindowedPre[iCounter-inicio]=pdAudioData[iCounter];
					}

					//Multiplica pela janela de Hamming
					for(iCounter=0;iCounter<iHammingSize;iCounter++)
						pdWindowedPre[iCounter] *= pdHamming[iCounter];

					//Calcula a FFT
					//fft_DATA contém o quadrado do módulo da FFT de cada janela de 20ms com tamanho fft_SIZE/2
					fft(pdWindowedPre, pdEnergy, M, iPointsFFT, iHammingSize);

#ifdef USE_MEL
					//Filtragem da FFT pelo banco de filtros na escala mel e calculo do log da saida dos filtros
					//-------------------------------------------------------
					//mel_DATA contém a saída dos filtros
					MelParameters_melFilters(pdMelData, pdEnergy, iPointsFFT, iMelSize);
					//-------------------------------------------------------

					//Coeficientes Mel Cepstrais
					//-------------------------------------------------------
					//mel_ceps_DATA contém os coeficientes
					MelParameters_melCepstrals(pdMelData, pdMelCeps[quadro], NUMBER_COEFICIENTES_MEL, iMelSize);
					//-------------------------------------------------------
					fwrite(pdMelCeps[quadro],sizeof(double),NUMBER_COEFICIENTES_MEL,pOutputFile);

#else
					//Calcula o Perfil de Energia
					PerfilCalc_extract(pdEnergy,iPointsFFT,pdPerfilArray[quadro],NUMBER_ENERGY_CONTOUR,iSampleFreq);
//					perfil(pdEnergy,iPointsFFT,pdPerfil,NUMBER_ENERGY_CONTOUR,iSampleFreq);

//					fwrite(pdPerfilArray[quadro],sizeof(double),NUMBER_ENERGY_CONTOUR,pOutputFile);
//					fwrite(pdPerfil,sizeof(double),NUMBER_ENERGY_CONTOUR,pOutputFile);
#endif
					//=====================
					inicio+=passo;
					fim+=passo;
					quadro++;
					//=====================
				}
#ifndef USE_MEL
				PerfilCalc_mean(pdPerfilArray, pOutputFile,NUMBER_ENERGY_CONTOUR,quadro,iSampleFreq);
#endif
				free(pdWindowedPre);
				free(pdPreEnfase);
				free(pdHamming);
				free(pdAudioData);
//				free(pdPerfil);
				free(pdEnergy);
				fclose(pOutputFile);
#ifdef USE_MEL
				for(iCounter = 0; iCounter < iNunQuadros; iCounter++) free(pdMelCeps[iCounter]);
				free(pdMelCeps);
#else
				for(iCounter = 0; iCounter < iNunQuadros; iCounter++) free(pdPerfilArray[iCounter]);
				free(pdPerfilArray);
#endif
			}
		}
	}
	fclose(pFileList);
	return 0;
}//fim MAIN

