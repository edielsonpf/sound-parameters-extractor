/*
Função: calc_nquadros.c
Calcula o número de quadros de 20ms dos sinal wave

Edielson Prevato Frigieri
Data de atualização: 30/08/07
------------------------------------------------------------
Parâmetros de entrada:
int wave_SIZE => tamanho dos dados de áudio
int hamming_wHAMMING => tamanho da janela de Hamming
 
Parâmetros de Saída:
int nQuadros => retorna o número de quadros de 20ms
*/
int calc_nquadros(int hamming_SIZE, long wave_SIZE)
{
	int inicio,fim,passo,nQuadros;
	
	//=============================
	inicio=0;
	fim=hamming_SIZE;
	passo = hamming_SIZE/2; //passo de 10ms
	nQuadros=0;		
	//=============================

	while(fim<wave_SIZE)
	{	
		//=====================
		inicio+=passo;
		fim+=passo;
		nQuadros++;
		//=====================		
	}	

	return(nQuadros);
}
