MAIN=FSMHSGen
STRUCTS=helpers.c helpers.h linked_list_node.h linked_list.c linked_list.h  FSM.c FSM.h integer_set.h integer_set.c FSM_Gedanken_experiments.c FSM_Gedanken_experiments.h specification_metrics.h specification_metrics.c 
PARSER=lexer.c parser.c fpl.c fpp.c
FLAGS=-O3 
#FLAGS=-g -std=gnu89 

all: c

lexer: lexer.l fp.l $(STRUCTS)
	lex lexer.l
	lex fp.l

parser: lexer parser.y lexer.c fpl.c fp.y $(STRUCTS)
	bison parser.y
	bison fp.y

c: parser $(PARSER) $(MAIN).c $(STRUCTS) 
	gcc $(FLAGS)-c helpers.c
	gcc $(FLAGS) -c linked_list.c
	gcc $(FLAGS) -c FSM.c
	gcc $(FLAGS) -c integer_set.c
	gcc $(FLAGS) -c FSM_Gedanken_experiments.c
	gcc $(FLAGS) -c specification_metrics.c
	gcc $(FLAGS) -o $(MAIN) *.o $(PARSER) $(MAIN).c

clean: 
	rm -rf fpl.h fpl.c fpp.h fpp.c lexer.c lexer.h parser.c parser.h *.gch *.o $(MAIN)

redo: clean c
