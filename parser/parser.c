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

static void program(FILE *fd, ast_node *parent);
static void program_h(tokenT type, FILE * fd, ast_node* parent);
static void parser_error(char *err_string, tokenT expected, tokenT got);
static void warning(char *err_string, tokenT expected, tokenT got);
static void match(tokenT token, FILE* fd);
static void program_1(FILE *fd, ast_node *parent, ast_node *sibling);
static void program_2(FILE *fd, ast_node *parent, ast_node *sibling);
static void fdl1(FILE *fd, ast_node *parent, ast_node *sibling);
static void fdl(FILE *fd, ast_node *parent);
static void fdl_h(tokenT v_type, FILE *fd, ast_node *parent);
static void pdl(FILE *fd, ast_node *parent);
static void pdl_h(tokenT v_type, FILE *fd, ast_node *parent);
static void pdl1(FILE *fd, ast_node *parent, ast_node *sibling);
static void pdl2(FILE *fd, ast_node *parent);
static void block(FILE *fd, ast_node *parent);
static void vdl(FILE *fd, ast_node *parent);
static void vdl_h(tokenT v_type, FILE *fd, ast_node *parent);
static void vdl1(FILE *fd, ast_node *parent, ast_node *sibling);
static void stmt_list(FILE *fd, ast_node *parent);
static void stmt(FILE *fd, ast_node *parent);
static void stmt_list1(FILE *fd, ast_node *parent);
static void expression_rec(FILE * fd);
static ast_node * expr(FILE *fd);
static void expr_list_p(FILE *fd, ast_node *parent);
static void expr_list(FILE *fd, ast_node *parent);
static ast_node * e1(FILE *fd);
static ast_node * e2(FILE *fd);
static ast_node * e3(FILE *fd);
static ast_node * e4(FILE *fd);
static ast_node * e5(FILE *fd);
static ast_node * e6(FILE *fd);
static ast_node * e7(FILE *fd);
static ast_node * e8(FILE *fd);
static ast_node * e0_p(FILE *fd);
static ast_node * e1_p(FILE *fd);
static ast_node * e2_p(FILE *fd);
static ast_node * e3_p(FILE *fd);
static ast_node * e4_p(FILE *fd);
static ast_node * e5_p(FILE *fd);
static ast_node * e6_p(FILE *fd);
static ast_node * e8_p(FILE *fd, ast_node *parent);
static ast_node *e3_h(FILE *fd);
static ast_node *e4_h(FILE *fd);
static ast_node *e5_h(FILE *fd);
static ast_node *e6_h(FILE *fd);
static ast_node *e7_h(tokenT token, FILE* fd);
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
  ast_info *s;
  ast_node *n;

  // create the root AST node
  s = create_new_ast_node_info(NONTERMINAL, 0, ROOT, 0, 0);
  n = create_ast_node(s);
  if(init_ast(&ast_tree, n)) {
        parser_error("ERROR: bad AST\n", -1, -1);
  }
  lookahead = lexan(fd);
  program(fd, ast_tree.root);  // program corresponds to the start state
  
  // the last token should be DONE
  if (lookahead != DONE) {
    parser_error("Expected end of file", DONE, lookahead);   
  } else {
     match(DONE, fd);
  }
  

}
/**************************************************************************/
static void parser_error(char *err_string, tokenT expected, tokenT got) {
  if(err_string) {
    printf("\n Error at line %d: %s ", src_lineno, err_string);
  }
  if(expected!=-1) {
    printf("Expected ");
    lexer_emit(expected, tokenval);
    printf("But got ");
    lexer_emit(lookahead, tokenval);
    printf("\n");
  }
  exit(1);
}  
static void warning(char *err_string, tokenT expected, tokenT got) {
  if(err_string) {
    printf("Warning at line %d: %s\n", src_lineno, err_string);
  }
  if(expected!=-1) {
    printf("\nExpected ");
    lexer_emit(expected, tokenval);
    printf("But got ");
    lexer_emit(lookahead, tokenval);
    printf("\n");
  }
} 
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
      program_h(INT, fd, parent);
    break;
    case CHAR:
      program_h(CHAR, fd, parent);
    break;
    default:
      while(lookahead != CHAR && lookahead != INT) {
        warning("Expected a different value at the begining of the program", -1, -1);
        lookahead = lexan(fd);
        if(lookahead == DONE) {
          parser_error("Reached the end of the file", -1, -1);
        }
      }
      break;
  }
}

