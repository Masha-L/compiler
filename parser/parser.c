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
static void program_1(FILE *fd, ast_node *parent, ast_node *sibling);
static void program_2(FILE *fd, ast_node *parent, ast_node *sibling);
static void fdl1(FILE *fd, ast_node *parent, ast_node *sibling);
static void fdl(FILE *fd, ast_node *parent);
static void pdl(FILE *fd, ast_node *parent);
static void pdl1(FILE *fd, ast_node *parent, ast_node *sibling);
static void pdl2(FILE *fd, ast_node *parent);
static void block(FILE *fd, ast_node *parent);
static void vdl(FILE *fd, ast_node *parent);
static void vdl1(FILE *fd, ast_node *parent, ast_node *sibling);
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
static ast_node * new_node(int token, int value, int grammar_sym,
                                  char * lexeme, int line_no);

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
  
  // the last token should be DONE
  if (lookahead != DONE) {
    parser_error("expected end of file");   
  } else {
     match(DONE, fd);
  }
  

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
  ast_node *child = NULL;
  switch( lookahead ) {
    case INT:
    {
      ast_node *type = new_node(lookahead, 0, 0, 0, 0);
      match(INT, fd);
      ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
      match(ID, fd);
      if(lookahead == LPAREN) {
        child = new_node(NONTERMINAL,0, FUNC_DEC,0,0);
        add_child_node(parent, child);
        add_child_node(child, type);
        add_child_node(child, id);
      }
      else if(lookahead == LBRACKET || lookahead == SEMIC) {
        child = new_node(NONTERMINAL,0, VAR_DEC,0,0);
        add_child_node(parent, child);
        add_child_node(child, type);
        add_child_node(child, id);
      }
      else {
        parser_error("ERROR");
      }
      program_1(fd, parent, child);
    }
    break;
    case CHAR:
    {
      ast_node *type = new_node(lookahead, 0, 0, 0, 0);
      match(CHAR, fd);
      ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
      match(ID, fd);
      if(lookahead == LPAREN) {
        child = new_node(NONTERMINAL,0, FUNC_DEC,0,0);
        add_child_node(parent, child);
        add_child_node(child, type);
        add_child_node(child, id);
      }
      else if(lookahead == LBRACKET || lookahead == SEMIC) {
        child = new_node(NONTERMINAL,0, VAR_DEC,0,0);
        add_child_node(parent, child);
        add_child_node(child, type);
        add_child_node(child, id);
      }
      else {
        parser_error("ERROR");
      }
      program_1(fd, parent, child);
    }
    break;
    default:
      parser_error("Your program starts with invalid symbol");
      break;
  }

  // // assert is useful for testing a function's pre and post conditions 
  // assert(parent->symbol->token == NONTERMINAL);
  // assert(parent->symbol->grammar_symbol == ROOT);
}

static void program_1(FILE *fd, ast_node *parent, ast_node *sibling) {
  switch( lookahead ) {
    case SEMIC:
      match(SEMIC, fd);
      program(fd, parent);
    break;
    case LBRACKET:
    {
      ast_node * lb = new_node(lookahead, 0, 0, 0, 0);
      match(LBRACKET, fd);
      add_child_node(sibling, lb);
      ast_node * size = new_node(lookahead, tokenval, 0, 0, 0);
      match(NUM, fd);
      add_child_node(sibling, size);
      ast_node * rb = new_node(lookahead, 0, 0, 0, 0);
      match(RBRACKET, fd);
      add_child_node(sibling, rb);      
      match(SEMIC, fd);
      program(fd, parent);
    break;
    }
    default:
      program_2(fd, parent, sibling);
    break;
  }
}

static void program_2(FILE *fd, ast_node *parent, ast_node *sibling) {
  printf("program2\n");
  switch( lookahead ) {
    case LPAREN: 
      match(LPAREN, fd);
      fdl1(fd, parent, sibling);
      break;
    default:
      parser_error("Expected \"(\"");
      break;
  }
}

static void fdl1(FILE *fd, ast_node *parent, ast_node *sibling) {
  printf("fdl1\n");
  ast_node * param_list = new_node(NONTERMINAL,0, PARAM_LIST,0,0);
  pdl(fd, param_list);
  if(get_num_children(param_list)> 0) {
      add_child_node(sibling, param_list);
  }
  match(RPAREN, fd);
  match(LBRACE, fd);
  ast_node * block_dec = new_node(NONTERMINAL,0, BLOCK,0,0);
  block(fd, block_dec);
  if(get_num_children(block_dec)> 0) {
      add_child_node(sibling, block_dec);
  }
  fdl(fd, parent);
}

