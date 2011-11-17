#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "diretivas.h"
#include "calc_nquadros.h"
#include "WaveCtrl.h"
#include "pre_enfase.h"
#include "hamming.h"
#include "perfil.h"
#include "fft.h"

#define MAX_STRING_SIZE			50
#define	OUTPUT_FILE_EXTENSION	".perfil"

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
	int 	M;				// M=9 ==> FFT de 512 pontos; M=10 ==> FFT de 1024 pontos

	int 	inicio, fim;	//variáveis auxiliares
	int 	quadro;		//variável auxiliar
	int 	passo; 		//intervalo entre cada janela de 20ms

	double*	pdEnergy;
	double* pdPerfil;
	int		iNumPerfis;
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
					break;

				case 11025:
					M=9;
					iPointsFFT=512;
					break;

				case 16000:
					M=10;
					iPointsFFT=1024;
					break;

				default:
					M=10;	//2^10 = 1024
					iPointsFFT=1024;
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

				pdPerfil = (double*)malloc (sizeof(double) * (NUMBER_ENERGY_CONTOUR));
				if(!pdPerfil)
				{
					printf("\nError allocating memory");
					return	-1;
				}

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
				iNumPerfis=NUMBER_ENERGY_CONTOUR;
				fwrite(&iNumPerfis,sizeof(int),1,pOutputFile);
				//=============================

				while(fim<lAudioSize)
				{
					//Pega um quadro de 20ms
					for(iCounter=inicio;iCounter<fim;iCounter++)
						pdWindowedPre[iCounter-inicio]=pdPreEnfase[iCounter];

					//Multiplica pela janela de Hamming
					for(iCounter=0;iCounter<iHammingSize;iCounter++)
						pdWindowedPre[iCounter] *= pdHamming[iCounter];

					//Calcula a FFT
					//fft_DATA contém o quadrado do módulo da FFT de cada janela de 20ms com tamanho fft_SIZE/2
					fft(pdWindowedPre, pdEnergy, M, iPointsFFT, iHammingSize);

					//Calcula o Perfil de Energia
					perfil(pdEnergy,iPointsFFT,pdPerfil,NUMBER_ENERGY_CONTOUR,iSampleFreq);

					fwrite(pdPerfil,sizeof(double),NUMBER_ENERGY_CONTOUR,pOutputFile);

					//=====================
					inicio+=passo;
					fim+=passo;
					quadro++;
					//=====================
				}
				free(pdWindowedPre);
				free(pdPreEnfase);
				free(pdHamming);
				free(pdAudioData);
				fclose(pOutputFile);
			}
		}
	}
	fclose(pFileList);
	return 0;
}//fim MAIN