static void program_h(tokenT v_type, FILE * fd, ast_node* parent) {
  ast_node * child = NULL;
  ast_node * type = new_node(lookahead, 0, 0, 0, 0);
      match(v_type, fd);
      ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
      match(ID, fd);
      while(lookahead != LBRACKET && lookahead != SEMIC && lookahead != LPAREN) {
        warning("Wrong symbol in function or variable declaration", -1, -1);
        lookahead = lexan(fd);
        if(lookahead == DONE) {
          parser_error("Unexpected DONE symbol", -1,-1);
        }
      }
      if(lookahead == LPAREN) {
        child = new_node(NONTERMINAL,0, FUNC_DEC,0,0);
        add_child_node(parent, child);
        add_child_node(child, type);
        add_child_node(child, id);
      }
      else {
        child = new_node(NONTERMINAL,0, VAR_DEC,0,0);
        add_child_node(parent, child);
        add_child_node(child, type);
        add_child_node(child, id);
      }
      program_1(fd, parent, child);
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
      while(LPAREN != lookahead) {
        warning("Expected a different value", LPAREN, lookahead);
        lookahead = lexan(fd);
        if(lookahead == DONE) {
          parser_error("Reached the end of the file.", LPAREN, lookahead);
        }
      }
      program_2(fd, parent, sibling);
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
  switch( lookahead ) {
    case INT:
      fdl_h(INT, fd, parent);
      break;
    case CHAR:
      fdl_h(CHAR, fd, parent);
      break;      
    default:
      break;
  }
}

static void fdl_h(tokenT v_type, FILE *fd, ast_node *parent){
  ast_node *child = NULL; 
  match(v_type, fd); 
  ast_node *type = new_node(v_type, 0, 0, 0, 0);
  ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
  match(ID, fd);
  child = new_node(NONTERMINAL,0, FUNC_DEC,0,0);
  add_child_node(parent, child);
  add_child_node(child, type);
  add_child_node(child, id);
  match(LPAREN, fd);
  fdl1(fd, parent, child);
}

static void pdl(FILE *fd, ast_node *parent) {
  printf("pdl\n");
  switch( lookahead ) {
    case INT: 
      pdl_h(INT, fd, parent);
      break;
    case CHAR:
      pdl_h(CHAR, fd, parent);
      break;  
    default:
      break;
  }
}

static void pdl_h(tokenT v_type, FILE *fd, ast_node *parent){
  ast_node * child = new_node(NONTERMINAL,0, PAR_DEC,0,0);
  ast_node * type = new_node(lookahead, 0, 0, 0, 0);
  match(v_type, fd); 
  add_child_node(child, type);
  ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
  match(ID, fd);
  add_child_node(child, id);
  pdl1(fd, parent, child);
  add_child_node(parent, child);
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
  ast_node * stat_list = new_node(NONTERMINAL, 0, STAT_LIST,0,0);
  stmt_list(fd, stat_list);
  if(get_num_children(stat_list) > 0) {
      add_child_node(parent, stat_list);
  }
}

static void vdl(FILE *fd, ast_node *parent) {
  printf("vdl\n");
  switch( lookahead ) {
    case INT:
      vdl_h(INT, fd, parent);
      break;  
    case CHAR:
      vdl_h(CHAR, fd, parent);
      break;  
    default:
      break;
  }
}

