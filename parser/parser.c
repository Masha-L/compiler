/*
 * parser.c: A Recursive Descent Parser for C--
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "parser.h"
#include "lexer.h"
#include "ast.h"

// TODO: you may completely wipe out or change the contents of this file; it
//       is just an example of how to get started on the structure of the 
//       parser program.

static void program(FILE *fd, ast_node *parent);
static void parser_error(char *err_string);
static void match(tokenT token, FILE* fd);
static void program_1(FILE *fd, ast_node *parent);
static void program_2(FILE *fd, ast_node *parent);
static void fdl1(FILE *fd, ast_node *parent);
static void fdl(FILE *fd, ast_node *parent);
static void pdl(FILE *fd, ast_node *parent);
static void pdl1(FILE *fd, ast_node *parent);
static void pdl2(FILE *fd, ast_node *parent);
static void block(FILE *fd, ast_node *parent);
static void vdl(FILE *fd, ast_node *parent);
static void vdl1(FILE *fd, ast_node *parent);
static void stmt_list(FILE *fd, ast_node *parent);
static void stmt(FILE *fd, ast_node *parent);
static void stmt_list1(FILE *fd, ast_node *parent);
static void expr(FILE *fd, ast_node *parent);
static void expr_list_p(FILE *fd, ast_node *parent);
static void expr_list(FILE *fd, ast_node *parent);
static void e1(FILE *fd, ast_node *parent);
static void e2(FILE *fd, ast_node *parent);
static void e3(FILE *fd, ast_node *parent);
static void e4(FILE *fd, ast_node *parent);
static void e5(FILE *fd, ast_node *parent);
static void e6(FILE *fd, ast_node *parent);
static void e7(FILE *fd, ast_node *parent);
static void e8(FILE *fd, ast_node *parent);
static void e0_p(FILE *fd, ast_node *parent);
static void e1_p(FILE *fd, ast_node *parent);
static void e2_p(FILE *fd, ast_node *parent);
static void e3_p(FILE *fd, ast_node *parent);
static void e4_p(FILE *fd, ast_node *parent);
static void e5_p(FILE *fd, ast_node *parent);
static void e6_p(FILE *fd, ast_node *parent);
static void e8_p(FILE *fd, ast_node *parent);


tokenT lookahead;  // stores next token returned by lexer
                // you may need to change its type to match your implementation
                
ast ast_tree;   // the abstract syntax tree

/**************************************************************************/
/*
 *  Main parser routine: parses the C-- program in input file pt'ed to by fd, 
 *                       calls the function corresponding to the start state,
 *                       checks to see that the last token is DONE,
 *                       prints a success msg if there were no parsing errors
 *  param fd: file pointer for input
 */
void parse(FILE *fd)  {
  // tokenT token = IF; 
  // while(token != DONE && token != LEXERROR) {
  //     token = lexan(fd);
  //     lexer_emit(token, tokenval);
  // }
  // TODO: here is an example of what this function might look like, 
  //       you may completely change this:
  ast_info *s;
  ast_node *n;

  // create the root AST node
  s = create_new_ast_node_info(NONTERMINAL, 0, ROOT, 0, 0);
  n = create_ast_node(s);
  if(init_ast(&ast_tree, n)) {
        parser_error("ERROR: bad AST\n");
  }
  lookahead = lexan(fd);
  program(fd, ast_tree.root);  // program corresponds to the start state
  /*
  // the last token should be DONE
  if (lookahead != DONE) {
    parser_error("expected end of file");   
  } else {
     match(DONE, fd);
  }
  */

}
/**************************************************************************/
static void parser_error(char *err_string) {
  if(err_string) {
    printf("%s\n", err_string);
  }
  exit(1);
}  
/**************************************************************************/
/*
 *  this function corresponds to the start symbol in the LL(1) grammar
 *  when this function returns, the full AST tree with parent node "parent"
 *  will be constructed
 *         fd: the input stream
 *     parent: the parent ast node  (it should be a ROOT)
 */
static void program(FILE *fd, ast_node *parent) {
  printf("program\n");
  switch( lookahead ) {
    case INT:
      match(INT, fd);
      match(ID, fd);
      program_1(fd, parent);
    break;
    case CHAR:
      match(CHAR, fd);
      match(ID, fd);
      program_1(fd, parent);
    break;
    default:
      parser_error("Your program starts with invalid symbol");
      break;
  }

  // assert is useful for testing a function's pre and post conditions 
  assert(parent->symbol->token == NONTERMINAL);
  assert(parent->symbol->grammar_symbol == ROOT);
}

static void program_1(FILE *fd, ast_node *parent) {
    printf("program1\n");
  switch( lookahead ) {
    case SEMIC:
      match(SEMIC, fd);
      program(fd, parent);
    break;
    case LBRACKET:
      match(LBRACKET, fd);
      match(NUM, fd);
      match(RBRACKET, fd);
      match(SEMIC, fd);
      program(fd, parent);
    break;
    default:
      program_2(fd, parent);
    break;
  }
}

