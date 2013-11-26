/**
 * @file   iloc_code.h
 * @brief  C�digo ILOC.
 *
 * Lista encadeada de opera��es ILOC
 */

#ifndef _ILOC_CODE_H
#define _ILOC_CODE_H

#include <stdarg.h>

/**
 * @brief Estrutura do nodo da lista de opera��es ILOC.
 *
 * Cont�m string da opera��o ILOC e ponteiro para o pr�ximo nodo.
 */
typedef struct _iloc_code {
	char *operation;					/**< String do comando ILOC. */
	struct _iloc_code *next;	/**< Ponteiro para a pr�xima opera��o. */
} iloc_code;

//!  Insere uma opera��o na lista de opera��es
int insert(iloc_code **code, char *format, ...);
//!  Concatena uma lista de opera��es com outra lista
void concatCode(iloc_code **code1, iloc_code **code2);
//!  Imprime o c�digo
int printCode(iloc_code *code);

#endif
