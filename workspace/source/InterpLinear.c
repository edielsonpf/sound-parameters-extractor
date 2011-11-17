/*
 * InterpLinear.c
 *
 *  Created on: 14/11/2011
 *      Author: unifei
 */

double InterpLinear(double x1,double y1, double x2, double y2, double y)
{
	// x1 componente x do ponto 1
	// y1 componente y do ponto 1
	// x2 componente x do ponto 2
	// y2 componente y do ponto 2
	// y  componente y do ponto intermediario


	double a; 				// coeficiente angular da reta que passa pelos pontos 1 e 2
	double b; 				// termo independente da reta que passa pelos pontos 1 e 2
	double aux_x, aux_y; 	// variaveis auxiliares
	double x; 				// valor de retorno da funcao

	aux_x = x1 - x2;
	aux_y = y1 - y2;

	a = aux_y / aux_x;
	b = y1-a*x1;

	x = (y - b)/a;

	return (x);
}
