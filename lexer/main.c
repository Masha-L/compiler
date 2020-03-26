/*
 *  Main function for testing the C-- lexical analyzer.
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"

int main(int argc, char *argv[]) {

  tokenT token;
  FILE *fd;

  if(argc <= 1) {
      printf("usage: lexer infile.c--\n");
      exit(1);
  }
  fd = fopen(argv[1], "r");
  if(fd == 0) {
      printf("error opening file: %s\n", argv[1]);
      exit(1);
  }
  token = IF; 
  while(token != DONE && token != LEXERROR) {
      token = lexan(fd);
      lexer_emit(token, tokenval);
  }
  if( token == LEXERROR ) {
      lexer_error("invalid symbol", tokenval);	
      exit(1);     /*  unsuccessful termination  */
  }
  fclose(fd);
  exit(0);     /*  successful termination  */
}
