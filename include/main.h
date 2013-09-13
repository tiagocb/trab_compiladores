/**
 * @file   main.h
 * @brief  Definições gerais do compilador IKS.
 */

#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include "comp_dict.h"
#include "comp_graph.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "iks_ast.h"
#include "gv.h"

/* Constantes a serem utilizadas como valor de retorno no caso de sucesso (IKS_SYNTAX_SUCESS) e erro (IKS_SYNTAX_ERRO) do analisador sintático. */
#define IKS_SYNTAX_SUCESSO 0
#define IKS_SYNTAX_ERRO    1

comp_dict_t tabelaDeSimbolos;
comp_tree_t *ast;

#endif
