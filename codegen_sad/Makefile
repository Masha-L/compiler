#
# compiles mycc, a compiler for C-- 
#
# make depend: automatically build .o file dependencies
# make: build mycc
# make clean: removes all .o and executable files
#

CC = gcc
CFLAGS = -Wall -g

LFLAGS = 
LIBS = 

INCLUDES =  -I../includes

# add additional source files here
SRCS = ../ast/ast.c ../lexer/lexer.c ../parser/parser.c \
       ../symtab/symtab.c codegen.c codetable.c main.c \
       ../lexer/lexemitter.c

OBJS = $(SRCS:.c=.o)


MAIN = mycc

# generic part of makefile...don't change this
all:    $(MAIN) 
	@echo  Simple parser named parser has been compiled

$(MAIN):  $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS) 

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

