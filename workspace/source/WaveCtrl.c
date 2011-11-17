/*
Função: wave_read.c
Função para abrir arquivo WAVE

Edielson Prevato Frigieri
Data de atualização: 03/08/07
------------------------------------------------------------
Parâmetros de entrada:
char *fNAME => nome do arquivo wave para abrir
DWORD *fSIZE => variável que receberá o valor do tamanho do dado wave e retornará para função main
DWORD *Fs => variável que conterá a frequencia de amostragem;

Parâmetros de Saída:
double * => conteúdo dos dados do arquivo wave.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

typedef struct	tag_WaveFormatex
{
	DWORD wChunckDescriptor;
	DWORD nChunckSize;
	DWORD wWave;
	DWORD wFmtChunck;
	DWORD nSubChunckSize;
	WORD  wFormatTag;
	WORD  nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD  nBlockAlign;
	WORD  wBitsPerSample;
	DWORD wPalavraData;
	DWORD nDataSize;
}ttag_WaveFormatex;

#define 	MAX_16BIT_AMP	32678
#define 	MAX_8BIT_AMP	255
#define 	BIT_SAMPLE_16	16
#define 	BIT_SAMPLE_8	8

double* WaveCtrl_read(char* szFileName, long* plSize, int* piSampleFreq)
{
	FILE*			pFilePointer; //ponteiro do arquivo
	ttag_WaveFormatex 	wWaveHeader; // cabeçalho com todas as informações do arquivo wave
	WORD*			pcBuffer; // variável com o conteúdo dos dados do arquivo wave
	double*			pdBuffer=NULL;
	int				iCounter;
	
    	//Abre o arquivo wave
	pFilePointer = fopen(szFileName,"rb");
	if(pFilePointer!=NULL)
	{	
		printf("\nArquivo aberto com sucesso!\n\n");
		fread(&wWaveHeader,sizeof(wWaveHeader),1,pFilePointer);
  		
  		//Mostra as informações do arquivo wave (número de canais, amostragem, etc...)
		printf("Chunck Descriptor=%x\n",wWaveHeader.wChunckDescriptor);
  		printf("Chunck Size=%d\n",wWaveHeader.nChunckSize);
  		printf("Wave=%x\n",wWaveHeader.wWave);
  		printf("Format Chunck=%x\n",wWaveHeader.wFmtChunck);
  		printf("Sub Chunck Size=%d\n",wWaveHeader.nSubChunckSize);
  		printf("Format Tag=%d\n",wWaveHeader.wFormatTag);
  		printf("Channels=%d\n",wWaveHeader.nChannels);
  		printf("Samples Per Sec=%d\n",wWaveHeader.nSamplesPerSec);
  		printf("Average Bytes Per Sec=%d\n",wWaveHeader.nAvgBytesPerSec);
  		printf("Block Align=%d\n",wWaveHeader.nBlockAlign);
  		printf("Bits Per Sample=%d\n",wWaveHeader.wBitsPerSample);
		printf("Palavra Data=%x\n",wWaveHeader.wPalavraData);
  		printf("Data Size=%d\n",wWaveHeader.nDataSize);
		
		//calcula o número de amostras do sinal wave
  		*plSize=((wWaveHeader.nDataSize*8)/(wWaveHeader.nChannels*wWaveHeader.wBitsPerSample));
		//retorna a frequencia de amostragem
		*piSampleFreq=wWaveHeader.nSamplesPerSec;
				
		//Verifica se o arquivo é um arqivo WAVE
		if(wWaveHeader.wWave == 1163280727)
		{
			printf("\nO arquivo %s é um arquivo wave!\n",szFileName);
						
			//Aloca memória e lê dados do arquivo wave
			pcBuffer = (WORD*) malloc (sizeof(WORD) * (*plSize));
			if(pcBuffer)
			{
				fread(pcBuffer, sizeof(WORD), (*plSize), pFilePointer);

				//Fecha ponteiro do arquivo
				fclose(pFilePointer);

				//Converte dados para double
				pdBuffer = (double*)malloc(sizeof(double)*(*plSize));
				if(pdBuffer)
				{
					for(iCounter=0; iCounter<(*plSize); iCounter++)
					{
						if(wWaveHeader.wBitsPerSample == BIT_SAMPLE_16)
							pdBuffer[iCounter] = ((double)pcBuffer[iCounter]/(double)MAX_16BIT_AMP);
						else if(wWaveHeader.wBitsPerSample == BIT_SAMPLE_8)
							pdBuffer[iCounter] = ((double)pcBuffer[iCounter]/(double)MAX_8BIT_AMP);
					}
				}
				free(pcBuffer);
			}
			else
			{
				printf("\nErro ao alocar memória!\n");
				free(pdBuffer);
				pdBuffer = NULL;
			}
		}
		else
		{
			printf("\nNão é um arquivo WAVE!\n");
			free(pdBuffer);
			pdBuffer = NULL;
		}
	}
  	else
  	{
		printf("Erro: falha ao abrir o arquivo: %s\n",szFileName);
		free(pdBuffer);
		pdBuffer = NULL;
	}
	return pdBuffer;
}

