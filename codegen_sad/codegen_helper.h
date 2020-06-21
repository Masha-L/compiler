#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "codegen.h"
#include "symtab.h"


// byte size of the type
#define INT_SIZE 4
#define CHAR_SIZE 1

#define REGISTER_SIZE 4

typedef struct {
	char* instr;
	char *op1; //R_D
	char *op2;  //R_S
	char *op3;  //R_T
} Instr;

typedef struct {
	int num_instr;
	Instr **instr_set;
} CodeTable;

typedef enum {
	S0 = 500, S1, S2, S3, S4, S5, S6, S7
} local_reg;

typedef enum {
	A0 = 508, A1, A2, A3
} param_reg;

typedef enum {
	T0 = 512, T1, T2, T3, T4, T5, T6, T7
} temp_reg;

typedef enum {
	SP = 520, FP, GP, RA, V0, V1
} key_Reg;

FILE *in;
FILE *out;

// $sp's total offset as program executes
extern int sp_offset;

extern void add_instr_to_code_table(Instr *instr);


extern void setup();
extern void jal(char *name);


extern int exec_routine(ast_node *node, int num_params, char* func_name);