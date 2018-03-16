all: idice

LDLIBS= -lsodium
CFLAGS= -O3
idice: eval.o parser.o type.o lex.yy.o


lex.yy.c: lexer.ll parser.c
	lex lexer.ll