static void program_2(FILE *fd, ast_node *parent) {
    printf("program2\n");

  switch( lookahead ) {
    case LPAREN: 
      match(LPAREN, fd);
      fdl1(fd, parent);
      break;
    default:
      parser_error("Expected \"(\"");
      break;
  }
}

static void fdl1(FILE *fd, ast_node *parent) {
  printf("fdl1\n");
  pdl(fd, parent);
  match(RPAREN, fd);
  match(LBRACE, fd);
  block(fd, parent);
  fdl(fd, parent);
}

static void fdl(FILE *fd, ast_node *parent) {
  switch( lookahead ) {
    case INT:
      match(INT, fd); 
      match(ID, fd);
      match(LPAREN, fd);
      fdl1(fd, parent);
      break;
    case CHAR:
      match(CHAR, fd); 
      match(ID, fd);
      match(LPAREN, fd);
      fdl1(fd, parent);
      break;      
    default:
      break;
  }
}

static void pdl(FILE *fd, ast_node *parent) {
  printf("pdl\n");
  switch( lookahead ) {
    case INT: 
      match(INT, fd); 
      match(ID, fd);
      pdl1(fd, parent);
      break;
    case CHAR:
      match(CHAR, fd); 
      match(ID, fd);
      pdl1(fd, parent);
      break;      
    default:
      break;
  }
}

static void pdl1(FILE *fd, ast_node *parent) {
  printf("pdl1\n");
  switch( lookahead ) {
    case LBRACKET:
      match(LBRACKET, fd);
      match(RBRACKET, fd);
      pdl2(fd, parent);
      break;
    default:
      pdl2(fd, parent);
      break;
  }
}

static void pdl2(FILE *fd, ast_node *parent) {
  printf("pdl2\n");
  switch( lookahead ) {
    case COMMA:
      match(COMMA, fd);
      pdl(fd, parent);
      break;
    default:
      break;
  }
}

static void block(FILE *fd, ast_node *parent) {
  vdl(fd, parent);
  stmt_list(fd, parent);
}

static void vdl(FILE *fd, ast_node *parent) {
  printf("vdl\n");
  switch( lookahead ) {
    case INT:
      match(INT, fd);
      match(ID, fd);
      vdl1(fd, parent);
      break;     
    case CHAR:
      match(CHAR, fd);
      match(ID, fd);
      vdl1(fd, parent);
      break;
    default:
      break;
  }
}

static void vdl1(FILE *fd, ast_node *parent) {
  printf("vdl1\n");
  switch( lookahead ) {
    case SEMIC:
      match(SEMIC, fd);
      vdl(fd, parent);
      break;
    case LBRACKET:
      match(LBRACKET, fd);
      match(NUM, fd);
      match(RBRACKET, fd);
      match(SEMIC, fd);
      vdl(fd, parent);
      break;
    default:
      parser_error("VDL1: should end here.");
  }
}

static void stmt_list(FILE *fd, ast_node *parent) {
  stmt(fd, parent);
  stmt_list1(fd, parent);
}

static void stmt(FILE *fd, ast_node *parent) {
  switch( lookahead ) {
    case SEMIC:
      match(SEMIC, fd);
      break;
    case RETURN:
      match(RETURN, fd);
      expr(fd, parent);
      match(SEMIC, fd);
      break;
    case WRITE:
      match(WRITE, fd);
      expr(fd, parent);
      match(SEMIC, fd);
      break;
    case READ:
      match(READ, fd);
      match(ID, fd);
      match(SEMIC, fd);
      break;
    case WRITELN:
      match(WRITELN, fd);
      match(SEMIC, fd);
      break;
    case BREAK:
      match(BREAK, fd);
      match(SEMIC, fd);
      break;
    case IF:
      match(IF, fd);
      match(LPAREN, fd);
      expr(fd, parent);
      match(RPAREN, fd);
      stmt(fd, parent);
      match(ELSE, fd);
      stmt(fd, parent);
      break;
    case WHILE:
      match(WHILE, fd);
      match(LPAREN, fd);
      expr(fd, parent);
      match(RPAREN, fd);
      stmt(fd, parent);
      break;
    case LBRACKET:
      match(LBRACKET, fd);
      block(fd,parent);
      break;
    default:
      parser_error("invalid begining of a statement");
      break;
  }
}

static void stmt_list1(FILE *fd, ast_node *parent) {
  switch( lookahead ) {
    case RBRACE:
      match(RBRACE, fd);
      break;
    default:
      stmt_list(fd, parent);
      break;
  }
}

static void expr(FILE *fd, ast_node *parent) {
  e1(fd, parent);
  e0_p(fd, parent);
}

