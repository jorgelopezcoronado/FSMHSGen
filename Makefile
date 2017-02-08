MAIN=FSMHSGen
#MAIN=FSMTest
STRUCTS=helpers.c helpers.h linked_list_node.h linked_list.c linked_list.h  FSM.c FSM.h 
PARSER=lexer.c parser.c
FLAGS=-g -std=gnu89 

all: c

lexer: lexer.l $(STRUCTS)
	lex lexer.l

parser: lexer parser.y lexer.c $(STRUCTS)
	bison parser.y

c: parser $(PARSER) $(MAIN).c $(STRUCTS) 
	gcc $(FLAGS)-c helpers.h helpers.c
	gcc $(FLAGS) -c linked_list_node.h linked_list.h linked_list.c
	gcc $(FLAGS) -c FSM.h FSM.c
	gcc $(FLAGS) -o $(MAIN) *.o $(PARSER) $(MAIN).c

clean: 
	rm -rf lexer.c lexer.h parser.c parser.h *.gch *.o $(MAIN)

redo: clean c