static void vdl_h(tokenT v_type, FILE *fd, ast_node *parent) {
  ast_node *child = new_node(NONTERMINAL,0, VAR_DEC,0,0);
  ast_node *type = new_node(lookahead, 0, 0, 0, 0);
  match(v_type, fd);
  add_child_node(child, type);
  ast_node *id = new_node(lookahead, 0, 0, lexbuf, 0);
  match(ID, fd);
  add_child_node(child, id);
  add_child_node(parent, child);
  vdl1(fd, parent, child);
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
      while(LBRACKET != lookahead && SEMIC != lookahead) {
        warning("Expected a different value", LBRACKET, lookahead);
        warning("Expected a different value", SEMIC, lookahead);
        lookahead = lexan(fd);
        if(lookahead == DONE) {
          parser_error("Reached the end of the file.", -1, -1);
        }
      }
      vdl1(fd, parent, sibling);
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
      add_child_node(return_n, expr(fd));
      match(SEMIC, fd);
      break;
    case WRITE:
      match(WRITE, fd);
      stat = new_node(NONTERMINAL,0, STAT,0,0);
      ast_node * w = new_node(WRITE,0, 0,0,0);
      add_child_node(stat, w);
      add_child_node(parent, stat);
      add_child_node(w, expr(fd));
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
      add_child_node(iff, expr(fd));
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
      add_child_node(wh, expr(fd));
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
      add_child_node(parent, expr(fd));
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

static ast_node * expr(FILE *fd) {
  printf("expr\n");
  ast_node * l_ch = NULL;
  switch(lookahead) {
    case NOT: case MINUS: case NUM: case LPAREN: case ID:
       l_ch = e1(fd);
       if(lookahead == ASSIGN){
         ast_node * p = new_node_with_ch(ASSIGN, 0,0,0,0, l_ch, e0_p(fd));
         return p;
       }
       return l_ch;
    break;
    default:
    expression_rec(fd);
    return expr(fd);
  }
  return NULL;
}

static void expression_rec(FILE * fd) {
  while( lookahead != NOT && lookahead != ID && lookahead != NUM
          && lookahead != LPAREN && lookahead != MINUS) {
      warning("Expected a different symbol at expression", -1,-1);
      lookahead = lexan(fd);
      if(lookahead == DONE){
        parser_error("Reached the end of the file.", -1, -1);
      }
    }
}

static ast_node * e0_p(FILE *fd) {
  printf("e0_p\n");
  match(ASSIGN, fd);
  return expr(fd);
}

static ast_node * e1(FILE *fd) {
  printf("e1\n");
  ast_node * l_ch = NULL;
  switch(lookahead) {
    case NOT: case MINUS: case NUM: case LPAREN: case ID:
       l_ch = e2(fd);
       if(lookahead == OR) {
          ast_node * p = new_node_with_ch(OR,0,0,0,0, l_ch, e1_p(fd));
          return p;
       }
       return l_ch;
    default:
      return NULL;
    }
}

static ast_node * e1_p(FILE *fd) {
  printf("e1_p\n");
  ast_node * l_ch = NULL;
  match(OR, fd);
  switch( lookahead ) {
    case NOT: case MINUS: case NUM: case LPAREN: case ID:
      l_ch = e2(fd);
      if(lookahead == OR) {
          ast_node * p = new_node_with_ch(OR,0,0,0,0, l_ch, e1_p(fd));
          return p;
      }
      return l_ch;
    default:
      return NULL;
  }
}

static ast_node * e2(FILE *fd) {
  printf("e2\n");
  ast_node * l_ch = NULL;
  switch( lookahead ) {
    case NOT: case MINUS: case NUM: case LPAREN: case ID:
      l_ch = e3(fd);
      if(lookahead == AND) {
        ast_node * p = new_node_with_ch(AND,0,0,0,0, l_ch, e2_p(fd));
        return p;
      }
      return l_ch;
    default:
      return NULL;
  }
}

static ast_node *e2_p(FILE *fd) {
  printf("e2_p\n");
  match(AND, fd);
  ast_node * l_ch = NULL;
  switch( lookahead ) {
    case NOT: case MINUS: case NUM: case LPAREN: case ID:
      l_ch = e3(fd);
      tokenT temp = lookahead;
      if(temp == EQ || temp == NEQ){
        ast_node * p = new_node_with_ch(temp,0,0,0,0, l_ch, e2_p(fd));
        return p;
      }
      return l_ch;
      default: 
        return NULL;
    }
}

static ast_node * e3(FILE *fd) {
  printf("e3\n");
  return e3_h(fd);
}

static ast_node *e3_h(FILE *fd) {
  ast_node * l_ch = NULL;
  switch( lookahead ){
    case NOT: case MINUS: case NUM: case LPAREN: case ID:
      l_ch = e4(fd);
      tokenT temp = lookahead;
      if(temp == EQ || temp == NEQ){
        ast_node * p = new_node_with_ch(temp,0,0,0,0, l_ch, e3_p(fd));
        return p;
      }
      return l_ch;
      default:
        return NULL;
  }
}

static ast_node * e3_p(FILE *fd) {
  printf("e3_p\n");
  switch( lookahead ){
    case EQ: 
      match(EQ, fd);
      return e3_h(fd);
    case NEQ:
      match(NEQ, fd);
      return e3_h(fd);
    default:
      return NULL;
  }
}

static ast_node * e4(FILE *fd) {
  printf("e4\n");
  return e4_h(fd);
}

static ast_node *e4_h(FILE *fd) {
  ast_node * l_ch = NULL;
  switch( lookahead ){
    case NOT: case MINUS: case NUM: case LPAREN: case ID:
      l_ch = e5(fd);
      tokenT temp = lookahead;
      if(temp == LESS || temp == LESSEQ || temp == MORE || temp == MOREEQ){
        ast_node * p = new_node_with_ch(temp,0,0,0,0, l_ch, e4_p(fd));
        return p;
      }
      return l_ch;
      default:
       return NULL;
  }
}


static ast_node * e4_p(FILE *fd) {
  printf("e4_p\n");
  switch( lookahead ){
    case LESS: 
      match(LESS, fd);
      return e4_h(fd);
    case LESSEQ:
      match(LESSEQ, fd);
      return e4_h(fd);
    case MORE: 
      match(MORE, fd);
      return e4_h(fd);
    case MOREEQ:
      match(MOREEQ, fd);
      return e4_h(fd);
    default:
      break;
  }
  return NULL;
}

static ast_node * e5(FILE *fd) {
  printf("e5\n");
  return e5_h(fd);
}

static ast_node *e5_h(FILE *fd) {
  ast_node * l_ch = NULL;
  switch( lookahead ){
    case NOT: case MINUS: case NUM: case LPAREN: case ID:
      l_ch = e6(fd);
      tokenT temp = lookahead;
      if(temp == PLUS || temp == MINUS){
        ast_node * p = new_node_with_ch(temp,0,0,0,0, l_ch, e5_p(fd));
        return p;
      }
      return l_ch;
      default:
        return NULL;
  }
}


static ast_node * e5_p(FILE *fd) {
  printf("e5_p\n");
  switch( lookahead ){
    case PLUS: 
      match(PLUS, fd);
      return e5_h(fd);
    case MINUS:
      match(MINUS, fd);
      return e5_h(fd);
    default:
      return NULL;
  }
}

static ast_node * e6(FILE *fd) {
  printf("e6\n");
  return e6_h(fd);
}

static ast_node *e6_h(FILE *fd) {
  ast_node * l_ch = NULL;
  switch( lookahead ){
    case NOT: case MINUS: case NUM: case LPAREN: case ID:
      l_ch = e7(fd);
      tokenT temp = lookahead;
      if(temp == STAR || temp == DIV){
        ast_node * p = new_node_with_ch(temp,0,0,0,0, l_ch, e6_p(fd));
        return p;
      }
      return l_ch;
      default:
        return NULL;
  }
}

static ast_node * e6_p(FILE *fd) {
  printf("e6_p\n");
  switch( lookahead ){
    case STAR: 
      match(STAR, fd);
      return e6_h(fd);
    case DIV:
      match(DIV, fd);
      return e6_h(fd);
    default:
      return NULL;
  }
}

static ast_node * e7(FILE *fd) {
  printf("e7\n");
  switch(lookahead) {
    case NOT:
      return e7_h(NOT, fd);
    case MINUS:
      return e7_h(MINUS, fd);
    default:
      return e8(fd);
    break;
  }
  return NULL;
}

static ast_node *e7_h(tokenT token, FILE* fd) {
  match(token, fd);
  ast_node * n = new_node(token,0,0,0,0);
  expression_rec(fd);
  add_child_node(n, e7(fd));
  return n;
}

static ast_node * e8(FILE *fd) {
  printf("e8\n");
  ast_node * n = NULL;
  switch(lookahead) {
    case NUM:
      n = new_node(NUM,tokenval,0,0,0);
      match(NUM, fd);
    break;
    case LPAREN:
      match(LPAREN, fd);
      n = expr(fd);
      match(RPAREN, fd);
    break;
    case ID:
      n = new_node(ID,0,0,lexbuf,0);
      match(ID, fd); 
      if(lookahead == LPAREN || lookahead == LBRACKET){
        add_child_node(n, e8_p(fd, n));
      }
      break;
    default:
      while( lookahead != ID && lookahead != NUM && lookahead != LPAREN) {
        warning("Expected a different symbol at expression", -1,-1);
        lookahead = lexan(fd);
        if(lookahead == DONE){
          parser_error("Reached the end of the file.", -1, -1);
        }
      }
      e8(fd);
    break;
  }
  return n;
}

static ast_node * e8_p(FILE *fd, ast_node *parent) {
  printf("e8_p\n");
  switch( lookahead ) {
    case LPAREN:
      match(LPAREN, fd);
      ast_node * expr_l = new_node(NONTERMINAL, 0, EXPR_LIST,0,0);
      expr_list(fd, expr_l);
      match(RPAREN, fd);
      return expr_l;
      break;
    case LBRACKET:
      match(LBRACKET, fd);
      add_child_node(parent, new_node(LBRACKET, 0,0,0,0));
      add_child_node(parent,expr(fd));
      match(RBRACKET, fd);
      return new_node(RBRACKET,0,0,0,0);
      break;
    default:
      return NULL;
  }
  return NULL;
}

static void expr_list(FILE *fd, ast_node *parent) {
  printf("expr_list\n");
  switch(lookahead) {
    case MINUS: case NOT: 
    case LPAREN: case ID: case NUM:
      add_child_node(parent, expr(fd));
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
  while(token != lookahead) {
      warning("Expected a different value", token, lookahead);
      lookahead = lexan(fd);
      if(lookahead == DONE) {
        parser_error("Reached the end of the file.", token, lookahead);
      }
  }
  printf("MATCH: ");
  lexer_emit(lookahead, tokenval);
  lookahead = lexan(fd);  
}