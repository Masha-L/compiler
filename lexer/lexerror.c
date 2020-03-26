//
// Error routines
//
#include <stdlib.h>
#include "lexer.h"

//
// generates an error message
//
void lexer_error(char *m, int lineno)  {
  fprintf(stderr, "line %d: %s\n", lineno, m);
  exit(1);   /*   unsuccessful termination  */
}
