/**
 * @file   iloc_code.h
 * @brief  Código ILOC.
 *
 * Lista encadeada de operações ILOC
 */

#ifndef _ILOC_CODE_H
#define _ILOC_CODE_H

#include <stdarg.h>

/**
 * @brief Estrutura do nodo da lista de operações ILOC.
 *
 * Contém string da operação ILOC e ponteiro para o próximo nodo.
 */
typedef struct _iloc_code {
	char *operation;					/**< String do comando ILOC. */
	struct _iloc_code *next;	/**< Ponteiro para a próxima operação. */
} iloc_code;

//!  Insere uma operação na lista de operações
int insert(iloc_code **code, char *format, ...);
//!  Concatena uma lista de operações com outra lista
void concatCode(iloc_code **code1, iloc_code **code2);
//!  Imprime o código
int printCode(iloc_code *code);

#endif
