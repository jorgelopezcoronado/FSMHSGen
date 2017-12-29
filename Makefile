MAIN=FSMHSGen
STRUCTS=helpers.c helpers.h linked_list_node.h linked_list.c linked_list.h  FSM.c FSM.h integer_set.h integer_set.c FSM_Gedanken_experiments.c FSM_Gedanken_experiments.h specification_metrics.h specification_metrics.c 
PARSER=lexer.c parser.c fpl.c fpp.c
FLAGS=-O3
#LDFLAGS=-arch x86_64 -syslibroot /Library/Developer/CommandLineTools/SDKs/MacOSX10.12.sdk

#FLAGS=-g -std=gnu89 

all: c

lexer: lexer.l fp.l $(STRUCTS)
	lex lexer.l
	lex fp.l

parser: lexer parser.y lexer.c fpl.c fp.y $(STRUCTS)
	bison parser.y
	bison fp.y

c: parser $(PARSER) $(MAIN).c $(STRUCTS) 
	gcc $(FLAGS) $(LDFLAGS) -c helpers.c 
	gcc $(FLAGS) $(LDFLAGS) -c linked_list.c 
	gcc $(FLAGS) $(LDFLAGS) -c FSM.c $(LDFLAGS)
	gcc $(FLAGS) $(LDFLAGS) -c integer_set.c
	gcc $(FLAGS) $(LDFLAGS) -c FSM_Gedanken_experiments.c
	gcc $(FLAGS) $(LDFLAGS) -c specification_metrics.c 
	gcc $(FLAGS) $(LDFLAGS) -o $(MAIN) *.o $(PARSER) $(MAIN).c 

clean: 
	rm -rf fpl.h fpl.c fpp.h fpp.c lexer.c lexer.h parser.c parser.h *.gch *.o $(MAIN)

redo: clean c
