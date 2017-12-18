CC = cc -g
LEX = flex
YACC = bison
CFLAGS = -DYYDEBUG=1

PROGRAMS4 = sql

all: ${PROGRAMS4}

sql: sql.tab.o sql.o
	${CC} -o $@ sql.tab.o sql.o

sql.tab.c sql.tab.h: sql.y
	${YACC} -vd sql.y

sql.c: sql.l
	${LEX} -o $@ $<

sql.o: lex.yy.c sql.tab.h

clean:
	rm -f sql sql.tab.c sql.tab.h sql.c sql.tab.o sql.o sql.output

.SUFFIXES:	.l .y .c

