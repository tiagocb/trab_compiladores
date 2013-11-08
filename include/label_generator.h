/**
 * @file   label_generator.h
 * @brief  Módulo de geração de nomes de rótulos
 *
 * Utiliza um contador para gerar um nome sempre diferente. Não existe limite para o número de rótulos gerados.
 * Todos os rótulos seguem a convenção de nome: L + <número>
 */

#ifndef _LABEL_GENERATOR_H
#define _LABEL_GENERATOR_H

#include <string.h>

int labelCounter = 0;

//!  Obtém o número de um rótulo novo
int getLabel(){
	labelCounter++;
	return labelCounter - 1;
}

#endif
