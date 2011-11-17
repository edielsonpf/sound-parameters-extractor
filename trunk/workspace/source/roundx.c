/*
Função: roundx.c
Função para arredondar valores

Edielson Prevato Frigieri
Data de atualização: 15/08/07
------------------------------------------------------------
Parâmetros de entrada:
float => valor para arredondar;

Parâmetros de Saída:
float => valor arredondado;

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"

double roundx(double value)
{
    if (value < 0)
        return -(floor(-value + 0.5));
    else
        return   floor( value + 0.5);
}