static void fdl(FILE *fd, ast_node *parent) {
  printf("fdl\n");
  ast_node *child = NULL;
  switch( lookahead ) {
    case INT:
      match(INT, fd); 
      ast_node *type = new_node(INT, 0, 0, 0, 0);
      ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
      match(ID, fd);
      child = new_node(NONTERMINAL,0, FUNC_DEC,0,0);
      add_child_node(parent, child);
      add_child_node(child, type);
      add_child_node(child, id);
      match(LPAREN, fd);
      fdl1(fd, parent, child);
      break;
    case CHAR:
      match(CHAR, fd); 
      match(ID, fd);
      match(LPAREN, fd);
      fdl1(fd, parent, child);
      break;      
    default:
      break;
  }
}

static void pdl(FILE *fd, ast_node *parent) {
  printf("pdl\n");
  ast_node *child = NULL;
  switch( lookahead ) {
    case INT: 
    {
      child = new_node(NONTERMINAL,0, PAR_DEC,0,0);
      ast_node *type = new_node(lookahead, 0, 0, 0, 0);
      match(INT, fd);
      add_child_node(child, type);
      ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
      match(ID, fd);
      add_child_node(child, id);
      pdl1(fd, parent, child);
      add_child_node(parent, child);
      break;
    }
    case CHAR:
    {
      child = new_node(NONTERMINAL,0, PAR_DEC,0,0);
      ast_node *type = new_node(lookahead, 0, 0, 0, 0);
      match(CHAR, fd); 
      add_child_node(child, type);
      ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
      match(ID, fd);
      add_child_node(child, id);
      pdl1(fd, parent, child);
      add_child_node(parent, child);
      break; 
    }     
    default:
      break;
  }
}

