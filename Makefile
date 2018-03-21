all: idice

LDLIBS= -lsodium
CFLAGS= -O3
YFLAGS= -d

idice: eval.o parser.o type.o lex.yy.o

lex.yy.c: lexer.ll parser.c
	lex lexer.ll

clean:
	rm *.o
	rm lex.yy.c
	rm parser.c
	rm y.tab.h
	rm idice
