#include "lexer.h"
//
// generates the lexer's output
//   t: the token
//   tval: token value
//
void lexer_emit(int t, int tval) {  
  switch(t) {
    case PERIOD:
      printf("PERIOD\n"); 
      break;
    case CHAR:
      printf("CHAR\n"); 
      break;
    case AND:
      printf("AND\n"); 
      break;
    case IF:
      printf("IF\n"); 
      break;
    case INT:
      printf("INT\n"); 
      break;
    case NEQ:
      printf("NEQ\n"); 
      break;
    case NOT:
      printf("NOT\n"); 
      break;
    case ELSE:
      printf("ELSE\n"); 
      break;
    case READ:
      printf("READ\n");
      break;
    case RETURN:
      printf("RETURN\n");
      break;
    case ID:
      printf("ID.%s\n", lexbuf); 
      break;
    case LBRACKET:
      printf("LBRACKET\n"); 
      break;
    case RBRACKET:
      printf("RBRACKET\n"); 
      break;
    case LBRACE:
      printf("LBRACE\n"); 
      break;
    case RBRACE:
      printf("RBRACE\n"); 
      break;
    case LPAREN:
      printf("LPAREN\n"); 
      break;
    case RPAREN:
      printf("RPAREN\n"); 
      break;
    case EQ:
      printf("EQ\n"); 
      break;
    case ASSIGN:
      printf("ASSIGN\n"); 
      break;
    case DIV:
      printf("DIV\n"); 
      break;
    case MINUS:
      printf("MINUS\n");
      break;
    case MORE:
      printf("MORE\n");
      break;
    case MOREEQ:
      printf("MOREEQ\n");
      break;
    case LESS:
      printf("LESS\n");
      break;
    case LESSEQ:
      printf("LESSEQ\n");
      break;
    case SEMIC:
      printf("SEMIC\n"); 
      break;
    case BREAK:
      printf("BREAK\n"); 
      break;
    case COMMA:
      printf("COMMA\n"); 
      break;
    case STAR:
      printf("STAR\n"); 
      break;
    case PLUS:
      printf("PLUS\n"); 
      break;
    case FOR:
      printf("FOR\n"); 
      break;
    case NUM:
      printf("NUM.%d\n", tval); 
      break;
    case WHILE:
      printf("WHILE\n"); 
      break;
    case WRITE:
      printf("WRITE\n"); 
      break;
    case WRITELN:
      printf("WRITELN\n"); 
      break;
    default:
      // printf("token %c\n", tval); 
      break;
  }
}
