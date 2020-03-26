/* Starter code provided to students */

/* The lexical analyzer for the C-- Programming Language
 */
// TODO: you are welcome to completely wipe out the contents of this
// file and start from scratch, or use this as your starting point.

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "lexer.h"

// these are likely values that will be needed by the parser, so
// making them global variables is okay
char lexbuf[MAXLEXSIZE]; // stores current lexeme
int tokenval = 0;        // stores current token's value
                         // (might not be used for every token)
int src_lineno = 0;      // current line number in source code input
int src_charno = 0;

// function prototypes:
static void print_lineno(); // static limits its scope to only in this .c file
static int is_word_over(char c);
static int isDig(char c);
static void set_up(FILE * fd);
static int getDig(char c, FILE * fd);
static void skipComment(char c, FILE * fd);
static int getKeyOrID(char c, FILE * fd);

/***************************************************************************/
/* 
 *  Main lexer routine:  returns the next token in the input 
 *
 *  param fd: file pointer for reading input file (source C-- program)
 *            TODO: you may want to add more parameters
 *
 *  returns: the next token, or 
 *           DONE if there are no more tokens, or
 *           LEXERROR if there is a token parsing error 
 */
int lexan(FILE *fd) {
  char c;
  set_up(fd);
  while ((c = fgetc(fd)) != EOF) {
    src_charno += 1;
    if(isDig(c) == 1) {
      return getDig(c, fd);
    }
    switch (c) {
      case ',': return COMMA;
      case '{': return LBRACE;
      case '}': return RBRACE;
      case '(': return LPAREN;
      case ')': return RPAREN;
      case '[': return LBRACKET;
      case ']': return RBRACKET;
      case ';': return SEMIC;
      case '+': return PLUS;
      case '*': return STAR;
      case '-': return MINUS;
      case '.': return PERIOD;    
      case '&':
        if ((c = fgetc(fd)) == '&') {
          src_charno += 1;
          return AND;
        }
        return LEXERROR;
      case '|':
        if ((c = fgetc(fd)) == '|') {
          src_charno += 1;
          return OR;
        }
        return LEXERROR;
      case '!':
        if ((c = fgetc(fd)) == '=') {
          src_charno += 1;
          return NEQ;
        }
        return NOT;
      case '<':
        if ((c = fgetc(fd)) == '=') {
          src_charno += 1;
          return LESSEQ;
        }
        return LESS;
      case '>':
        if ((c = fgetc(fd)) == '=') {
          src_charno += 1;
          return MOREEQ;
        }
        return MORE;
      case '=':
        src_charno += 1;
        if ((c = fgetc(fd)) == '=') {
          src_charno += 1;
          return EQ;
        }
        return ASSIGN;
      case '/':
        if ((c = fgetc(fd)) != '/' && c != '*') {
          src_charno += 1;
          return DIV;
        }
        // if  comment
        skipComment(c, fd);
        continue;
      case '\n':
      case ' ':
        continue;
      default:
        return getKeyOrID(c, fd);
    }
  }
    return DONE;
}

static void set_up(FILE * fd) {
  fseek(fd, src_charno, SEEK_SET);
}

static int is_word_over(char c) {
  switch (c) {
  case ';':
  case ',':
  case '.':
  case '{':
  case '[':
  case '(':
  case '}':
  case ']':
  case ')':
  case ' ':
  case '\n':
  case '\t':
  case ':':
  case '<':
  case '>':
  case '=':
  case '+':
  case '-':
  case '*':
  case '\0':
  case '!':
  case '&':
  case '/':
    return 1;
  default:
    return 0;
  }
}

static int isDig(char c) {
  switch (c) {
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
  case '0':
  case '\'':
    return 1;
  default:
    return 0;
  }
}

static int getDig(char c, FILE * fd) {
    int a = c - 48;
    if(c=='\'') {
      a = fgetc(fd);
      if (fgetc(fd) != '\'') {
        return LEXERROR;
      }
      src_charno += 2;
    } else {
      while(isDig(c = fgetc(fd))) {
        src_charno += 1;
        a = a * 10 + (c - '0');
      }
    }
    tokenval = a;
    return NUM;
}


static void skipComment(char c, FILE * fd) {
  if (c == '/') {
    src_charno += 2;
    while ((c = fgetc(fd)) != '\n') {
      src_charno += 1;
    }
  } else if (c == '*') {
    c = fgetc(fd);
    src_charno += 2;
    while (c != '*' && (c = fgetc(fd)) != '/') {
      src_charno += 1;
    }
    c = fgetc(fd);
    src_charno += 1;
  }
}

static int getKeyOrID(char c, FILE * fd) {
  lexbuf[0] = c;
  int i = 1;
  while (is_word_over((c=fgetc(fd))) != 1) {
    lexbuf[i] = c;
    i++;
    src_charno += 1;
  }
  // binary search
  lexbuf[i] = 0;
  int compare = strcmp(lexbuf, "int");
  if(compare > 0) {
    compare = strcmp(lexbuf, "while");
    if(compare > 0) {
      if(strcmp(lexbuf, "writeln")==0) { return WRITELN; }
      if(strcmp(lexbuf, "write")==0) { return WRITE; }
      return ID;
    } else if(compare < 0) {
      if(strcmp(lexbuf, "read")==0) { return READ; }
      if(strcmp(lexbuf, "return")==0) { return RETURN; }
      return ID;
    }
    return WHILE;
  } else if(compare < 0) {
    compare = strcmp(lexbuf, "else");
    if(compare > 0) {
      if(strcmp(lexbuf, "for")==0) { return FOR; }
      if(strcmp(lexbuf, "if")==0) { return IF; }
      return ID;
    } else if(compare < 0) {
      if(strcmp(lexbuf, "break")==0) { return BREAK; }
      if(strcmp(lexbuf, "char")==0) { return CHAR; }
      return ID;
    }
    return ELSE;
  }
  return INT;
}