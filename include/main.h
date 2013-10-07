/**
 * @file   main.h
 * @brief  Definições gerais do compilador IKS.
 */


#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include "comp_graph.h"
#include "comp_tree.h"
#include "iks_ast.h"
#include "gv.h"


//! Constantes para uso como valor de retorno do compilador
#define IKS_SUCCESS					0  //sucesso
#define IKS_ERROR_UNDECLARED		1  //identificador não declarado
#define IKS_ERROR_DECLARED			2  //identificador já foi declarado
#define IKS_ERROR_VARIABLE			3  //identificador deve ser utilizado como variável
#define IKS_ERROR_VECTOR			4  //identificador deve ser utilizado como vetor
#define IKS_ERROR_FUNCTION			5  //identificador deve ser utilizado como função
#define IKS_ERROR_WRONG_TYPE		6  //tipos incompatíveis
#define IKS_ERROR_STRING_TO_X		7  //coerção impossível do tipo string
#define IKS_ERROR_CHAR_TO_X			8  //coerção impossível do tipo char
#define IKS_ERROR_MISSING_ARGS		9  //poucos argumentos
#define IKS_ERROR_EXCESS_ARGS		10 //muitos argumentos
#define IKS_ERROR_WRONG_TYPE_ARGS	11 //tipos dos argumentos são incompatíveis
#define IKS_ERROR_WRONG_PAR_INPUT	12 //parâmetro não é identificador
#define IKS_ERROR_WRONG_PAR_OUTPUT	13 //parâmetro não é literal string ou expressão
#define IKS_ERROR_WRONG_PAR_RETURN	14 //parâmetro não é expressão compatível com tipo de retorno
#define IKS_SYNTAX_ERROR    		15 //erro sintático
#define IKS_MEMORY_ERROR			16 //exaustão da memória

comp_dict_t *tabelaDeSimbolosEscopoGlobal;
comp_tree_t *ast;

#endif
