LEX = flex
YACC = yacc

LEX_SRC = NewGramma.l
YACC_SRC = Gramma.y

all			:Gramma

Gramma GRAMMA gramma	:y.tab.o lex.yy.o
			cc -g -o Gramma y.tab.o lex.yy.o -lfl
#//			-rm y.tab.c; rm y.tab.h; rm y.tab.o; rm lex.yy.c; rm lex.yy.o

y.tab.o			: y.tab.c y.tab.h
			cc -c -g  -o y.tab.o y.tab.c
#			cc -c -Wall -o y.tab.o y.tab.c

lex.yy.o		: lex.yy.c y.tab.h
			cc -c -g -o lex.yy.o lex.yy.c
#			cc -c -Wall -o lex.yy.o lex.yy.c

y.tab.c y.tab.h		: $(YACC_SRC)
			$(YACC) -d $(YACC_SRC)

lex.yy.c		:$(LEX_SRC)
			$(LEX) $(LEX_SRC)
clean:
			-rm y.tab.c y.tab.h y.tab.o lex.yy.c lex.yy.o &> /dev/null
cleanall:
			-rm y.tab.c y.tab.h y.tab.o lex.yy.c lex.yy.o Gramma grammar.cc &> /dev/null
