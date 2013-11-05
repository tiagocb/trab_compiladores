/**
 * @file   label_generator.h
 * @brief  M�dulo de gera��o de nomes de r�tulos
 *
 * Utiliza um contador para gerar um nome sempre diferente. N�o existe limite para o n�mero de r�tulos gerados.
 * Todos os r�tulos seguem a conven��o de nome: L + <n�mero>
 */

#ifndef _LABEL_GENERATOR_H
#define _LABEL_GENERATOR_H

#include <string.h>

int labelCounter = 0;

//!  Obt�m o n�mero de um r�tulo novo
int getLabel(){
	labelCounter++;
	return labelCounter - 1;
}

#endif
