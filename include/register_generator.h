/**
 * @file   register_generator.h
 * @brief  M�dulo de gera��o de nomes de registradores
 *
 * Utiliza um contador para gerar um nome sempre diferente. N�o existe limite para o n�mero de registradores gerados.
 * Todos os registradores seguem a conven��o de nome: r + <n�mero>
 */

#ifndef _REGISTER_GENERATOR_H
#define _REGISTER_GENERATOR_H

#include <string.h>

int registerCounter = 0;

//!  Obt�m o n�mero de um registrador novo
int getRegister(){
	registerCounter++;
	return registerCounter - 1;
}

#endif