static void pdl1(FILE *fd, ast_node *parent, ast_node *sibling) {
  printf("pdl1\n");
  switch( lookahead ) {
    case LBRACKET:
      {      
        ast_node * lb = new_node(lookahead, 0, 0, 0, 0);
        match(LBRACKET, fd);
        ast_node * rb = new_node(lookahead, 0, 0, 0, 0);
        match(RBRACKET, fd);
        add_child_node(sibling, lb);
        add_child_node(sibling, rb);
        pdl2(fd, parent);
      break;
    }
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
  printf("block\n");
  vdl(fd, parent);
  ast_node * stat_list = new_node(NONTERMINAL,0, STAT_LIST,0,0);
  stmt_list(fd, stat_list);
  if(get_num_children(stat_list)> 0) {
      add_child_node(parent, stat_list);
  }
}

static void vdl(FILE *fd, ast_node *parent) {
  printf("vdl\n");
  ast_node *child = NULL;
  switch( lookahead ) {
    case INT:
    {
      child = new_node(NONTERMINAL,0, VAR_DEC,0,0);
      ast_node *type = new_node(lookahead, 0, 0, 0, 0);
      match(INT, fd);
      add_child_node(child, type);
      ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
      match(ID, fd);
      add_child_node(child, id);
      add_child_node(parent, child);
      vdl1(fd, parent, child);
      break;
    }    
    case CHAR:
    {
      child = new_node(NONTERMINAL,0, VAR_DEC,0,0);
      ast_node *type = new_node(lookahead, 0, 0, 0, 0);
      match(CHAR, fd);
      add_child_node(child, type);
      ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
      match(ID, fd);
      add_child_node(child, id);
      add_child_node(parent, child);
      vdl1(fd, parent, child);
      break;
    }
    default:
      break;
  }
}

static void vdl1(FILE *fd, ast_node *parent, ast_node *sibling) {
  printf("vdl1\n");
  switch( lookahead ) {
    case SEMIC:
      match(SEMIC, fd);
      vdl(fd, parent);
      break;
    case LBRACKET:
    {
      ast_node * lb = new_node(lookahead, 0, 0, 0, 0);
      match(LBRACKET, fd);
      add_child_node(sibling, lb);
      ast_node * size = new_node(lookahead, tokenval, 0, 0, 0);
      match(NUM, fd);
      add_child_node(sibling, size);
      ast_node * rb = new_node(lookahead, 0, 0, 0, 0);
      match(RBRACKET, fd);
      add_child_node(sibling, rb);      
      match(SEMIC, fd);
      vdl(fd, parent);
      break;
    }
    default:
      parser_error("VDL1: should end here.");
  }
}

static void stmt_list(FILE *fd, ast_node *parent) {
  printf("stmt_list\n");
  stmt(fd, parent);
  stmt_list1(fd, parent);
}

static void stmt(FILE *fd, ast_node *parent) {
  printf("stmt\n");
  ast_node * stat = NULL;
  switch( lookahead ) {
    case SEMIC:
      match(SEMIC, fd);
      break;
    case RETURN:
      match(RETURN, fd);
      stat = new_node(NONTERMINAL,0, STAT,0,0);
      ast_node * return_n = new_node(RETURN,0, 0,0,0);
      add_child_node(stat, return_n);
      add_child_node(parent, stat);
      expr(fd, return_n);
      match(SEMIC, fd);
      break;
    case WRITE:
      match(WRITE, fd);
      stat = new_node(NONTERMINAL,0, STAT,0,0);
      ast_node * w = new_node(WRITE,0, 0,0,0);
      add_child_node(stat, w);
      add_child_node(parent, stat);
      expr(fd, w);
      match(SEMIC, fd);
      break;
    case READ:
      match(READ, fd);
      stat = new_node(NONTERMINAL,0, STAT,0,0);
      ast_node * r = new_node(READ,0, 0,0,0);
      add_child_node(stat, r);
      add_child_node(parent, stat);
      ast_node * id = new_node(ID,0, 0,lexbuf,0);
      add_child_node(r, id);
      match(ID, fd);
      match(SEMIC, fd);
      break;
    case WRITELN:
      match(WRITELN, fd);
      stat = new_node(NONTERMINAL,0, STAT,0,0);
      ast_node * wn = new_node(WRITELN,0, 0,0,0);
      add_child_node(stat, wn);
      add_child_node(parent, stat);
      match(SEMIC, fd);
      break;
    case BREAK:
      match(BREAK, fd);
      stat = new_node(NONTERMINAL,0, STAT,0,0);
      ast_node * b = new_node(BREAK,0, 0,0,0);
      add_child_node(stat, b);
      add_child_node(parent, stat);
      match(SEMIC, fd);
      break;
    case IF:
      match(IF, fd);
      stat = new_node(NONTERMINAL,0, STAT,0,0);
      ast_node * iff = new_node(IF,0, 0,0,0);
      match(LPAREN, fd);
      expr(fd, iff);
      match(RPAREN, fd);
      stmt(fd, iff);
      add_child_node(stat, iff);
      add_child_node(parent, stat);
      match(ELSE, fd);
      ast_node * el = new_node(ELSE,0, 0,0,0);
      stmt(fd, el);
      if(get_num_children(el) > 0){
        add_child_node(stat, el);
      }
      break;
    case WHILE:
      match(WHILE, fd);
      stat = new_node(NONTERMINAL,0, STAT,0,0);
      ast_node * wh = new_node(WHILE,0, 0,0,0);
      add_child_node(stat, wh);
      add_child_node(parent, stat);
      match(LPAREN, fd);
      expr(fd, wh);
      match(RPAREN, fd);
      stmt(fd, wh);
      break;
    case LBRACE:
      match(LBRACE, fd);
      ast_node * block_dec = new_node(NONTERMINAL,0, BLOCK,0,0);
      block(fd, block_dec);
      if(get_num_children(block_dec)> 0) {
        add_child_node(parent, block_dec);
      }
      break;
    default:
      expr(fd, parent);
      match(SEMIC, fd);
      break;
  }
}

static void stmt_list1(FILE *fd, ast_node *parent) {
  printf("stmt_list1\n");
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
  printf("expr\n");
  e1(fd, parent);
  e0_p(fd, parent);
}

static void e0_p(FILE *fd, ast_node *parent) {
  printf("e0_p\n");
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
  printf("e1\n");
  e2(fd,parent);
  e1_p(fd, parent);
}

static void e1_p(FILE *fd, ast_node *parent) {
  printf("e1_p\n");
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
  printf("e2\n");
  e3(fd,parent);
  e2_p(fd,parent);
}

static void e2_p(FILE *fd, ast_node *parent) {
  printf("e2_p\n");
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
  printf("e3\n");
  e4(fd,parent);
  e3_p(fd, parent);
}

static void e3_p(FILE *fd, ast_node *parent) {
  printf("e3_p\n");
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
  printf("e4\n");
  e5(fd, parent);
  e4_p(fd, parent);
}


static void e4_p(FILE *fd, ast_node *parent) {
  printf("e4_p\n");
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
  printf("e5\n");
  e6(fd,parent);
  e5_p(fd, parent);
}

static void e5_p(FILE *fd, ast_node *parent) {
  printf("e5_p\n");
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
  printf("e6\n");
  e7(fd,parent);
  e6_p(fd,parent);
}

static void e6_p(FILE *fd, ast_node *parent) {
  printf("e6_p\n");
  switch( lookahead ){
    case STAR: 
      match(STAR, fd);
      e7(fd,parent);
      e6_p(fd,parent);
      break;
    case DIV:
      match(DIV, fd);
      lexer_emit(lookahead, tokenval);
      e7(fd,parent);
      e6_p(fd,parent);
      break;
    default:
      break;
  }
}

static void e7(FILE *fd, ast_node *parent) {
  printf("e7\n");
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
  printf("e8\n");
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
  printf("e8_p\n");
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
  printf("expr_list\n");
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
  printf("expr_list_p\n");
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
  printf("MATCH: ");
  lexer_emit(lookahead, tokenval);
  lookahead = lexan(fd);  
}

static ast_node * new_node(int token, int value, int grammar_sym,
                                  char * lexeme, int line_no) {
  ast_info *s = create_new_ast_node_info(token, value, grammar_sym, lexeme, line_no);
  ast_node *n = create_ast_node(s);
  return n;
}
