/**
 * @file   register_generator.h
 * @brief  Módulo de geração de nomes de registradores
 *
 * Utiliza um contador para gerar um nome sempre diferente. Não existe limite para o número de registradores gerados.
 * Todos os registradores seguem a convenção de nome: r + <número>
 */

#ifndef _REGISTER_GENERATOR_H
#define _REGISTER_GENERATOR_H

#include <string.h>

int registerCounter = 0;

//!  Obtém o número de um registrador novo
int getRegister(){
	registerCounter++;
	return registerCounter - 1;
}

#endif