static void e0_p(FILE *fd, ast_node *parent) {
  switch (lookahead){
    case ASSIGN:
      match(ASSIGN, fd);
      expr(fd, parent);
      break;
    default:
    break;
  }
}

static void e1(FILE *fd, ast_node *parent) {
  e2(fd,parent);
  e1_p(fd, parent);
}

static void e1_p(FILE *fd, ast_node *parent) {
  switch( lookahead ){
    case OR: 
      match(OR, fd);
      e2(fd,parent);
      e1_p(fd,parent);
      break;
    default:
      break;
  }
}

static void e2(FILE *fd, ast_node *parent) {
  e3(fd,parent);
  e2_p(fd,parent);
}

static void e2_p(FILE *fd, ast_node *parent) {
  switch( lookahead ){
    case AND: 
      match(AND, fd);
      e3(fd,parent);
      e2_p(fd,parent);
      break;
    default:
      break;
  }
}

static void e3(FILE *fd, ast_node *parent) {
  e4(fd,parent);
  e3_p(fd, parent);
}

static void e3_p(FILE *fd, ast_node *parent) {
  switch( lookahead ){
    case EQ: 
      match(EQ, fd);
      e4(fd,parent);
      e3_p(fd,parent);
      break;
    case NEQ:
      match(NEQ, fd);
      e4(fd,parent);
      e3_p(fd,parent);
      break;
    default:
      break;
  }
}

static void e4(FILE *fd, ast_node *parent) {
  e5(fd, parent);
  e4_p(fd, parent);
}


static void e4_p(FILE *fd, ast_node *parent) {
  switch( lookahead ){
    case LESS: 
      match(LESS, fd);
      e5(fd,parent);
      e4_p(fd,parent);
      break;
    case LESSEQ:
      match(LESSEQ, fd);
      e5(fd,parent);
      e4_p(fd,parent);
      break;
    case MORE: 
      match(MORE, fd);
      e5(fd,parent);
      e4_p(fd,parent);
      break;
    case MOREEQ:
      match(MOREEQ, fd);
      e5(fd,parent);
      e4_p(fd,parent);
      break;
    default:
      break;
  }
}

static void e5(FILE *fd, ast_node *parent) {
  e6(fd,parent);
  e5_p(fd, parent);
}

static void e5_p(FILE *fd, ast_node *parent) {
switch( lookahead ){
    case PLUS: 
      match(PLUS, fd);
      e6(fd,parent);
      e5_p(fd,parent);
      break;
    case MINUS:
      match(MINUS, fd);
      e6(fd,parent);
      e5_p(fd,parent);
      break;
    default:
      break;
  }
}

static void e6(FILE *fd, ast_node *parent) {
  e7(fd,parent);
  e6_p(fd,parent);
}

static void e6_p(FILE *fd, ast_node *parent) {
switch( lookahead ){
    case STAR: 
      match(STAR, fd);
      e7(fd,parent);
      e6_p(fd,parent);
      break;
    case DIV:
      match(DIV, fd);
      e7(fd,parent);
      e6_p(fd,parent);
      break;
    default:
      break;
  }
}

static void e7(FILE *fd, ast_node *parent) {
  switch(lookahead) {
    case NOT:
      match(NOT, fd);
      e7(fd, parent);
    break;
    case MINUS:
      match(MINUS, fd);
      e7(fd, parent);
    break;
    default:
      e8(fd, parent);
    break;
  }
}

static void e8(FILE *fd, ast_node *parent) {
    switch(lookahead) {
    case NUM:
      match(NUM, fd);
    break;
    case LPAREN:
      match(LPAREN, fd);
      expr(fd, parent);
      match(RPAREN, fd);
    break;
    case ID:
      match(ID, fd);
      e8_p(fd, parent);
      break;
    default:
      parser_error("Error!");
    break;
  }
}

static void e8_p(FILE *fd, ast_node *parent) {
    switch( lookahead ) {
    case LPAREN:
      match(LPAREN, fd);
      expr_list(fd, parent);
      match(RPAREN, fd);
      break;
    case LBRACKET:
      match(LBRACKET, fd);
      expr(fd, parent);
      match(RBRACKET, fd);
      break;
    default:
      break;
  }
}

static void expr_list(FILE *fd, ast_node *parent) {
  switch(lookahead) {
    case MINUS: case NOT: 
    case LPAREN: case ID: case NUM:
      expr(fd, parent);
      expr_list_p(fd, parent);
      break;
    default:
      break;
  }
}


static void expr_list_p(FILE *fd, ast_node *parent) {
  switch(lookahead) {
    case COMMA:
      match(COMMA, fd);
      expr_list(fd, parent);
      break;
    default:
      break;
  }
}

static void match(tokenT token, FILE *fd) {
  if(token != lookahead) {
      parser_error("Expected a different value");
  }
  lookahead = lexan(fd);
}

