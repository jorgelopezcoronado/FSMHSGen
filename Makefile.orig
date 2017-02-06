MAIN=spa
STRUCTS=paren.c paren.h
FILES=lexer.c paren.c parser.c

lexer: lexer.l $(STRUCTS)
	lex lexer.l

parser: lexer parser.y lexer.c $(STRUCTS)
	bison parser.y

c: parser $(MAIN).c
	gcc -o $(MAIN) $(FILES) $(MAIN).c

cpp: parser $(MAIN).cpp
	g++ -Wno-write-strings -o $(MAIN) $(FILES) $(MAIN).cpp
clean: 
	rm -rf lexer.c lexer.h parser.c parser.h $(MAIN)

all: c

redo: clean c
