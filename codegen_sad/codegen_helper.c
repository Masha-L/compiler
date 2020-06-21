/*
	Helper functions to manage 
	memory for codegen, including registers in use 
	and variables' placement

	@author Masha Lifshits
	@data Sunday, 21st June 2020
*/

#include <stdio.h>
#include "parser.h"
#include "lexer.h"
#include "symtab.h"

CodeTable *code_table;

// Keeps track of the temp registers being used/stored
static int is_treg_used[8] = {0, 0, 0, 0, 0, 0, 0, 0};
static int is_treg_stored[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Keeps track of the local registers being used/stored
static int is_lreg_used[8] = {0, 0, 0, 0, 0, 0, 0, 0};
static int is_lreg_stored[8] = {0, 0, 0, 0, 0, 0, 0, 0};

// Helpers for the memory management
static int calc_global_offset(int type);
static int calc_local_offset(int type);
static int calc_array_memory(int type, int size);
static int calc_index_address(ast_node *node, sym_tab_entry *info, int scope);

// function call
int exec_routine(ast_node *node, int num_params, char* func_name) {
	temp_reg reg_id = get_empty_reg();

	setup();

	// process params
	process_params(node, num_params);

	// save address and jump to the subroutine 
	jal(func_name);

	generate_function_postcall();
	move_registers(returnValue, V0); 
	//Return the return value
	return returnValue;
}


/*
	Called before a function call. Stores temporary registers and 
	parameter registers, so the called function can use them.
*/

void setup() {
	// requests 8 temp and 4 arg registers, 4 bytes each
	int requested_space = (8+4)*REGISTER_SIZE;

	// upd stack pointer location
	add_instr(SP, SP, -requested_space);
	sp_offset += requested_space;

	//I use numbers here because I felt it would be more readable
	store_word_instr(T0, 44, SP);
	store_word_instr(T1, 40, SP);
	store_word_instr(T2, 36, SP);
	store_word_instr(T3, 32, SP);
	store_word_instr(T4, 28, SP);
	store_word_instr(T5, 24, SP);
	store_word_instr(T6, 20, SP);
	store_word_instr(T7, 16, SP);

	store_word_instr(A0, 12, SP);
	store_word_instr(A1, 8, SP);
	store_word_instr(A2, 4, SP);
	store_word_instr(A3, 0, SP);
}


/*
	Updates the pointers in a safe from overflow way
	addiu dest_reg, src_reg, im_instr
*/
 void add_immed_instr(int dest_reg, int src_reg, int im_instr) {

 	//Get length of 3immediate value
	int immed_size = snprintf(NULL, 0, "%d", immed);

	// extra byte for the end of the string symbol
	char *imed_temp = malloc(sizeof(immed_size)+1);

	// save the instruction
	sprintf(imed_temp, "%d", im_instr);

	Inst* addiu = setup_3op_instr(strdup("addiu"), 
		getRegStr(dest_reg), getRegStr(src_reg), imed_temp);

	add_instr_to_code_table(addiu);
}

 Instr *setup_3op_instr(char *command, char *dest_reg, 
 	char *src_reg1, char *src_reg2) {
 	char *instr = init_instr();
 	instr->command = command;

	instr->op1 = dest_reg;
	instr->op2 = src_reg1;
	instr->op3 = src_reg2;
	return instr;
}

/*
	Makes space for an Instruction structure in the heap, initalizes
	all fields to NULL. We use this function everytime we want to make
	an instruction (to later add in the code table)!
*/
Instr *init_instr() {
	Instr *instr = malloc(sizeof(Instr));
	instr->command=NULL, instr->op1=NULL, instr->op2=NULL,
	instr->op3=NULL;
	return instr;

//Adds an instruction to codetable
void add_instr_to_code_table(Instr *instr) {
	code_table->num_instr++;
	
	//Re-allocates memory for instruction set of the code table
	code_table->instr_set = realloc(code_table->instr_set, 
		sizeof(Instr)*code_table->num_instr);

	code_table->instr_set[code_table->num_instr-1] = instr;
}

