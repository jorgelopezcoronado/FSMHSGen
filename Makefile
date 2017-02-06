MAIN=FSMHSGen
STRUCTS=helpers.c helpers.h linked_list_node.h linked_list.c linked_list.h  FSM.c FSM.h 
PARSER=lexer.c parser.c
FLAGS=-std=gnu89

lexer: lexer.l $(STRUCTS)
	lex lexer.l

parser: lexer parser.y lexer.c $(STRUCTS)
	bison parser.y

#c: parser $(MAIN).c

	#gcc -o $(MAIN) $(FILES) $(MAIN).c
c: $(MAIN).c $(STRUCTS) 
	gcc -c helpers.h helpers.c
	gcc -c linked_list_node.h linked_list.h linked_list.c
	gcc -c FSM.h FSM.c
	gcc $(FLAGS) -o $(MAIN) *.o $(MAIN).c

clean: 
	rm -rf lexer.c lexer.h parser.c parser.h *.gch *.o $(MAIN)

all: c

redo: clean c
