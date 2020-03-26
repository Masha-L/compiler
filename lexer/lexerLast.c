/* Starter code provided to students */

/* The lexical analyzer for the C-- Programming Language
 */
// TODO: you are welcome to completely wipe out the contents of this
// file and start from scratch, or use this as your starting point.

#include <stdlib.h>
#include <assert.h>
#include "lexer.h"

// these are likely values that will be needed by the parser, so 
// making them global variables is okay
char lexbuf[MAXLEXSIZE];  // stores current lexeme
int  tokenval=0;          // stores current token's value
                          // (might not be used for every token)
int  src_lineno=0;        // current line number in source code input

int src_charno = 0;

// function prototypes:
static void print_lineno();  // static limits its scope to only in this .c file
static int is_word_over(char c);
static int isDig(char c);
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
  //find where we left o
  fseek(fd, src_charno, SEEK_SET);

  char c;
  int start;

  while((c = fgetc(fd))!= EOF){
    start = src_charno;
    switch(c)
    {
      // comments and div
      case '/': 
        src_charno = start - (fd -> _r);
        c = fgetc(fd);
        if('/'==c) {
          do {
            src_charno = start - (fd -> _r);
            c = fgetc(fd);
          } while( (c) != '\n');
          continue;
        }
        else if ('*' == c) {
          src_charno = start - (fd -> _r);
          while(1) {
            src_charno = start - (fd -> _r);
            c = fgetc(fd);
            if(c=='*' && (c = fgetc(fd))=='/') {
              break;
            }
            if(c == EOF) {
              return DONE;
            }
          }
          continue;
        }
        return DIV;
      // if and int
      case 'i':
          c = fgetc(fd);
          src_charno = start - (fd -> _r);
          switch(c)
          {
            case 'f':
              c = fgetc(fd);
              src_charno = start - (fd -> _r);
              if(is_word_over(c)==1) {
                return IF;
              }
              else {
                return ID;

              }
              //and the next symol is a symbol (not _), bracket, space, tab or empty line
              return IF;
            case 'n':
              c = fgetc(fd);
              src_charno = start - (fd -> _r);
              if(c == 't') {
                return INT;
              }
            default:
              return ID;
          }
        break;
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
      {
        int a = c-48;
        src_charno = start - (fd -> _r);
        while(isDig(c=fgetc(fd))) {
          src_charno = start - (fd -> _r);
          a = a*10 + (c - '0');
        }
        tokenval = a;
        return NUM;
      }
      case '&':
        if((c = fgetc(fd))=='&') {
          return AND;
        }
        return LEXERROR;
      case '=':
        src_charno = start - (fd -> _r);
        if((c = fgetc(fd))=='=') {
          src_charno = start - (fd -> _r);
          return EQ;
        }
        return ASSIGN;
      case 'b':
          c = fgetc(fd);
          src_charno = start - (fd -> _r);
          if(c == 'r' &&
            (c = fgetc(fd))=='e' &&
            (c = fgetc(fd))=='a' &&
            (c = fgetc(fd))=='k') 
          {
            src_charno = start - (fd -> _r);
            c = fgetc(fd);
              if(is_word_over(c)==1) {
                return BREAK;
              }
            return ID;
          }
          return ID;
      case 'f':
          c = fgetc(fd);
          src_charno = start - (fd -> _r);
          if(c == '0' &&
            (c = fgetc(fd))=='r')
          {
            src_charno = start - (fd -> _r);
            c = fgetc(fd);
              if(is_word_over(c)==1) {
                return BREAK;
              }
            return ID;
          }
          return ID;
      case ',':
        src_charno = start - (fd -> _r);
        return COMMA;
      case '{':
        src_charno = start - (fd -> _r);
        return LBRACE;
      case '}':
        src_charno = start - (fd -> _r);
        return RBRACE;
      case '(':
        src_charno = start - (fd -> _r);
        return LPAREN;
      case ')':
        src_charno = start - (fd -> _r);
        return RPAREN;
      case '[':
        src_charno = start - (fd -> _r);
        return LBRACKET;
      case ']':
        src_charno = start - (fd -> _r);
        return RBRACKET;
      case ';':
        src_charno = start - (fd -> _r);
        return SEMIC;      
      case '-':
        src_charno = start - (fd -> _r);
        return MINUS;   
      case '!':
        src_charno = start - (fd -> _r);
        if((c = fgetc(fd))=='=') {
          src_charno = start - (fd -> _r);
          return NEQ;
        }
        return NOT;
      case '<':
        src_charno = start - (fd -> _r);
        if((c = fgetc(fd))=='=') {
          src_charno = start - (fd -> _r);
          return LESSEQ;
        }
        return LESS;
      case '>':
        src_charno = start - (fd -> _r);
        if((c = fgetc(fd))=='=') {
          src_charno = start - (fd -> _r);
          return MOREEQ;
        }
        return MORE;
      case '+': 
        src_charno = start - (fd -> _r);
        return PLUS;  
      case '*': 
        src_charno = start - (fd -> _r);
        return STAR;     
      default:
        while(is_word_over(fgetc(fd))!=1) {}
        src_charno = start - (fd -> _r);
        return OR;
    }

    printf("%c", c);
    src_charno = src_charno - (fd -> _r);
    //save current location in file
    if(c == ' ') {
      break;
    }
  }


  // bogus code - we need it till it all works:
  if(c == '\n' && src_lineno < 100) {
    src_lineno++;
  }

  if(src_lineno < 59) {
    return COMMA;
  }
  print_lineno();
  return DONE;
}
/***************************************************************************/
// A function for demonstrating that functions should be declared static
// if they are to be used only in the file in which they are defined.
// Static limits the scope to only this .c file
static void print_lineno() {
  
  printf("line no = %d\n", src_lineno);

}

static int is_word_over(char c) {
  switch(c){
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
  switch(c) {
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
        return 1;
      default:
        return 0;
  }
}

