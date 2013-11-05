/**
 * @file   iks_ast.h
 * @brief  Definições dos tipos de nós da AST.
 */

#ifndef __IKS_AST_H
#define __IKS_AST_H

#define IKS_AST_PROGRAMA             0 //! raiz da AST
#define IKS_AST_FUNCAO               1 //! funcao
#define IKS_AST_IF_ELSE              2 //! if-else
#define IKS_AST_DO_WHILE             3 //! do-while
#define IKS_AST_WHILE_DO             4 //! while-do
#define IKS_AST_INPUT                5 //! operador de entrada
#define IKS_AST_OUTPUT               6 //! operador de saída
#define IKS_AST_ATRIBUICAO           7 //! operador de atribuição
#define IKS_AST_RETURN               8 //! operador de retorno de função
#define IKS_AST_BLOCO                9 //! bloco de comandos
#define IKS_AST_IDENTIFICADOR       10 //! identificador (variável, vetor, função)
#define IKS_AST_LITERAL             11 //! literal
#define IKS_AST_ARIM_SOMA           12 //! operador de soma aritmética (+)
#define IKS_AST_ARIM_SUBTRACAO      13 //! operador de subtração aritmética (-)
#define IKS_AST_ARIM_MULTIPLICACAO  14 //! operador de multiplicação aritmética (*)
#define IKS_AST_ARIM_DIVISAO        15 //! operador de divisão aritmética (/)
#define IKS_AST_ARIM_INVERSAO       16 //! operador de inversão aritmética (-)
#define IKS_AST_LOGICO_E            17 //! operador de comparação lógica E (&&)
#define IKS_AST_LOGICO_OU           18 //! operador de comparação lógica OU (||)
#define IKS_AST_LOGICO_COMP_DIF     19 //! operador de comparação aritmética (!=)
#define IKS_AST_LOGICO_COMP_IGUAL   20 //! operador de comparação aritmética (==)
#define IKS_AST_LOGICO_COMP_LE      21 //! operador de comparação aritmética (<=)
#define IKS_AST_LOGICO_COMP_GE      22 //! operador de comparação aritmética (>=)
#define IKS_AST_LOGICO_COMP_L       23 //! operador de comparação aritmética (<)
#define IKS_AST_LOGICO_COMP_G       24 //! operador de comparação aritmética (>)
#define IKS_AST_LOGICO_COMP_NEGACAO 25 //! operador de negação lógica (!)
#define IKS_AST_VETOR_INDEXADO      26 //! uso de vetores multidimensionais
#define IKS_AST_CHAMADA_DE_FUNCAO   27 //! chamada de uma funcao

#endif
