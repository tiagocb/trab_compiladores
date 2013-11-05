/**
 * @file   ILOC_code.h
 * @brief  Código ILOC.
 *
 * Lista encadeada de operações ILOC
 * Pode possuir rótulos antes de operações.
 */

#ifndef _ILOC_CODE_H
#define _ILOC_CODE_H

/**
 * @brief Estrutura de operando ILOC.
 *
 * Contém operador e três operandos
 */
typedef struct _iloc_code {
	int label;								/**< Valor do rótulo. */
	iloc_operation operation;	/**< Operação ILOC. */
	struct _iloc_code *next;	/**< Ponteiro para a próxima operação. */
} iloc_code;

#endif
