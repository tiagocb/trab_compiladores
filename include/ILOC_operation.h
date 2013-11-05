/**
 * @file   ILOC_operation.h
 * @brief  Operações ILOC.
 *
 * As operações ILOC são identificadas pelo operador, operandos de entrada e operandos de saída.
 * Na estrutura utilizada, existe um campo para identificar o operador e até três operandos.
 */

#ifndef _ILOC_OPERATIONS_H
#define _ILOC_OPERATIONS_H

//! Tipos de operadores ILOC
#define ILOC_NOP			0	//! Não faz nada
#define ILOC_ADD			1 //! Soma registradores
#define ILOC_SUB			2 //! Subtrai registradores
#define ILOC_MULT			3 //! Multiplica registradores
#define ILOC_DIV			4 //! Divide registradores
#define ILOC_ADDI			5 //! Soma valor constante em registrador
#define ILOC_SUBI			6 //! Subtrai valor constante de registrador
#define ILOC_RSUBI		7 //! Subtrai registrador de valor constante
#define ILOC_MULTI		8 //! Multiplica valor constante em registrador
#define ILOC_DIVI			9 //! Divide registrador por valor constante
#define ILOC_RDIVI		10 //! Divide valor constante por registrador
#define ILOC_LSHIFT		11 //! Desloca para a esquerda
#define ILOC_LSHIFTI	12 //! Desloca para a esquerda (valor constante)
#define ILOC_RSHIFT		13 //! Desloca para a direita
#define ILOC_RSHIFTI	14 //! Desloca para a direita (valor constante)
#define ILOC_AND			15 //! AND
#define ILOC_ANDI			16 //! AND (valor constante)
#define ILOC_OR				17 //! OR
#define ILOC_ORI			18 //! OR (valor constante)
#define ILOC_XOR			19 //! XOR
#define ILOC_XORI			20 //! XOR (valor constante)
#define ILOC_LOADI		21 //! Carrega valor constante
#define ILOC_LOAD			22 //! Carrega valor da memória
#define ILOC_LOADAI		23 //! Carrega valor da memória (somado a um valor constante)
#define ILOC_LOADAO		24 //! Carrega valor da memória (somado a um registrador)
#define ILOC_CLOAD		25 //! Carrega caracter da memória
#define ILOC_CLOADAI	26 //! Carrega caracter da memória (somado a um valor constante)
#define ILOC_CLOADAO	27 //! Carrega caracter da memória (somado a um registrador)
#define ILOC_STORE		28 //! Armazena valor na memória
#define ILOC_STOREAI	29 //! Armazena valor na memória (somado a um valor constante)
#define ILOC_STOREAO	30 //! Armazena valor na memória (somado a um registrador)
#define ILOC_CSTORE		31 //! Armazena caracter na memória
#define ILOC_CSTOREAI	32 //! Armazena caracter na memória (somado a um valor constante)
#define ILOC_CSTOREAO	33 //! Armazena caracter na memória (somado a um registrador)
#define ILOC_I2I			34 //! Move valor de registrador para registrador
#define ILOC_C2C			35 //! Move caracter de registrador para registrador
#define ILOC_C2I			36 //! Converte caracter para inteiro
#define ILOC_I2C			37 //! Converte inteiro para caracter
#define ILOC_JUMPI		38 //! Desvio incondicional (valor constante)
#define ILOC_JUMP			39 //! Desvio incondicional
#define ILOC_CBR			40 //! Desvio condicional
#define ILOC_CMPLT		41 //! Operador lógico <
#define ILOC_CMPLE		42 //! Operador lógico <=
#define ILOC_CMPEQ		43 //! Operador lógico ==
#define ILOC_CMPGE		44 //! Operador lógico >=
#define ILOC_CMPGT		45 //! Operador lógico >
#define ILOC_CMPNE		46 //! Operador lógico !=

//! Tipos de operandos ILOC
#define ILOC_OPERAND_REGISTER	0 //! Registrador
#define ILOC_OPERAND_CONSTANT	1 //! Valor constante
#define ILOC_OPERAND_LABEL		2 //! Rótulo

/**
 * @brief Estrutura de operando ILOC.
 *
 * Contém operador e três operandos
 */
typedef struct {
	int type;		/**< Tipo (constante, registrador ou rótulo) */
	int valor;	/**< Valor */
} iloc_operand;

/**
 * @brief Estrutura de operação ILOC.
 *
 * Contém operador e três operandos
 */
typedef struct {
	int op;									/**< Operador */
	iloc_operand operand1;	/**< Operando 1 */
	iloc_operand operand2;	/**< Operando 2 */
	iloc_operand operand3;	/**< Operando 3 */
} iloc_operation;

#endif
