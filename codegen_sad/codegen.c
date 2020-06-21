// functions for generating MIPS code given an AST of a C-- program
// from the parser

// TODO: add more includes files here as necessary
#include <stdio.h>
#include <assert.h>
#include "parser.h"
#include "codegen.h"
#include "lexer.h"

typedef struct {
  char* instr;
  char* op1;
  char* op2;
  char* op3;
} Instr;


typedef struct {
	int num_instrs;
	Instr** instrs;
} Codetable;

void add_start(FILE *fp);
void codegen(FILE * fp);
void process_node(ast_node* node);
void add_end(FILE *fp);
Instr* new_command(char *comand, char* op1, char* op2, char* op3);
void add_new_c(Instr* c);
void process_lex_node(ast_node* node, tokenT t);
void process_nont_node(ast_node* node, int t);
Codetable* ct; 

// this function will be called after your parse function
// depending on how you are storing the AST (a global or a return
// value from parse, you may need to add some parameters to this function
void codegen(FILE * fp) {
	ct = malloc(sizeof(Codetable));
	ct -> num_instrs = 0;
	add_start(fp);
	ast_node* node = ast_tree.root;
	ast_node** list = get_childlist(node);
	process_node(node);
	process_lex_node(node, node->symbol->token);
	int n = ct->num_instrs;

	for(int i = 0; i < n; i++) {
		Instr * c = ct->instrs[i];
		fprintf(fp, "\t %s ",c->instr);
		if(c->op1){
			fprintf(fp, "%s ", c->op1);
		}
		if(c->op2){
			fprintf(fp, "%s ", c->op2);
		}
		if(c->op3){
			fprintf(fp, "%s", c->op3);
		}
		fprintf(fp, "%s", "\n");

		printf("%s, %s, %s, %s\n",c->instr, c->op1, c->op2, c->op3);
	}
	add_end(fp);
}

void add_start(FILE *fp) {
	char * str = "\t.text\n\t.globl main\nmain:\n";
	fputs(str, fp);
}

void process_node(ast_node* node) {
	ast_info * t = node->symbol;
	if((t->token >= STARTTOKEN) && (t->token <= ENDTOKEN)) {
		process_lex_node(node, t->token);
	} else if (t->token == NONTERMINAL) {
		process_nont_node(node, t->grammar_symbol);
	}
}

void process_lex_node(ast_node* node, tokenT t) {
	t = WRITELN;
	switch(t) {
		case WRITELN:
		{
			Instr* c = new_command("la", "$a0", "_newline_", NULL);
			add_new_c(c);
			c = new_command("syscall", NULL, NULL, NULL);
			add_new_c(c);
			break; 
		}
		case WRITE:
		{ 
			//write - child 
			Instr* c = new_command("la", "$a0", "_newline_", NULL);
			add_new_c(c);
			c = new_command("syscall", NULL, NULL, NULL);
			add_new_c(c);
			break; 
		}
		default:
			break;
	}
}

void process_nont_node(ast_node* node, int t) {

}

Instr* new_command(char *instr, char* op1, char* op2, char* op3){
	Instr* c;
	c = malloc(sizeof(Instr));
	c->instr = instr;
	c->op1 = op1;
	c->op2 = op2;
	c->op3 = op3;
	return c;
}

void add_new_c(Instr* c) {
	ct -> num_instrs +=1;
	ct -> instrs = realloc(ct -> instrs, sizeof(c)*(ct -> num_instrs));
	ct -> instrs[ct -> num_instrs-1] = c;
}

void add_end(FILE *fp) {
	char * str = "\tli $v0, 10\n\tsyscall\n\n\t.data\n\t_newline_: .asciiz \"\\n\"";
	fputs(str, fp);
}

